//#include "oglwidget.h"
#include <QApplication>
#include <iostream>
#include "menu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QSurfaceFormat sformat;
//    sformat.setSamples(16);
//    sformat.setDepthBufferSize(24);
//    QSurfaceFormat::setDefaultFormat(sformat);

//    OGLWidget oglw;
//    oglw.show();

    menu m;
    m.show();

    return a.exec();
}
