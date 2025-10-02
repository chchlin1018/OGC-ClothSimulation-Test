#include "ui/MainWindow.h"
#include "ui/OpenGLWidget.h"
#include "physics/ClothSimulation.h"
#include <QApplication>
#include <QDebug>
#include <QTime>

namespace UI {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_openglWidget(nullptr)
    , m_controlPanel(nullptr)
    , m_isRunning(false)
    , m_statusTimer(new QTimer(this))
{
    setWindowTitle("OGC 布料模擬測試");
    setMinimumSize(1200, 800);
    
    // 初始化布料模擬
    initializeSimulation();
    
    // 設定 UI
    setupUI();
    
    // 連接信號
    connectSignals();
    
    // 設定狀態更新計時器
    m_statusTimer->setInterval(100); // 10 FPS
    connect(m_statusTimer, &QTimer::timeout, this, &MainWindow::updateStatus);
    m_statusTimer->start();
    
    qDebug() << "主窗口初始化完成";
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    
    // 創建 OpenGL 視圖
    m_openglWidget = new OpenGLWidget(this);
    m_openglWidget->setClothSimulation(m_clothSimulation);
    m_openglWidget->setMinimumSize(800, 600);
    
    // 創建控制面板
    m_controlPanel = new QWidget(this);
    m_controlPanel->setFixedWidth(300);
    m_controlPanel->setStyleSheet("QGroupBox { font-weight: bold; }");
    
    m_controlLayout = new QVBoxLayout(m_controlPanel);
    
    // 設定各個控制組
    setupSimulationGroup();
    setupSceneGroup();
    setupOGCGroup();
    setupRenderGroup();
    setupStatsGroup();
    
    m_controlLayout->addStretch();
    
    // 添加到主佈局
    m_mainLayout->addWidget(m_openglWidget, 1);
    m_mainLayout->addWidget(m_controlPanel);
    
    // 設定狀態欄
    statusBar()->showMessage("就緒");
}

void MainWindow::setupSimulationGroup() {
    m_simulationGroup = new QGroupBox("模擬控制", m_controlPanel);
    QVBoxLayout* layout = new QVBoxLayout(m_simulationGroup);
    
    // 開始/停止按鈕
    m_startStopButton = new QPushButton("開始", m_simulationGroup);
    m_startStopButton->setStyleSheet("QPushButton { font-size: 14px; padding: 8px; }");
    
    // 重置按鈕
    m_resetButton = new QPushButton("重置", m_simulationGroup);
    
    // 單步按鈕
    m_stepButton = new QPushButton("單步", m_simulationGroup);
    
    // 狀態標籤
    m_statusLabel = new QLabel("狀態: 停止", m_simulationGroup);
    
    layout->addWidget(m_startStopButton);
    layout->addWidget(m_resetButton);
    layout->addWidget(m_stepButton);
    layout->addWidget(m_statusLabel);
    
    m_controlLayout->addWidget(m_simulationGroup);
}

void MainWindow::setupSceneGroup() {
    m_sceneGroup = new QGroupBox("場景參數", m_controlPanel);
    QGridLayout* layout = new QGridLayout(m_sceneGroup);
    
    // 布料尺寸
    layout->addWidget(new QLabel("布料寬度:"), 0, 0);
    m_clothWidthSpinBox = new QSpinBox(m_sceneGroup);
    m_clothWidthSpinBox->setRange(5, 50);
    m_clothWidthSpinBox->setValue(15);
    layout->addWidget(m_clothWidthSpinBox, 0, 1);
    
    layout->addWidget(new QLabel("布料高度:"), 1, 0);
    m_clothHeightSpinBox = new QSpinBox(m_sceneGroup);
    m_clothHeightSpinBox->setRange(5, 50);
    m_clothHeightSpinBox->setValue(15);
    layout->addWidget(m_clothHeightSpinBox, 1, 1);
    
    // 重力
    layout->addWidget(new QLabel("重力:"), 2, 0);
    m_gravitySpinBox = new QDoubleSpinBox(m_sceneGroup);
    m_gravitySpinBox->setRange(-20.0, 0.0);
    m_gravitySpinBox->setValue(-9.8);
    m_gravitySpinBox->setSingleStep(0.1);
    layout->addWidget(m_gravitySpinBox, 2, 1);
    
    // 風力
    layout->addWidget(new QLabel("風力 X:"), 3, 0);
    m_windXSpinBox = new QDoubleSpinBox(m_sceneGroup);
    m_windXSpinBox->setRange(-10.0, 10.0);
    m_windXSpinBox->setValue(0.0);
    m_windXSpinBox->setSingleStep(0.1);
    layout->addWidget(m_windXSpinBox, 3, 1);
    
    layout->addWidget(new QLabel("風力 Y:"), 4, 0);
    m_windYSpinBox = new QDoubleSpinBox(m_sceneGroup);
    m_windYSpinBox->setRange(-10.0, 10.0);
    m_windYSpinBox->setValue(0.0);
    m_windYSpinBox->setSingleStep(0.1);
    layout->addWidget(m_windYSpinBox, 4, 1);
    
    layout->addWidget(new QLabel("風力 Z:"), 5, 0);
    m_windZSpinBox = new QDoubleSpinBox(m_sceneGroup);
    m_windZSpinBox->setRange(-10.0, 10.0);
    m_windZSpinBox->setValue(0.0);
    m_windZSpinBox->setSingleStep(0.1);
    layout->addWidget(m_windZSpinBox, 5, 1);
    
    // 阻尼
    layout->addWidget(new QLabel("阻尼:"), 6, 0);
    m_dampingSpinBox = new QDoubleSpinBox(m_sceneGroup);
    m_dampingSpinBox->setRange(0.0, 1.0);
    m_dampingSpinBox->setValue(0.01);
    m_dampingSpinBox->setSingleStep(0.001);
    m_dampingSpinBox->setDecimals(3);
    layout->addWidget(m_dampingSpinBox, 6, 1);
    
    m_controlLayout->addWidget(m_sceneGroup);
}

void MainWindow::setupOGCGroup() {
    m_ogcGroup = new QGroupBox("OGC 參數", m_controlPanel);
    QGridLayout* layout = new QGridLayout(m_ogcGroup);
    
    // OGC 啟用
    m_ogcEnabledCheckBox = new QCheckBox("啟用 OGC", m_ogcGroup);
    m_ogcEnabledCheckBox->setChecked(true);
    layout->addWidget(m_ogcEnabledCheckBox, 0, 0, 1, 2);
    
    // 接觸半徑
    layout->addWidget(new QLabel("接觸半徑:"), 1, 0);
    m_contactRadiusSpinBox = new QDoubleSpinBox(m_ogcGroup);
    m_contactRadiusSpinBox->setRange(0.01, 1.0);
    m_contactRadiusSpinBox->setValue(0.1);
    m_contactRadiusSpinBox->setSingleStep(0.01);
    m_contactRadiusSpinBox->setDecimals(3);
    layout->addWidget(m_contactRadiusSpinBox, 1, 1);
    
    m_controlLayout->addWidget(m_ogcGroup);
}

void MainWindow::setupRenderGroup() {
    m_renderGroup = new QGroupBox("渲染選項", m_controlPanel);
    QVBoxLayout* layout = new QVBoxLayout(m_renderGroup);
    
    // 顯示選項
    m_showWireframeCheckBox = new QCheckBox("顯示線框", m_renderGroup);
    m_showWireframeCheckBox->setChecked(true);
    layout->addWidget(m_showWireframeCheckBox);
    
    m_showParticlesCheckBox = new QCheckBox("顯示粒子", m_renderGroup);
    m_showParticlesCheckBox->setChecked(true);
    layout->addWidget(m_showParticlesCheckBox);
    
    m_showCollidersCheckBox = new QCheckBox("顯示碰撞體", m_renderGroup);
    m_showCollidersCheckBox->setChecked(true);
    layout->addWidget(m_showCollidersCheckBox);
    
    // 相機重置
    m_resetCameraButton = new QPushButton("重置相機", m_renderGroup);
    layout->addWidget(m_resetCameraButton);
    
    m_controlLayout->addWidget(m_renderGroup);
}

void MainWindow::setupStatsGroup() {
    m_statsGroup = new QGroupBox("統計資訊", m_controlPanel);
    QVBoxLayout* layout = new QVBoxLayout(m_statsGroup);
    
    m_particleCountLabel = new QLabel("粒子數: 0", m_statsGroup);
    m_constraintCountLabel = new QLabel("約束數: 0", m_statsGroup);
    m_simulationTimeLabel = new QLabel("模擬時間: 0.0s", m_statsGroup);
    m_fpsLabel = new QLabel("FPS: 0", m_statsGroup);
    
    layout->addWidget(m_particleCountLabel);
    layout->addWidget(m_constraintCountLabel);
    layout->addWidget(m_simulationTimeLabel);
    layout->addWidget(m_fpsLabel);
    
    m_controlLayout->addWidget(m_statsGroup);
}

void MainWindow::connectSignals() {
    // 模擬控制
    connect(m_startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(m_stepButton, &QPushButton::clicked, this, &MainWindow::onStepClicked);
    
    // 場景參數
    connect(m_clothWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onClothSizeChanged);
    connect(m_clothHeightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onClothSizeChanged);
    connect(m_gravitySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onGravityChanged);
    connect(m_windXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onWindChanged);
    connect(m_windYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onWindChanged);
    connect(m_windZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onWindChanged);
    connect(m_dampingSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onDampingChanged);
    
    // OGC 參數
    connect(m_ogcEnabledCheckBox, &QCheckBox::toggled, this, &MainWindow::onOGCEnabledChanged);
    connect(m_contactRadiusSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onContactRadiusChanged);
    
    // 渲染選項
    connect(m_showWireframeCheckBox, &QCheckBox::toggled, this, &MainWindow::onShowWireframeChanged);
    connect(m_showParticlesCheckBox, &QCheckBox::toggled, this, &MainWindow::onShowParticlesChanged);
    connect(m_showCollidersCheckBox, &QCheckBox::toggled, this, &MainWindow::onShowCollidersChanged);
    connect(m_resetCameraButton, &QPushButton::clicked, this, &MainWindow::onResetCameraClicked);
}

void MainWindow::initializeSimulation() {
    m_clothSimulation = std::make_shared<Physics::ClothSimulation>(15, 15, 0.2f);
    
    // 添加圓柱體碰撞體
    m_clothSimulation->addCylinder(QVector3D(0, -0.5f, 0), 1.0f, 2.0f);
    
    // 設定初始參數
    m_clothSimulation->setGravity(QVector3D(0, -9.8f, 0));
    m_clothSimulation->setWind(QVector3D(0, 0, 0));
    m_clothSimulation->setDamping(0.01f);
    m_clothSimulation->setUseOGC(true);
    m_clothSimulation->setOGCContactRadius(0.1f);
    
    qDebug() << "布料模擬初始化完成";
}

void MainWindow::onStartStopClicked() {
    m_isRunning = !m_isRunning;
    
    if (m_isRunning) {
        m_startStopButton->setText("停止");
        m_openglWidget->setAnimating(true);
        m_statusLabel->setText("狀態: 運行中");
        statusBar()->showMessage("模擬運行中...");
    } else {
        m_startStopButton->setText("開始");
        m_openglWidget->setAnimating(false);
        m_statusLabel->setText("狀態: 停止");
        statusBar()->showMessage("模擬已停止");
    }
}

void MainWindow::onResetClicked() {
    m_clothSimulation->reset();
    m_openglWidget->update();
    statusBar()->showMessage("模擬已重置");
}

void MainWindow::onStepClicked() {
    if (!m_isRunning) {
        m_clothSimulation->update(0.016f);
        m_openglWidget->update();
    }
}

void MainWindow::onClothSizeChanged() {
    if (!m_isRunning) {
        int width = m_clothWidthSpinBox->value();
        int height = m_clothHeightSpinBox->value();
        m_clothSimulation->initialize(width, height, 0.2f);
        m_openglWidget->update();
    }
}

void MainWindow::onGravityChanged() {
    float gravity = m_gravitySpinBox->value();
    m_clothSimulation->setGravity(QVector3D(0, gravity, 0));
}

void MainWindow::onWindChanged() {
    QVector3D wind(
        m_windXSpinBox->value(),
        m_windYSpinBox->value(),
        m_windZSpinBox->value()
    );
    m_clothSimulation->setWind(wind);
}

void MainWindow::onDampingChanged() {
    float damping = m_dampingSpinBox->value();
    m_clothSimulation->setDamping(damping);
}

void MainWindow::onOGCEnabledChanged(bool enabled) {
    m_clothSimulation->setUseOGC(enabled);
    m_contactRadiusSpinBox->setEnabled(enabled);
}

void MainWindow::onContactRadiusChanged() {
    float radius = m_contactRadiusSpinBox->value();
    m_clothSimulation->setOGCContactRadius(radius);
}

void MainWindow::onShowWireframeChanged(bool show) {
    m_openglWidget->setShowWireframe(show);
}

void MainWindow::onShowParticlesChanged(bool show) {
    m_openglWidget->setShowParticles(show);
}

void MainWindow::onShowCollidersChanged(bool show) {
    m_openglWidget->setShowColliders(show);
}

void MainWindow::onResetCameraClicked() {
    m_openglWidget->resetCamera();
}

void MainWindow::updateStatus() {
    if (m_clothSimulation) {
        m_particleCountLabel->setText(QString("粒子數: %1").arg(m_clothSimulation->getParticleCount()));
        m_constraintCountLabel->setText(QString("約束數: %1").arg(m_clothSimulation->getConstraintCount()));
        m_simulationTimeLabel->setText(QString("模擬時間: %1s").arg(m_clothSimulation->getSimulationTime(), 0, 'f', 2));
        
        // 簡單的 FPS 計算
        static int frameCount = 0;
        static QTime lastTime = QTime::currentTime();
        frameCount++;
        
        QTime currentTime = QTime::currentTime();
        int elapsed = lastTime.msecsTo(currentTime);
        if (elapsed >= 1000) {
            float fps = frameCount * 1000.0f / elapsed;
            m_fpsLabel->setText(QString("FPS: %1").arg(fps, 0, 'f', 1));
            frameCount = 0;
            lastTime = currentTime;
        }
    }
}

} // namespace UI

#include "MainWindow.moc"
