# 簡化的範例程序 CMakeLists.txt
# 只編譯不依賴 OpenGLWidget 的範例

# 基本布料測試 (純物理模擬，無GUI)
add_executable(BasicClothTest
    basic_cloth_test.cpp
    ../src/physics/ClothSimulation.cpp
    ../src/physics/OGCContactModel.cpp
)

target_link_libraries(BasicClothTest
    Qt6::Core
    Qt6::Gui
    OpenGL::GL
)

target_include_directories(BasicClothTest PRIVATE
    ../include
)

# 簡化性能測試 (純物理模擬，無GUI，避免Qt類型輸出問題)
add_executable(SimplePerformanceTest
    simple_performance_test.cpp
    ../src/physics/ClothSimulation.cpp
    ../src/physics/OGCContactModel.cpp
)

target_link_libraries(SimplePerformanceTest
    Qt6::Core
    Qt6::Gui
    OpenGL::GL
)

target_include_directories(SimplePerformanceTest PRIVATE
    ../include
)

# 設定目標屬性
set_target_properties(BasicClothTest PROPERTIES
    CMAKE_AUTOMOC ON
)

set_target_properties(SimplePerformanceTest PROPERTIES
    CMAKE_AUTOMOC ON
)

# 注意：OpenGLRenderTest 暫時跳過，因為它依賴 OpenGLWidget
# 主程序 OGCClothSimulation 已經包含了完整的 OpenGL 渲染功能
