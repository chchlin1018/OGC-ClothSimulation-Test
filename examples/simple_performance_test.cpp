#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "physics/ClothSimulation.h"

/**
 * @brief 簡化的性能測試程序
 * 
 * 比較 OGC 模型和基本碰撞模型的性能差異，避免 Qt 類型輸出問題。
 */
class SimplePerformanceTest : public QObject {
    Q_OBJECT

public:
    struct TestResult {
        std::string name;
        int totalFrames;
        double totalTime;
        double avgFrameTime;
        std::vector<double> frameTimes;
    };

    SimplePerformanceTest(QObject* parent = nullptr) : QObject(parent) {
        std::cout << "簡化性能測試程序初始化" << std::endl;
    }

public slots:
    void runTests() {
        std::cout << "\n=== 開始性能測試 ===" << std::endl;
        
        TestResult basic = runBasicCollisionTest();
        TestResult ogc = runOGCCollisionTest();
        
        compareResults(basic, ogc);
        
        // 退出應用程式
        QCoreApplication::quit();
    }

private:
    TestResult runBasicCollisionTest() {
        std::cout << "\n測試基本碰撞模型..." << std::endl;
        
        auto simulation = std::make_unique<Physics::ClothSimulation>(15, 15, 0.2f);
        simulation->addCylinder(QVector3D(0, -0.5f, 0), 1.0f, 2.0f);
        simulation->setGravity(QVector3D(0, -9.8f, 0));
        simulation->setUseOGC(false); // 使用基本碰撞
        
        TestResult result;
        result.name = "基本碰撞模型";
        result.totalFrames = 300;
        
        QElapsedTimer totalTimer;
        totalTimer.start();
        
        for (int frame = 0; frame < result.totalFrames; ++frame) {
            QElapsedTimer frameTimer;
            frameTimer.start();
            
            simulation->update(0.016f);
            
            qint64 frameTime = frameTimer.nsecsElapsed();
            result.frameTimes.push_back(frameTime / 1000000.0); // 轉換為毫秒
            
            if (frame % 60 == 0) {
                std::cout << "  幀 " << frame << ": " 
                          << result.frameTimes.back() << " ms" << std::endl;
            }
        }
        
        result.totalTime = totalTimer.elapsed();
        result.avgFrameTime = result.totalTime / double(result.totalFrames);
        
        std::cout << "基本碰撞模型測試完成" << std::endl;
        return result;
    }
    
    TestResult runOGCCollisionTest() {
        std::cout << "\n測試 OGC 碰撞模型..." << std::endl;
        
        auto simulation = std::make_unique<Physics::ClothSimulation>(15, 15, 0.2f);
        simulation->addCylinder(QVector3D(0, -0.5f, 0), 1.0f, 2.0f);
        simulation->setGravity(QVector3D(0, -9.8f, 0));
        simulation->setUseOGC(true); // 啟用 OGC
        simulation->setOGCContactRadius(0.05f);
        
        TestResult result;
        result.name = "OGC 碰撞模型";
        result.totalFrames = 300;
        
        QElapsedTimer totalTimer;
        totalTimer.start();
        
        for (int frame = 0; frame < result.totalFrames; ++frame) {
            QElapsedTimer frameTimer;
            frameTimer.start();
            
            simulation->update(0.016f);
            
            qint64 frameTime = frameTimer.nsecsElapsed();
            result.frameTimes.push_back(frameTime / 1000000.0); // 轉換為毫秒
            
            if (frame % 60 == 0) {
                std::cout << "  幀 " << frame << ": " 
                          << result.frameTimes.back() << " ms" << std::endl;
            }
        }
        
        result.totalTime = totalTimer.elapsed();
        result.avgFrameTime = result.totalTime / double(result.totalFrames);
        
        std::cout << "OGC 碰撞模型測試完成" << std::endl;
        return result;
    }
    
    void compareResults(const TestResult& basic, const TestResult& ogc) {
        std::cout << "\n=== 性能比較結果 ===" << std::endl;
        
        std::cout << "\n" << basic.name << ":" << std::endl;
        std::cout << "  總時間: " << basic.totalTime << " ms" << std::endl;
        std::cout << "  平均每幀: " << basic.avgFrameTime << " ms" << std::endl;
        
        std::cout << "\n" << ogc.name << ":" << std::endl;
        std::cout << "  總時間: " << ogc.totalTime << " ms" << std::endl;
        std::cout << "  平均每幀: " << ogc.avgFrameTime << " ms" << std::endl;
        
        double speedRatio = basic.avgFrameTime / ogc.avgFrameTime;
        std::cout << "\n性能比較:" << std::endl;
        std::cout << "  速度比率: " << speedRatio << "x" << std::endl;
        
        if (speedRatio > 1.0) {
            std::cout << "  OGC 模型比基本模型快 " 
                      << ((speedRatio - 1.0) * 100) << "%" << std::endl;
        } else {
            std::cout << "  OGC 模型比基本模型慢 " 
                      << ((1.0 - speedRatio) * 100) << "%" << std::endl;
        }
        
        // 計算標準差
        double basicStdDev = calculateStandardDeviation(basic.frameTimes);
        double ogcStdDev = calculateStandardDeviation(ogc.frameTimes);
        
        std::cout << "\n穩定性分析:" << std::endl;
        std::cout << "  基本模型標準差: " << basicStdDev << " ms" << std::endl;
        std::cout << "  OGC 模型標準差: " << ogcStdDev << " ms" << std::endl;
        
        // 保存結果到文件
        saveResults(basic, ogc);
    }
    
    void saveResults(const TestResult& basic, const TestResult& ogc) {
        std::ofstream file("simple_performance_results.csv");
        if (!file.is_open()) {
            std::cerr << "無法創建結果文件" << std::endl;
            return;
        }
        
        file << "Frame,BasicTime,OGCTime\n";
        
        size_t minSize = std::min(basic.frameTimes.size(), ogc.frameTimes.size());
        for (size_t i = 0; i < minSize; ++i) {
            file << i << "," << basic.frameTimes[i] << "," << ogc.frameTimes[i] << "\n";
        }
        
        file.close();
        std::cout << "\n結果已保存到 simple_performance_results.csv" << std::endl;
    }
    
    double calculateStandardDeviation(const std::vector<double>& values) {
        if (values.empty()) return 0.0;
        
        double mean = 0.0;
        for (double value : values) {
            mean += value;
        }
        mean /= values.size();
        
        double variance = 0.0;
        for (double value : values) {
            variance += (value - mean) * (value - mean);
        }
        variance /= values.size();
        
        return std::sqrt(variance);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    std::cout << "=== OGC 布料模擬簡化性能測試 ===" << std::endl;
    
    SimplePerformanceTest test;
    
    // 使用計時器延遲啟動測試
    QTimer::singleShot(100, [&test]() {
        test.runTests();
    });
    
    return app.exec();
}

#include "simple_performance_test.moc"
