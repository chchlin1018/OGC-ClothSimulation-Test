# Vulkan 警告說明

在編譯過程中，您可能會看到以下警告信息：

```
-- Could NOT find WrapVulkanHeaders (missing: Vulkan_INCLUDE_DIR) 
-- Could NOT find WrapVulkanHeaders (missing: Vulkan_INCLUDE_DIR) 
```

## 這是正常的，不會影響編譯

這些警告是 Qt6 自動檢測系統功能時產生的，**不會影響項目的編譯和運行**。

### 為什麼會出現這些警告？

1. **Qt6 自動檢測**: Qt6 會自動檢測系統中可用的圖形 API，包括 OpenGL、Vulkan、Metal 等
2. **我們使用 OpenGL**: 本項目使用 OpenGL 進行 3D 渲染，不需要 Vulkan 支持
3. **macOS 默認不包含 Vulkan**: macOS 系統默認不安裝 Vulkan SDK

### 如何消除這些警告？

#### 方法 1: 使用項目提供的編譯腳本（推薦）
```bash
./build.sh
```
編譯腳本已經配置了適當的 CMake 參數來減少不必要的警告。

#### 方法 2: 手動配置 CMake
```bash
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake/Qt6 -DQT_FEATURE_vulkan=OFF
```

#### 方法 3: 安裝 Vulkan SDK（可選）
如果您想完全消除警告，可以安裝 Vulkan SDK：
```bash
# 使用 Homebrew
brew install vulkan-headers vulkan-loader

# 或從官網下載
# https://vulkan.lunarg.com/sdk/home#mac
```

**注意**: 安裝 Vulkan SDK 不是必需的，因為本項目不使用 Vulkan。

### 確認編譯成功

即使有 Vulkan 警告，只要看到以下信息就表示編譯成功：

```bash
-- Configuring done (1.0s)
-- Generating done (0.1s)
-- Build files have been generated successfully.
```

然後 `make` 命令能夠成功完成編譯。

### 運行時不受影響

這些編譯時的警告不會影響：
- 程序的正常運行
- OpenGL 渲染功能
- 布料物理模擬
- 用戶界面操作

### 其他常見的無害警告

在 macOS 上編譯 Qt6 項目時，您可能還會看到其他無害的警告：

```bash
-- Could NOT find WrapSystemHarfbuzz
-- Could NOT find WrapSystemFreetype
-- Could NOT find WrapSystemPNG
```

這些都是正常的，Qt6 會使用內建的版本。

## 總結

**Vulkan 警告是正常現象，不需要擔心。** 項目使用 OpenGL 進行渲染，所有功能都能正常工作。

如果您想要一個更清潔的編譯輸出，請使用項目提供的 `build.sh` 腳本，它已經配置了適當的參數來減少不必要的警告。
