cmake_minimum_required(VERSION 3.16)
project(OGC-ClothSimulation-Test VERSION 1.0.0 LANGUAGES CXX)

# 設定 C++ 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS 特定設定
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
    # 只針對當前架構編譯以避免問題
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for OS X")
    else()
        set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architectures for OS X")
    endif()
endif()

# 編譯選項
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 尋找 Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

# 尋找 OpenGL
find_package(OpenGL REQUIRED)

# 啟用 Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含目錄
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 明確列出所有源文件
set(SOURCES
    src/main.cpp
    src/physics/ClothSimulation.cpp
    src/physics/OGCContactModel.cpp
    src/ui/MainWindow.cpp
    src/ui/OpenGLWidget.cpp
)

# 明確列出所有頭文件
set(HEADERS
    include/physics/ClothSimulation.h
    include/physics/OGCContactModel.h
    include/ui/MainWindow.h
    include/ui/OpenGLWidget.h
)

# 主要可執行文件
add_executable(OGCClothSimulation ${SOURCES} ${HEADERS})

# 設定目標屬性
set_target_properties(OGCClothSimulation PROPERTIES
    CMAKE_AUTOMOC ON
    CMAKE_AUTOUIC ON
    CMAKE_AUTORCC ON
)

# 連結庫
target_link_libraries(OGCClothSimulation
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
)

# macOS Bundle 設定
if(APPLE)
    set_target_properties(OGCClothSimulation PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "OGC Cloth Simulation"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.physicslab.ogc-cloth-simulation"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    )
endif()

# 範例程序（簡化版本）
add_subdirectory(examples)
