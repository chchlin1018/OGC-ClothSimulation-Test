#include "ui/OpenGLWidget.h"
#include "physics/ClothSimulation.h"
#include <QDebug>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

namespace UI {

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_animationTimer(new QTimer(this))
    , m_animating(false)
    , m_cameraPosition(0, 5, 10)
    , m_cameraTarget(0, 0, 0)
    , m_cameraUp(0, 1, 0)
    , m_cameraDistance(10.0f)
    , m_cameraYaw(0.0f)
    , m_cameraPitch(-20.0f)
    , m_mousePressed(false)
    , m_showWireframe(true)
    , m_showParticles(true)
    , m_showColliders(true)
{
    // 設定動畫計時器
    m_animationTimer->setInterval(16); // ~60 FPS
    connect(m_animationTimer, &QTimer::timeout, this, &OpenGLWidget::updateAnimation);
    
    // 啟用滑鼠追蹤
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    // 清理 OpenGL 資源
    doneCurrent();
}

void OpenGLWidget::setClothSimulation(std::shared_ptr<Physics::ClothSimulation> simulation) {
    m_clothSimulation = simulation;
    update();
}

void OpenGLWidget::setAnimating(bool animate) {
    m_animating = animate;
    if (animate) {
        m_animationTimer->start();
    } else {
        m_animationTimer->stop();
    }
}

void OpenGLWidget::resetCamera() {
    m_cameraDistance = 10.0f;
    m_cameraYaw = 0.0f;
    m_cameraPitch = -20.0f;
    updateCamera();
    update();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    
    // 設定 OpenGL 狀態
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    
    // 設定背景顏色
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    
    qDebug() << "OpenGL 初始化完成";
    qDebug() << "OpenGL 版本:" << reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 設定投影和視圖矩陣
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projection.constData());
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_view.constData());
    
    // 渲染座標系
    renderCoordinateSystem();
    
    // 渲染布料
    if (m_clothSimulation) {
        renderCloth();
        
        if (m_showColliders) {
            renderColliders();
        }
    }
}

void OpenGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    
    // 設定投影矩陣
    m_projection.setToIdentity();
    float aspect = float(width) / float(height ? height : 1);
    m_projection.perspective(45.0f, aspect, 0.1f, 100.0f);
    
    updateCamera();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
    m_lastMousePos = event->pos();
    m_mousePressed = true;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!m_mousePressed) return;
    
    QPoint delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();
    
    // 旋轉相機
    m_cameraYaw += delta.x() * 0.5f;
    m_cameraPitch += delta.y() * 0.5f;
    
    // 限制俯仰角
    m_cameraPitch = qBound(-89.0f, m_cameraPitch, 89.0f);
    
    updateCamera();
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
    // 縮放相機
    float delta = event->angleDelta().y() / 120.0f;
    m_cameraDistance *= (1.0f - delta * 0.1f);
    m_cameraDistance = qBound(1.0f, m_cameraDistance, 50.0f);
    
    updateCamera();
    update();
}

void OpenGLWidget::updateAnimation() {
    if (m_clothSimulation && m_animating) {
        m_clothSimulation->update(0.016f); // ~60 FPS
        update();
    }
}

void OpenGLWidget::setupCamera() {
    updateCamera();
}

void OpenGLWidget::updateCamera() {
    // 計算相機位置
    float yawRad = qDegreesToRadians(m_cameraYaw);
    float pitchRad = qDegreesToRadians(m_cameraPitch);
    
    m_cameraPosition.setX(m_cameraDistance * cos(pitchRad) * sin(yawRad));
    m_cameraPosition.setY(m_cameraDistance * sin(pitchRad));
    m_cameraPosition.setZ(m_cameraDistance * cos(pitchRad) * cos(yawRad));
    
    // 設定視圖矩陣
    m_view.setToIdentity();
    m_view.lookAt(m_cameraPosition, m_cameraTarget, m_cameraUp);
}

void OpenGLWidget::renderCloth() {
    if (!m_clothSimulation) return;
    
    glPushMatrix();
    
    // 渲染粒子
    if (m_showParticles) {
        glColor3f(1.0f, 0.2f, 0.2f);
        glPointSize(4.0f);
        glBegin(GL_POINTS);
        
        for (int i = 0; i < m_clothSimulation->getParticleCount(); ++i) {
            // 這裡需要訪問粒子位置，但由於封裝性，我們使用渲染方法
            // 實際實現中應該提供適當的訪問接口
        }
        
        glEnd();
    }
    
    // 使用布料模擬的內建渲染方法
    if (m_showWireframe) {
        m_clothSimulation->renderWireframe();
    } else {
        m_clothSimulation->render();
    }
    
    glPopMatrix();
}

void OpenGLWidget::renderColliders() {
    if (!m_clothSimulation) return;
    
    glPushMatrix();
    
    // 使用布料模擬的碰撞體渲染方法
    m_clothSimulation->renderColliders();
    
    glPopMatrix();
}

void OpenGLWidget::renderCoordinateSystem() {
    glPushMatrix();
    
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    
    // X 軸 (紅色)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    
    // Y 軸 (綠色)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    
    // Z 軸 (藍色)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    
    glEnd();
    
    glPopMatrix();
}

void OpenGLWidget::drawSphere(const QVector3D& center, float radius, int segments) {
    glPushMatrix();
    glTranslatef(center.x(), center.y(), center.z());
    
    // 簡化的球體渲染
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(angle), 0, radius * sin(angle));
    }
    glEnd();
    
    glPopMatrix();
}

void OpenGLWidget::drawCylinder(const QVector3D& center, float radius, float height, int segments) {
    glPushMatrix();
    glTranslatef(center.x(), center.y(), center.z());
    
    // 底面圓
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(angle), -height/2, radius * sin(angle));
    }
    glEnd();
    
    // 頂面圓
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(angle), height/2, radius * sin(angle));
    }
    glEnd();
    
    // 側面線條
    glBegin(GL_LINES);
    for (int i = 0; i < segments; i += 4) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, -height/2, z);
        glVertex3f(x, height/2, z);
    }
    glEnd();
    
    glPopMatrix();
}

void OpenGLWidget::drawLine(const QVector3D& start, const QVector3D& end) {
    glBegin(GL_LINES);
    glVertex3f(start.x(), start.y(), start.z());
    glVertex3f(end.x(), end.y(), end.z());
    glEnd();
}

void OpenGLWidget::drawPoint(const QVector3D& position, float size) {
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3f(position.x(), position.y(), position.z());
    glEnd();
}

} // namespace UI
