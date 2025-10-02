cmake_minimum_required(VERSION 3.16)
project(OGC-ClothSimulation-Test VERSION 1.0.0 LANGUAGES CXX)

# 設定 C++ 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS 特定設定
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for OS X")
endif()

# 編譯選項
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 禁用不需要的 Qt6 功能以減少警告
set(QT_FEATURE_vulkan OFF CACHE BOOL "Disable Vulkan support")

# 尋找 Qt6 - 只包含我們需要的組件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets)

# 尋找 OpenGL
find_package(OpenGL REQUIRED)

# 包含目錄
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/physics
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)

# 收集源文件
file(GLOB_RECURSE PHYSICS_SOURCES "src/physics/*.cpp")
file(GLOB_RECURSE UI_SOURCES "src/ui/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.cpp")

# 收集頭文件
file(GLOB_RECURSE PHYSICS_HEADERS "include/physics/*.h")
file(GLOB_RECURSE UI_HEADERS "include/ui/*.h")
file(GLOB_RECURSE UTILS_HEADERS "include/utils/*.h")

# 主要可執行文件
add_executable(OGCClothSimulation
    src/main.cpp
    ${PHYSICS_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
    ${PHYSICS_HEADERS}
    ${UI_HEADERS}
    ${UTILS_HEADERS}
)

# 連結庫
target_link_libraries(OGCClothSimulation
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
)

# macOS 特定設定
if(APPLE)
    set_target_properties(OGCClothSimulation PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/assets/Info.plist.in
    )
endif()

# 啟用 Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 範例程序
add_subdirectory(examples)

# 安裝規則
install(TARGETS OGCClothSimulation
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# CPack 設定 (用於創建安裝包)
if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_FORMAT "UDBZ")
    set(CPACK_DMG_VOLUME_NAME "OGC Cloth Simulation")
    set(CPACK_SYSTEM_NAME "macOS")
endif()

include(CPack)
