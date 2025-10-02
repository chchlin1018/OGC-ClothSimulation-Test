# 範例程序

# 基本布料測試
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
    ../include/physics
)

# 性能測試
add_executable(PerformanceTest
    performance_test.cpp
    ../src/physics/ClothSimulation.cpp
    ../src/physics/OGCContactModel.cpp
)

target_link_libraries(PerformanceTest
    Qt6::Core
    Qt6::Gui
    OpenGL::GL
)

target_include_directories(PerformanceTest PRIVATE
    ../include
    ../include/physics
)

# OpenGL 渲染測試
add_executable(OpenGLRenderTest
    opengl_render_test.cpp
    ../src/physics/ClothSimulation.cpp
    ../src/physics/OGCContactModel.cpp
    ../src/ui/OpenGLWidget.cpp
)

target_link_libraries(OpenGLRenderTest
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
)

target_include_directories(OpenGLRenderTest PRIVATE
    ../include
    ../include/physics
    ../include/ui
)

set_target_properties(BasicClothTest PerformanceTest OpenGLRenderTest PROPERTIES
    CMAKE_AUTOMOC ON
)
