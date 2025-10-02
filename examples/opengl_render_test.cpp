#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QStatusBar>
#include <QFrame>
#include <QKeyEvent>
#include <memory>
#include "ui/OpenGLWidget.h"
#include "physics/ClothSimulation.h"

/**
 * @brief OpenGL 渲染測試程序
 * 
 * 這個程序專門測試 OpenGL 渲染功能，展示布料模擬的視覺效果。
 */
class OpenGLRenderTestWindow : public QMainWindow {
    Q_OBJECT

public:
    OpenGLRenderTestWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("OGC 布料模擬 - OpenGL 渲染測試");
        setMinimumSize(1000, 700);
        
        setupUI();
        initializeSimulation();
        connectSignals();
        
        qDebug() << "OpenGL 渲染測試窗口初始化完成";
    }

private slots:
    void onStartStopClicked() {
        m_isRunning = !m_isRunning;
        
        if (m_isRunning) {
            m_startStopButton->setText("停止渲染");
            m_openglWidget->setAnimating(true);
            m_statusLabel->setText("狀態: 渲染中");
        } else {
            m_startStopButton->setText("開始渲染");
            m_openglWidget->setAnimating(false);
            m_statusLabel->setText("狀態: 已停止");
        }
    }
    
    void onResetClicked() {
        m_clothSimulation->reset();
        m_openglWidget->update();
        m_statusLabel->setText("狀態: 已重置");
    }
    
    void onWireframeToggled() {
        bool show = m_wireframeButton->text() == "顯示線框";
        m_openglWidget->setShowWireframe(show);
        m_wireframeButton->setText(show ? "隱藏線框" : "顯示線框");
    }
    
    void onParticlesToggled() {
        bool show = m_particlesButton->text() == "顯示粒子";
        m_openglWidget->setShowParticles(show);
        m_particlesButton->setText(show ? "隱藏粒子" : "顯示粒子");
    }
    
    void onCollidersToggled() {
        bool show = m_collidersButton->text() == "顯示碰撞體";
        m_openglWidget->setShowColliders(show);
        m_collidersButton->setText(show ? "隱藏碰撞體" : "顯示碰撞體");
    }
    
    void onResetCameraClicked() {
        m_openglWidget->resetCamera();
    }
    
    void updateStats() {
        if (m_clothSimulation) {
            QString stats = QString("粒子: %1 | 約束: %2 | 時間: %3s")
                .arg(m_clothSimulation->getParticleCount())
                .arg(m_clothSimulation->getConstraintCount())
                .arg(m_clothSimulation->getSimulationTime(), 0, 'f', 2);
            m_statsLabel->setText(stats);
        }
    }

