QT += gui
QT += widgets
QT += openglwidgets

TEMPLATE = lib
DEFINES += GRAPHICS_ENGINE_LIBRARY

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    dimensional.cpp \
    graphics_engine.cpp \
    group.cpp \
    material.cpp \
    materiallibrary.cpp \
    object3d.cpp \
    object3dbuilder.cpp \
    skybox.cpp

HEADERS += \
    camera.h \
    dimensional.h \
    graphics_engine_global.h \
    graphics_engine.h \
    group.h \
    material.h \
    materiallibrary.h \
    object3d.h \
    object3dbuilder.h \
    skybox.h \
    tools.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

LIBS += -lopengl32

STATECHARTS +=

DISTFILES += \
    w.qmodel
