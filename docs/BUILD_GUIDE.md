# macOS 編譯指南

本文檔詳細說明如何在 macOS 系統上編譯和運行 OGC Cloth Simulation Test 項目。

## 系統要求

### 最低要求
- **macOS**: 10.15 (Catalina) 或更高版本
- **Xcode**: 12.0 或更高版本
- **CMake**: 3.16 或更高版本
- **Qt6**: 6.2 或更高版本

### 推薦配置
- **macOS**: 12.0 (Monterey) 或更高版本
- **Xcode**: 14.0 或更高版本
- **CMake**: 3.24 或更高版本
- **Qt6**: 6.5 或更高版本

## 安裝依賴

### 方法一：使用 Homebrew（推薦）

```bash
# 安裝 Homebrew（如果尚未安裝）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安裝 Qt6
brew install qt6

# 安裝 CMake
brew install cmake

# 安裝 Git（如果需要）
brew install git
```

### 方法二：手動安裝

#### 安裝 Qt6
1. 訪問 [Qt 官方網站](https://www.qt.io/download)
2. 下載 Qt Online Installer
3. 運行安裝程序並選擇以下組件：
   - Qt 6.5.x (最新版本)
   - Qt Creator
   - CMake
   - Ninja

#### 安裝 Xcode Command Line Tools
```bash
xcode-select --install
```

## 編譯項目

### 1. 克隆項目

```bash
git clone https://github.com/chchlin1018/OGC-ClothSimulation-Test.git
cd OGC-ClothSimulation-Test
```

### 2. 配置環境變量

如果使用 Homebrew 安裝的 Qt6：
```bash
export Qt6_DIR=/opt/homebrew/lib/cmake/Qt6
export PATH="/opt/homebrew/bin:$PATH"
```

如果手動安裝的 Qt6：
```bash
export Qt6_DIR=/Users/$(whoami)/Qt/6.5.x/macos/lib/cmake/Qt6
export PATH="/Users/$(whoami)/Qt/6.5.x/macos/bin:$PATH"
```

### 3. 創建編譯目錄

```bash
mkdir build
cd build
```

### 4. 配置項目

#### 使用 Homebrew Qt6
```bash
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6
```

#### 使用手動安裝的 Qt6
```bash
cmake .. -DCMAKE_PREFIX_PATH=/Users/$(whoami)/Qt/6.5.x/macos/lib/cmake/Qt6
```

#### 高級配置選項
```bash
# 指定編譯類型
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6

# 指定架構（Apple Silicon Mac）
cmake .. -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6

# 指定架構（Intel Mac）
cmake .. -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6

# 通用二進制（同時支持 Intel 和 Apple Silicon）
cmake .. -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6
```

### 5. 編譯

```bash
# 使用所有可用的 CPU 核心
make -j$(sysctl -n hw.ncpu)

# 或者使用 ninja（如果安裝了）
ninja
```

### 6. 運行程序

```bash
# 運行主程序
./OGCClothSimulation

# 運行範例程序
./examples/BasicClothTest
./examples/PerformanceTest
```

## 故障排除

### 常見問題

#### 1. Qt6 找不到
**錯誤信息**: `Could not find a package configuration file provided by "Qt6"`

**解決方案**:
```bash
# 檢查 Qt6 安裝路徑
find /opt/homebrew -name "Qt6Config.cmake" 2>/dev/null
find /Users/$(whoami)/Qt -name "Qt6Config.cmake" 2>/dev/null

# 使用正確的路徑重新配置
cmake .. -DCMAKE_PREFIX_PATH=<正確的Qt6路徑>
```

#### 2. OpenGL 相關錯誤
**錯誤信息**: OpenGL 函數未找到

**解決方案**:
```bash
# 確保 Xcode Command Line Tools 已安裝
xcode-select --install

# 檢查 OpenGL 框架
ls /System/Library/Frameworks/OpenGL.framework/
```

#### 3. 編譯器版本問題
**錯誤信息**: C++17 特性不支持

**解決方案**:
```bash
# 檢查編譯器版本
clang++ --version

# 確保使用正確的編譯器
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
```

#### 4. 權限問題
**錯誤信息**: Permission denied

**解決方案**:
```bash
# 修復權限
chmod +x ./OGCClothSimulation
chmod +x ./examples/*
```

### 調試技巧

#### 1. 詳細編譯輸出
```bash
make VERBOSE=1
```

#### 2. CMake 調試
```bash
cmake .. --debug-output -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6
```

#### 3. 檢查依賴
```bash
# 檢查可執行文件的依賴
otool -L ./OGCClothSimulation
```

## 性能優化

### 編譯優化
```bash
# Release 模式編譯
cmake .. -DCMAKE_BUILD_TYPE=Release

# 啟用 LTO（鏈接時優化）
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
```

### 運行時優化
```bash
# 設定 OpenGL 環境變量
export MESA_GL_VERSION_OVERRIDE=3.3
export MESA_GLSL_VERSION_OVERRIDE=330
```

## 開發環境設定

### 使用 Qt Creator
1. 打開 Qt Creator
2. 選擇 "Open Project"
3. 選擇項目根目錄的 `CMakeLists.txt`
4. 配置編譯套件（Kit）
5. 點擊 "Configure Project"

### 使用 Xcode
```bash
# 生成 Xcode 項目
cmake .. -G Xcode -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6

# 打開 Xcode 項目
open OGC-ClothSimulation-Test.xcodeproj
```

### 使用 Visual Studio Code
1. 安裝 CMake Tools 擴展
2. 打開項目文件夾
3. 按 `Cmd+Shift+P` 並選擇 "CMake: Configure"
4. 選擇適當的編譯套件

## 打包和分發

### 創建 macOS 應用程序包
```bash
# 編譯 Release 版本
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)

# 創建應用程序包
make install

# 使用 macdeployqt 打包依賴
/opt/homebrew/bin/macdeployqt OGCClothSimulation.app
```

### 創建 DMG 安裝包
```bash
# 使用 CPack
cpack -G DragNDrop
```

## 更新和維護

### 更新依賴
```bash
# 更新 Homebrew 包
brew update && brew upgrade qt6

# 重新編譯項目
cd build
make clean
cmake ..
make -j$(sysctl -n hw.ncpu)
```

### 清理編譯文件
```bash
# 清理編譯輸出
make clean

# 完全重新編譯
rm -rf build
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

## 支持和幫助

如果遇到編譯問題，請：

1. 檢查系統要求是否滿足
2. 確認所有依賴都已正確安裝
3. 查看 [GitHub Issues](https://github.com/chchlin1018/OGC-ClothSimulation-Test/issues)
4. 提交新的 Issue 並包含：
   - macOS 版本
   - Xcode 版本
   - Qt6 版本
   - 完整的錯誤信息
   - CMake 配置輸出

---

**注意**: 本指南基於 macOS 12.0+ 和 Qt6.5+ 編寫。較舊的版本可能需要額外的配置步驟。