private:
    // UI 組件
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    UI::OpenGLWidget* m_openglWidget;
    QWidget* m_controlPanel;
    QVBoxLayout* m_controlLayout;
    
    QPushButton* m_startStopButton;
    QPushButton* m_resetButton;
    QPushButton* m_wireframeButton;
    QPushButton* m_particlesButton;
    QPushButton* m_collidersButton;
    QPushButton* m_resetCameraButton;
    
    QLabel* m_statusLabel;
    QLabel* m_statsLabel;
    QLabel* m_instructionsLabel;
    
    // 模擬相關
    std::shared_ptr<Physics::ClothSimulation> m_clothSimulation;
    bool m_isRunning;
    QTimer* m_statsTimer;
    
    void setupUI() {
        m_centralWidget = new QWidget(this);
        setCentralWidget(m_centralWidget);
        
        m_mainLayout = new QHBoxLayout(m_centralWidget);
        
        // 創建 OpenGL 視圖
        m_openglWidget = new UI::OpenGLWidget(this);
        m_openglWidget->setMinimumSize(700, 500);
        
        // 創建控制面板
        setupControlPanel();
        
        // 添加到主佈局
        m_mainLayout->addWidget(m_openglWidget, 1);
        m_mainLayout->addWidget(m_controlPanel);
        
        // 設定狀態欄
        statusBar()->showMessage("OpenGL 渲染測試就緒");
    }
    
    void setupControlPanel() {
        m_controlPanel = new QWidget(this);
        m_controlPanel->setFixedWidth(250);
        m_controlPanel->setStyleSheet(
            "QWidget { background-color: #f0f0f0; }"
            "QPushButton { padding: 8px; margin: 2px; font-size: 12px; }"
            "QLabel { margin: 4px; }"
        );
        
        m_controlLayout = new QVBoxLayout(m_controlPanel);
        
        // 標題
        QLabel* titleLabel = new QLabel("OpenGL 渲染測試", m_controlPanel);
        titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
        titleLabel->setAlignment(Qt::AlignCenter);
        m_controlLayout->addWidget(titleLabel);
        
        // 模擬控制
        m_startStopButton = new QPushButton("開始渲染", m_controlPanel);
        m_startStopButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
        m_controlLayout->addWidget(m_startStopButton);
        
        m_resetButton = new QPushButton("重置場景", m_controlPanel);
        m_controlLayout->addWidget(m_resetButton);
        
        // 分隔線
        QFrame* line1 = new QFrame(m_controlPanel);
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
        m_controlLayout->addWidget(line1);
        
        // 渲染選項
        QLabel* renderLabel = new QLabel("渲染選項:", m_controlPanel);
        renderLabel->setStyleSheet("font-weight: bold;");
        m_controlLayout->addWidget(renderLabel);
        
        m_wireframeButton = new QPushButton("隱藏線框", m_controlPanel);
        m_controlLayout->addWidget(m_wireframeButton);
        
        m_particlesButton = new QPushButton("隱藏粒子", m_controlPanel);
        m_controlLayout->addWidget(m_particlesButton);
        
        m_collidersButton = new QPushButton("隱藏碰撞體", m_controlPanel);
        m_controlLayout->addWidget(m_collidersButton);
        
        m_resetCameraButton = new QPushButton("重置相機", m_controlPanel);
        m_controlLayout->addWidget(m_resetCameraButton);
        
        // 分隔線
        QFrame* line2 = new QFrame(m_controlPanel);
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);
        m_controlLayout->addWidget(line2);
        
        // 狀態信息
        QLabel* statusTitleLabel = new QLabel("狀態信息:", m_controlPanel);
        statusTitleLabel->setStyleSheet("font-weight: bold;");
        m_controlLayout->addWidget(statusTitleLabel);
        
        m_statusLabel = new QLabel("狀態: 就緒", m_controlPanel);
        m_controlLayout->addWidget(m_statusLabel);
        
        m_statsLabel = new QLabel("統計: --", m_controlPanel);
        m_controlLayout->addWidget(m_statsLabel);
        
        // 分隔線
        QFrame* line3 = new QFrame(m_controlPanel);
        line3->setFrameShape(QFrame::HLine);
        line3->setFrameShadow(QFrame::Sunken);
        m_controlLayout->addWidget(line3);
        
        // 操作說明
        QLabel* instructionsTitleLabel = new QLabel("操作說明:", m_controlPanel);
        instructionsTitleLabel->setStyleSheet("font-weight: bold;");
        m_controlLayout->addWidget(instructionsTitleLabel);
        
        m_instructionsLabel = new QLabel(
            "• 滑鼠拖拽: 旋轉視角\n"
            "• 滾輪: 縮放視圖\n"
            "• 空格: 暫停/繼續\n"
            "• R: 重置場景\n"
            "• W: 切換線框模式\n"
            "• P: 切換粒子顯示",
            m_controlPanel
        );
        m_instructionsLabel->setStyleSheet("font-size: 10px; color: #666;");
        m_instructionsLabel->setWordWrap(true);
        m_controlLayout->addWidget(m_instructionsLabel);
        
        m_controlLayout->addStretch();
        
        // 版本信息
        QLabel* versionLabel = new QLabel("v1.0.0", m_controlPanel);
        versionLabel->setStyleSheet("font-size: 9px; color: #999;");
        versionLabel->setAlignment(Qt::AlignCenter);
        m_controlLayout->addWidget(versionLabel);
    }
    
    void initializeSimulation() {
        // 創建一個較小的布料以便更好地展示渲染效果
        m_clothSimulation = std::make_shared<Physics::ClothSimulation>(12, 12, 0.25f);
        
        // 添加圓柱體碰撞體
        m_clothSimulation->addCylinder(QVector3D(0, -0.8f, 0), 1.2f, 2.5f);
        
        // 設定物理參數以產生有趣的視覺效果
        m_clothSimulation->setGravity(QVector3D(0, -9.8f, 0));
        m_clothSimulation->setWind(QVector3D(2.0f, 0, 1.0f)); // 添加一些風力
        m_clothSimulation->setDamping(0.005f); // 較低的阻尼以保持運動
        
        // 啟用 OGC 模型
        m_clothSimulation->setUseOGC(true);
        m_clothSimulation->setOGCContactRadius(0.08f);
        
        // 設定到 OpenGL 視圖
        m_openglWidget->setClothSimulation(m_clothSimulation);
        
        // 初始化狀態
        m_isRunning = false;
        
        // 設定統計更新計時器
        m_statsTimer = new QTimer(this);
        m_statsTimer->setInterval(100); // 10 FPS
        connect(m_statsTimer, &QTimer::timeout, this, &OpenGLRenderTestWindow::updateStats);
        m_statsTimer->start();
        
        qDebug() << "渲染測試模擬初始化完成";
        qDebug() << "粒子數:" << m_clothSimulation->getParticleCount();
        qDebug() << "約束數:" << m_clothSimulation->getConstraintCount();
    }
    
    void connectSignals() {
        connect(m_startStopButton, &QPushButton::clicked, this, &OpenGLRenderTestWindow::onStartStopClicked);
        connect(m_resetButton, &QPushButton::clicked, this, &OpenGLRenderTestWindow::onResetClicked);
        connect(m_wireframeButton, &QPushButton::clicked, this, &OpenGLRenderTestWindow::onWireframeToggled);
        connect(m_particlesButton, &QPushButton::clicked, this, &OpenGLRenderTestWindow::onParticlesToggled);
        connect(m_collidersButton, &QPushButton::clicked, this, &OpenGLRenderTestWindow::onCollidersToggled);
        connect(m_resetCameraButton, &QPushButton::clicked, this, &OpenGLRenderTestWindow::onResetCameraClicked);
    }
    
protected:
    void keyPressEvent(QKeyEvent* event) override {
        switch (event->key()) {
        case Qt::Key_Space:
            onStartStopClicked();
            break;
        case Qt::Key_R:
            onResetClicked();
            break;
        case Qt::Key_W:
            onWireframeToggled();
            break;
        case Qt::Key_P:
            onParticlesToggled();
            break;
        case Qt::Key_C:
            onCollidersToggled();
            break;
        case Qt::Key_Escape:
            close();
            break;
        default:
            QMainWindow::keyPressEvent(event);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 設定應用程式資訊
    app.setApplicationName("OGC Cloth Simulation - OpenGL Render Test");
    app.setApplicationVersion("1.0.0");
    
    // 設定 OpenGL 格式
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4); // 4x MSAA
    QSurfaceFormat::setDefaultFormat(format);
    
    qDebug() << "=== OpenGL 渲染測試程序 ===";
    qDebug() << "Qt 版本:" << QT_VERSION_STR;
    
    // 創建並顯示測試窗口
    OpenGLRenderTestWindow window;
    window.show();
    
    qDebug() << "渲染測試窗口已顯示";
    qDebug() << "使用滑鼠和鍵盤與場景交互";
    
    return app.exec();
}

#include "opengl_render_test.moc"
