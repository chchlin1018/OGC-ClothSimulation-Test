#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 設定應用程式資訊
    app.setApplicationName("OGC Cloth Simulation Test");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Physics Simulation Lab");
    
    // 設定 OpenGL 格式
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4); // 4x MSAA
    QSurfaceFormat::setDefaultFormat(format);
    
    qDebug() << "應用程式啟動";
    qDebug() << "Qt 版本:" << QT_VERSION_STR;
    
    // 創建並顯示主窗口
    UI::MainWindow window;
    window.show();
    
    qDebug() << "主窗口已顯示";
    
    return app.exec();
}
