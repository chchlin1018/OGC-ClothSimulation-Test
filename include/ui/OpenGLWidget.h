#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector3D>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <memory>

namespace Physics {
class ClothSimulation;
}

namespace UI {

/**
 * @brief OpenGL 渲染 Widget
 * 
 * 這個類別負責渲染布料模擬的3D場景，包括布料、碰撞體和其他視覺元素。
 */
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    /**
     * @brief 設定布料模擬實例
     * @param simulation 布料模擬指標
     */
    void setClothSimulation(std::shared_ptr<Physics::ClothSimulation> simulation);

    /**
     * @brief 開始/停止動畫
     * @param animate 是否開始動畫
     */
    void setAnimating(bool animate);

    /**
     * @brief 設定是否顯示線框
     * @param show 是否顯示線框
     */
    void setShowWireframe(bool show) { m_showWireframe = show; update(); }

    /**
     * @brief 設定是否顯示粒子
     * @param show 是否顯示粒子
     */
    void setShowParticles(bool show) { m_showParticles = show; update(); }

    /**
     * @brief 設定是否顯示碰撞體
     * @param show 是否顯示碰撞體
     */
    void setShowColliders(bool show) { m_showColliders = show; update(); }

    /**
     * @brief 重置相機視角
     */
    void resetCamera();

protected:
    // QOpenGLWidget 重寫方法
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    // 滑鼠事件處理
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    /**
     * @brief 動畫更新槽
     */
    void updateAnimation();

private:
    // 布料模擬
    std::shared_ptr<Physics::ClothSimulation> m_clothSimulation;

    // 動畫控制
    QTimer* m_animationTimer;
    bool m_animating;

    // 相機控制
    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QVector3D m_cameraPosition;
    QVector3D m_cameraTarget;
    QVector3D m_cameraUp;
    float m_cameraDistance;
    float m_cameraYaw;
    float m_cameraPitch;

    // 滑鼠控制
    QPoint m_lastMousePos;
    bool m_mousePressed;

    // 渲染選項
    bool m_showWireframe;
    bool m_showParticles;
    bool m_showColliders;

    // 私有方法
    void setupCamera();
    void updateCamera();
    void renderCloth();
    void renderColliders();
    void renderCoordinateSystem();

    // OpenGL 輔助方法
    void drawSphere(const QVector3D& center, float radius, int segments = 16);
    void drawCylinder(const QVector3D& center, float radius, float height, int segments = 16);
    void drawLine(const QVector3D& start, const QVector3D& end);
    void drawPoint(const QVector3D& position, float size = 3.0f);
};

} // namespace UI
