#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QStatusBar>
#include <QTimer>
#include <memory>

namespace Physics {
class ClothSimulation;
}

namespace UI {
class OpenGLWidget;

/**
 * @brief 主窗口類別
 * 
 * 提供布料模擬的用戶界面，包括控制面板和3D視圖。
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // 模擬控制
    void onStartStopClicked();
    void onResetClicked();
    void onStepClicked();
    
    // 場景控制
    void onClothSizeChanged();
    void onGravityChanged();
    void onWindChanged();
    void onDampingChanged();
    
    // OGC 控制
    void onOGCEnabledChanged(bool enabled);
    void onContactRadiusChanged();
    
    // 渲染控制
    void onShowWireframeChanged(bool show);
    void onShowParticlesChanged(bool show);
    void onShowCollidersChanged(bool show);
    
    // 相機控制
    void onResetCameraClicked();
    
    // 狀態更新
    void updateStatus();

private:
    // UI 組件
    QWidget* m_centralWidget;
    QHBoxLayout* m_mainLayout;
    
    // 3D 視圖
    OpenGLWidget* m_openglWidget;
    
    // 控制面板
    QWidget* m_controlPanel;
    QVBoxLayout* m_controlLayout;
    
    // 模擬控制組
    QGroupBox* m_simulationGroup;
    QPushButton* m_startStopButton;
    QPushButton* m_resetButton;
    QPushButton* m_stepButton;
    QLabel* m_statusLabel;
    
    // 場景參數組
    QGroupBox* m_sceneGroup;
    QSpinBox* m_clothWidthSpinBox;
    QSpinBox* m_clothHeightSpinBox;
    QDoubleSpinBox* m_gravitySpinBox;
    QDoubleSpinBox* m_windXSpinBox;
    QDoubleSpinBox* m_windYSpinBox;
    QDoubleSpinBox* m_windZSpinBox;
    QDoubleSpinBox* m_dampingSpinBox;
    
    // OGC 參數組
    QGroupBox* m_ogcGroup;
    QCheckBox* m_ogcEnabledCheckBox;
    QDoubleSpinBox* m_contactRadiusSpinBox;
    
    // 渲染選項組
    QGroupBox* m_renderGroup;
    QCheckBox* m_showWireframeCheckBox;
    QCheckBox* m_showParticlesCheckBox;
    QCheckBox* m_showCollidersCheckBox;
    QPushButton* m_resetCameraButton;
    
    // 統計資訊組
    QGroupBox* m_statsGroup;
    QLabel* m_particleCountLabel;
    QLabel* m_constraintCountLabel;
    QLabel* m_simulationTimeLabel;
    QLabel* m_fpsLabel;
    
    // 布料模擬
    std::shared_ptr<Physics::ClothSimulation> m_clothSimulation;
    
    // 狀態
    bool m_isRunning;
    QTimer* m_statusTimer;
    
    // 私有方法
    void setupUI();
    void setupSimulationGroup();
    void setupSceneGroup();
    void setupOGCGroup();
    void setupRenderGroup();
    void setupStatsGroup();
    void connectSignals();
    void initializeSimulation();
    void updateSimulationParameters();
};

} // namespace UI
