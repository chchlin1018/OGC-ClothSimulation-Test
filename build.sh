#!/bin/bash

# OGC Cloth Simulation Test - macOS 編譯腳本
# 這個腳本會自動檢測環境並編譯項目

set -e  # 遇到錯誤時退出

echo "=== OGC Cloth Simulation Test - macOS 編譯腳本 ==="

# 檢查是否在項目根目錄
if [ ! -f "CMakeLists.txt" ]; then
    echo "錯誤: 請在項目根目錄運行此腳本"
    exit 1
fi

# 檢查必要工具
echo "檢查編譯環境..."

if ! command -v cmake &> /dev/null; then
    echo "錯誤: CMake 未安裝"
    echo "請運行: brew install cmake"
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "錯誤: Make 未安裝"
    echo "請安裝 Xcode Command Line Tools: xcode-select --install"
    exit 1
fi

# 檢測 Qt6 安裝路徑
QT6_PATH=""
if [ -d "/opt/homebrew/lib/cmake/Qt6" ]; then
    QT6_PATH="/opt/homebrew/lib/cmake/Qt6"
    echo "✓ 找到 Qt6 (Homebrew): $QT6_PATH"
elif [ -d "/usr/local/lib/cmake/Qt6" ]; then
    QT6_PATH="/usr/local/lib/cmake/Qt6"
    echo "✓ 找到 Qt6 (Homebrew Intel): $QT6_PATH"
elif [ -d "/Users/$(whoami)/Qt" ]; then
    # 查找最新版本的 Qt6
    QT6_VERSION=$(find "/Users/$(whoami)/Qt" -name "6.*" -type d | sort -V | tail -n1)
    if [ -n "$QT6_VERSION" ]; then
        QT6_PATH="$QT6_VERSION/macos/lib/cmake/Qt6"
        echo "✓ 找到 Qt6 (手動安裝): $QT6_PATH"
    fi
fi

if [ -z "$QT6_PATH" ]; then
    echo "錯誤: 找不到 Qt6 安裝"
    echo "請安裝 Qt6:"
    echo "  方法1: brew install qt6"
    echo "  方法2: 從 https://www.qt.io/download 下載"
    exit 1
fi

# 檢查 Qt6 路徑是否有效
if [ ! -f "$QT6_PATH/Qt6Config.cmake" ]; then
    echo "錯誤: Qt6 配置文件不存在: $QT6_PATH/Qt6Config.cmake"
    exit 1
fi

# 創建並進入編譯目錄
echo "準備編譯目錄..."
if [ -d "build" ]; then
    echo "清理舊的編譯文件..."
    rm -rf build
fi
mkdir build
cd build

# 檢測系統架構
ARCH=$(uname -m)
if [ "$ARCH" = "arm64" ]; then
    echo "檢測到 Apple Silicon Mac"
    CMAKE_ARCH="arm64"
elif [ "$ARCH" = "x86_64" ]; then
    echo "檢測到 Intel Mac"
    CMAKE_ARCH="x86_64"
else
    echo "警告: 未知架構 $ARCH，使用默認設定"
    CMAKE_ARCH=""
fi

# 配置 CMake
echo "配置項目..."
CMAKE_ARGS=(
    "-DCMAKE_PREFIX_PATH=$QT6_PATH"
    "-DCMAKE_BUILD_TYPE=Release"
    "-DQT_FEATURE_vulkan=OFF"
)

if [ -n "$CMAKE_ARCH" ]; then
    CMAKE_ARGS+=("-DCMAKE_OSX_ARCHITECTURES=$CMAKE_ARCH")
fi

echo "CMake 參數: ${CMAKE_ARGS[*]}"
cmake .. "${CMAKE_ARGS[@]}"

# 編譯
echo "開始編譯..."
CPU_COUNT=$(sysctl -n hw.ncpu)
echo "使用 $CPU_COUNT 個 CPU 核心進行編譯"
make -j$CPU_COUNT

# 檢查編譯結果
echo "檢查編譯結果..."
EXECUTABLES=(
    "OGCClothSimulation"
    "examples/BasicClothTest"
    "examples/SimplePerformanceTest"
)

ALL_SUCCESS=true
for exe in "${EXECUTABLES[@]}"; do
    if [ -f "$exe" ]; then
        echo "✓ $exe"
    else
        echo "✗ $exe (編譯失敗)"
        ALL_SUCCESS=false
    fi
done

if [ "$ALL_SUCCESS" = true ]; then
    echo ""
    echo "🎉 編譯成功！"
    echo ""
    echo "運行程序:"
    echo "  主程序:           ./OGCClothSimulation"
    echo "  基本測試:         ./examples/BasicClothTest"
    echo "  簡化性能測試:     ./examples/SimplePerformanceTest"
    echo ""
    echo "提示: 使用 './OGCClothSimulation' 啟動主程序"
    echo "      主程序包含完整的 GUI 界面和 3D 可視化功能"
else
    echo ""
    echo "❌ 編譯失敗，請檢查錯誤信息"
    exit 1
fi
