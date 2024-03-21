#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_engine.h"

class GRAPHICS_ENGINE_EXPORT camera : public WorldEngineObject
{
public:

    camera();

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions*functions = nullptr, bool using_textures = true) override;

    void rotate(const QQuaternion &r) override;

    void translate(const QVector3D &t) override;

    void scale(float s) override;

    void SetGlobalTransform(const QMatrix4x4 &gt) override;

    void rotateX(const QQuaternion &r);

    void rotateY(const QQuaternion &r);

    const QVector3D &getLocation() const;

    const QMatrix4x4 &getViewMatrix() const;

    void reset();

    virtual ~camera() = default;

protected:
    void updateViewMatrix();

private:
    QQuaternion m_rotate;

    QQuaternion m_rotateX;
    QQuaternion m_rotateY;

    QVector3D m_translate;

    float m_scale;

    QMatrix4x4 m_GlobalTransform;

    QMatrix4x4 m_ViewMatrix;
};
#endif // CAMERA_H
