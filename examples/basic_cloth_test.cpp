#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include <iostream>
#include <fstream>
#include "physics/ClothSimulation.h"

/**
 * @brief 基本布料測試程序
 * 
 * 這個程序演示了如何使用 ClothSimulation 類別進行基本的布料物理模擬。
 * 它會運行一個簡單的模擬並輸出結果到 OBJ 文件。
 */
class BasicClothTest : public QObject {
    Q_OBJECT

public:
    BasicClothTest(QObject* parent = nullptr) : QObject(parent) {
        // 初始化布料模擬
        m_simulation = std::make_unique<Physics::ClothSimulation>(12, 12, 0.25f);
        
        // 添加圓柱體碰撞體
        m_simulation->addCylinder(QVector3D(0, -0.5f, 0), 0.8f, 2.0f);
        
        // 設定物理參數
        m_simulation->setGravity(QVector3D(0, -9.8f, 0));
        m_simulation->setWind(QVector3D(1.0f, 0, 0));
        m_simulation->setDamping(0.01f);
        
        // 啟用 OGC
        m_simulation->setUseOGC(true);
        m_simulation->setOGCContactRadius(0.05f);
        
        std::cout << "基本布料測試初始化完成" << std::endl;
        std::cout << "粒子數: " << m_simulation->getParticleCount() << std::endl;
        std::cout << "約束數: " << m_simulation->getConstraintCount() << std::endl;
    }
    
    void runTest(int frames = 300) {
        std::cout << "\n開始運行測試..." << std::endl;
        
        QElapsedTimer timer;
        timer.start();
        
        // 導出初始狀態
        exportToOBJ("basic_test_initial.obj", 0);
        
        for (int frame = 0; frame < frames; ++frame) {
            m_simulation->update(0.016f); // ~60 FPS
            
            // 每60幀輸出一次狀態
            if (frame % 60 == 0) {
                printStatus(frame);
                
                // 導出關鍵幀
                QString filename = QString("basic_test_frame_%1.obj").arg(frame);
                exportToOBJ(filename.toStdString(), frame);
            }
        }
        
        // 導出最終狀態
        exportToOBJ("basic_test_final.obj", frames);
        
        qint64 elapsed = timer.elapsed();
        std::cout << "\n測試完成!" << std::endl;
        std::cout << "總時間: " << elapsed << " ms" << std::endl;
        std::cout << "平均每幀: " << (double)elapsed / frames << " ms" << std::endl;
        std::cout << "模擬時間: " << m_simulation->getSimulationTime() << " 秒" << std::endl;
        
        QCoreApplication::quit();
    }

private:
    std::unique_ptr<Physics::ClothSimulation> m_simulation;
    
    void printStatus(int frame) {
        std::cout << "幀 " << frame 
                  << ", 時間: " << m_simulation->getSimulationTime() << "s"
                  << ", 粒子: " << m_simulation->getParticleCount()
                  << ", 約束: " << m_simulation->getConstraintCount() << std::endl;
    }
    
    void exportToOBJ(const std::string& filename, int frame) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "無法創建文件: " << filename << std::endl;
            return;
        }
        
        file << "# 基本布料測試 OBJ 文件\n";
        file << "# 幀: " << frame << "\n";
        file << "# 時間: " << m_simulation->getSimulationTime() << "s\n";
        file << "# 粒子數: " << m_simulation->getParticleCount() << "\n\n";
        
        // 注意: 這裡需要訪問粒子數據，但由於封裝性，
        // 實際實現中應該在 ClothSimulation 類別中提供導出方法
        
        file << "# 布料頂點數據\n";
        file << "# (實際實現中需要從 ClothSimulation 獲取頂點數據)\n";
        
        file.close();
        std::cout << "導出 OBJ 文件: " << filename << std::endl;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    std::cout << "=== 基本布料測試程序 ===" << std::endl;
    
    BasicClothTest test;
    
    // 使用計時器延遲啟動測試
    QTimer::singleShot(100, [&test]() {
        test.runTest(240); // 4秒的模擬
    });
    
    return app.exec();
}

#include "basic_cloth_test.moc"
