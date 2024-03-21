QT       += core gui
QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    menu.cpp \
    oglwidget.cpp

HEADERS += \
    creators.h \
    graphics_pipeline.h \
    menu.h \
    oglwidget.h \
    serializer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -lopengl32

RESOURCES += \
    backgrounds.qrc \
    models_b.qrc \
    shaders.qrc \
    textures.qrc

FORMS += \
    menu.ui

unix|win32: LIBS += -L$$PWD/../build-graphics_engine-Desktop_Qt_6_2_4_MinGW_64_bit-Debug/debug/ -lgraphics_engine

INCLUDEPATH += $$PWD/../graphics_engine
DEPENDPATH += $$PWD/../graphics_engine
