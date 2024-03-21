#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include "graphics_engine_global.h"

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class GRAPHICS_ENGINE_EXPORT WorldEngineObject
{
public:
    virtual void rotate(const QQuaternion &r) = 0;

    virtual void translate(const QVector3D &t) = 0;

    virtual void scale(float s) = 0;

    //метод, который выставляет локальную систему координат в пределах глобальной
    virtual void SetGlobalTransform(const QMatrix4x4 &q) = 0; //задаёт позицию и поворот в локальной СК в пределах родительской для неё глобальной СК

    virtual void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true) = 0;

    virtual ~WorldEngineObject() = default;
};
#endif // GRAPHICS_ENGINE_H
