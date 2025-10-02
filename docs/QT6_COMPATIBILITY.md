# Qt6 兼容性修復指南

本文檔說明了在 Qt6 中遇到的兼容性問題及其解決方案。

## 已修復的問題

### 1. QVector3D 頭文件找不到

**問題描述**:
```cpp
fatal error: 'QVector3D' file not found
```

**原因**:
在 Qt6 中，`QVector3D` 和相關的數學類別被移動到了 `Qt6::Gui` 模塊中，而不再是 `Qt6::Core` 的一部分。

**解決方案**:
1. 在 CMakeLists.txt 中添加 `Qt6::Gui` 模塊：
```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
target_link_libraries(target_name Qt6::Gui)
```

2. 確保包含了正確的頭文件：
```cpp
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
```

### 2. OGCContactModel 重複定義

**問題描述**:
```cpp
error: redefinition of 'OGCContactModel'
```

**原因**:
`OGCContactModel` 類別在 `ClothSimulation.h` 和 `OGCContactModel.h` 中都有定義，導致重複定義錯誤。

**解決方案**:
1. 在 `ClothSimulation.h` 中只使用前向聲明：
```cpp
// OGC 接觸模型前向聲明
class OGCContactModel;
```

2. 在 `ClothSimulation.cpp` 中包含完整的頭文件：
```cpp
#include "physics/OGCContactModel.h"
```

## Qt6 與 Qt5 的主要差異

### 模塊重組
- **Qt5**: `QVector3D` 在 `QtGui` 中，但通常通過 `QtWidgets` 自動包含
- **Qt6**: 需要顯式鏈接 `Qt6::Gui` 模塊

### 頭文件包含
```cpp
// Qt6 中需要顯式包含
#include <QVector3D>    // 需要 Qt6::Gui
#include <QVector2D>    // 需要 Qt6::Gui
#include <QMatrix4x4>   // 需要 Qt6::Gui
#include <QOpenGLWidget> // 需要 Qt6::OpenGLWidgets
```

### CMake 配置
```cmake
# Qt6 需要更明確的模塊聲明
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui          # 新增：數學類別
    Widgets 
    OpenGL 
    OpenGLWidgets
)

target_link_libraries(target_name
    Qt6::Core
    Qt6::Gui     # 新增：必須鏈接
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
```

## 其他常見的 Qt6 兼容性問題

### 1. QString 和 QStringList
```cpp
// Qt5
#include <QString>  // 通常自動包含

// Qt6
#include <QString>  // 需要顯式包含
#include <QStringList>
```

### 2. QOpenGL 相關
```cpp
// Qt6 中 OpenGL 功能分離更明確
#include <QOpenGLWidget>      // Qt6::OpenGLWidgets
#include <QOpenGLFunctions>   // Qt6::OpenGL
```

### 3. 信號和槽連接
```cpp
// Qt6 推薦使用新語法
connect(sender, &SenderClass::signal, 
        receiver, &ReceiverClass::slot);
```

## 檢查 Qt6 安裝

### 驗證 Qt6 模塊
```bash
# 檢查 Qt6 安裝
find /opt/homebrew -name "Qt6*Config.cmake" 2>/dev/null

# 檢查特定模塊
ls /opt/homebrew/lib/cmake/Qt6Gui/
ls /opt/homebrew/lib/cmake/Qt6OpenGLWidgets/
```

### 測試編譯
```bash
# 清理重建
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6
make -j$(sysctl -n hw.ncpu)
```

## 最佳實踐

### 1. 明確的模塊依賴
始終明確聲明需要的 Qt6 模塊，不要依賴隱式包含。

### 2. 前向聲明
在頭文件中使用前向聲明，在實現文件中包含完整定義。

### 3. 命名空間
使用適當的命名空間來避免名稱衝突。

### 4. 版本檢查
```cmake
# 確保 Qt6 版本
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
```

## 故障排除

### 如果仍然遇到 QVector3D 問題：

1. **檢查 Qt6 安裝**：
```bash
brew list qt6
```

2. **重新安裝 Qt6**：
```bash
brew reinstall qt6
```

3. **清理 CMake 緩存**：
```bash
rm -rf build CMakeCache.txt
```

4. **檢查環境變量**：
```bash
echo $Qt6_DIR
echo $CMAKE_PREFIX_PATH
```

### 如果遇到鏈接錯誤：

1. **檢查庫文件**：
```bash
ls /opt/homebrew/lib/libQt6Gui*
```

2. **檢查 CMake 輸出**：
```bash
cmake .. --debug-output
```

## 更新日誌

- **v1.0.1**: 修復 Qt6::Gui 模塊缺失問題
- **v1.0.2**: 解決 OGCContactModel 重複定義
- **v1.0.3**: 完善 CMake 配置和文檔

---

**注意**: 這些修復確保了項目與 Qt6.2+ 的完全兼容性。如果使用較舊的 Qt6 版本，可能需要額外的調整。
