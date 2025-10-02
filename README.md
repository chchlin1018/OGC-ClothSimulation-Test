# OGC Cloth Simulation Test

一個基於Qt6和OpenGL的布料物理模擬系統，實現了OGC（Offset Geometry Contact）接觸模型。

## 功能特點

### 🎯 核心功能
- **布料物理模擬**: 基於粒子系統的真實布料物理
- **OGC接觸模型**: 先進的偏移幾何接觸算法
- **實時渲染**: 基於OpenGL的3D可視化
- **交互式界面**: 完整的Qt6用戶界面

### 🔬 物理特性
- Verlet積分器用於粒子更新
- 基於位置的約束滿足（PBCS）
- 多層約束系統（結構、對角線、彎曲）
- 圓柱體-粒子碰撞檢測
- 動態風力和重力模擬

### 🎨 視覺效果
- 實時3D渲染
- 線框/實體渲染模式
- 粒子和約束可視化
- 碰撞體顯示
- 相機控制（旋轉、縮放）

## 系統要求

### macOS
- macOS 10.15 (Catalina) 或更高版本
- Xcode 12 或更高版本
- Qt6.2 或更高版本
- CMake 3.16 或更高版本

### 依賴庫
- Qt6 Core, Widgets, OpenGL, OpenGLWidgets
- OpenGL 3.3 或更高版本

## 編譯安裝

### 1. 安裝依賴

#### 使用 Homebrew (推薦)
```bash
# 安裝 Qt6
brew install qt6

# 安裝 CMake
brew install cmake
```

#### 手動安裝
- 從 [Qt官網](https://www.qt.io/download) 下載並安裝Qt6
- 從 [CMake官網](https://cmake.org/download/) 下載並安裝CMake

### 2. 編譯項目

#### 方法一：使用自動編譯腳本（推薦）
```bash
# 克隆項目
git clone https://github.com/chchlin1018/OGC-ClothSimulation-Test.git
cd OGC-ClothSimulation-Test

# 運行自動編譯腳本
./build.sh
```

#### 方法二：手動編譯
```bash
# 克隆項目
git clone https://github.com/chchlin1018/OGC-ClothSimulation-Test.git
cd OGC-ClothSimulation-Test

# 創建編譯目錄
mkdir build && cd build

# 配置項目
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6

# 編譯
make -j$(sysctl -n hw.ncpu)
```

**注意**: 編譯過程中可能會出現 Vulkan 相關警告，這是正常的，不會影響編譯。詳見 [Vulkan 警告說明](docs/VULKAN_WARNING.md)。

### 3. 運行程序

```bash
# 運行主程序
./OGCClothSimulation

# 運行範例程序
./examples/BasicClothTest
./examples/PerformanceTest
./examples/OpenGLRenderTest
```

## 使用說明

### 主程序界面

#### 模擬控制
- **開始/停止**: 控制模擬的運行狀態
- **重置**: 重置布料到初始狀態
- **單步**: 執行單個時間步長

#### 場景參數
- **布料尺寸**: 調整布料網格的寬度和高度
- **重力**: 設定重力加速度
- **風力**: 設定三個方向的風力
- **阻尼**: 調整系統阻尼係數

#### OGC參數
- **啟用OGC**: 開啟/關閉OGC接觸模型
- **接觸半徑**: 調整OGC模型的接觸半徑

#### 渲染選項
- **顯示線框**: 切換線框渲染模式
- **顯示粒子**: 顯示/隱藏粒子點
- **顯示碰撞體**: 顯示/隱藏碰撞體
- **重置相機**: 重置相機到默認視角

### 相機控制
- **滑鼠拖拽**: 旋轉視角
- **滾輪**: 縮放視圖
- **重置按鈕**: 恢復默認視角

## 範例程序

### BasicClothTest
基本的布料模擬測試，演示核心功能：
```bash
./examples/BasicClothTest
```

### PerformanceTest
性能測試程序，比較OGC模型和基本碰撞模型：
```bash
./examples/PerformanceTest
```

### OpenGLRenderTest
OpenGL渲染測試，展示視覺效果：
```bash
./examples/OpenGLRenderTest
```

## 項目結構

```
OGC-ClothSimulation-Test/
├── src/                    # 源代碼
│   ├── physics/           # 物理模擬
│   ├── ui/                # 用戶界面
│   ├── utils/             # 工具類
│   └── main.cpp           # 主程序入口
├── include/               # 頭文件
│   ├── physics/
│   ├── ui/
│   └── utils/
├── examples/              # 範例程序
├── docs/                  # 文檔
├── assets/                # 資源文件
├── build/                 # 編譯輸出
├── CMakeLists.txt         # CMake配置
└── README.md              # 本文件
```

## 技術細節

### 物理引擎
- **積分器**: Verlet積分，提供數值穩定性
- **約束求解**: 迭代式約束滿足，確保布料結構
- **碰撞檢測**: 高效的幾何碰撞算法
- **OGC模型**: 偏移幾何接觸，提供更真實的接觸響應

### 渲染系統
- **OpenGL版本**: 3.3 Core Profile
- **渲染管線**: 立即模式渲染（為了簡化）
- **抗鋸齒**: 4x MSAA
- **深度測試**: 啟用Z-buffer

### 性能優化
- **空間分割**: 用於碰撞檢測優化
- **約束緩存**: 減少重複計算
- **渲染批處理**: 減少OpenGL調用

## 開發指南

### 添加新功能
1. 在相應的命名空間中添加類別
2. 更新CMakeLists.txt
3. 添加對應的測試程序
4. 更新文檔

### 調試技巧
- 使用Qt Creator進行調試
- 啟用OpenGL調試上下文
- 檢查CMake配置輸出

## 已知問題

1. **macOS Monterey兼容性**: 某些OpenGL功能可能需要額外配置
2. **高DPI顯示**: 界面縮放可能需要調整
3. **性能**: 大型布料網格可能影響實時性能

## 貢獻指南

1. Fork 本項目
2. 創建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 開啟 Pull Request

## 許可證

本項目採用 MIT 許可證 - 詳見 [LICENSE](LICENSE) 文件。

## 致謝

- Qt框架提供的優秀GUI和OpenGL支持
- 物理模擬算法參考了相關學術論文
- OGC接觸模型基於最新的接觸力學研究

## 聯繫方式

- 項目主頁: https://github.com/yourusername/OGC-ClothSimulation-Test
- 問題報告: https://github.com/yourusername/OGC-ClothSimulation-Test/issues
- 電子郵件: your.email@example.com

---

**注意**: 這是一個實驗性項目，主要用於研究和教育目的。在生產環境中使用前請進行充分測試。
