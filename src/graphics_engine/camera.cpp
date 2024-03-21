#include "camera.h"
#include <QOpenGLShaderProgram>

camera::camera() : m_scale(1.0f)
{
    m_GlobalTransform.setToIdentity();
}

void camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool using_textures)
{
    program->setUniformValue("u_viewMatrix", m_ViewMatrix);

    Q_UNUSED(functions); Q_UNUSED(using_textures);
}

void camera::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
    updateViewMatrix();
}

void camera::translate(const QVector3D &t)
{
    m_translate += t;
    updateViewMatrix();
}

void camera::scale(float s)
{
    m_scale *= s;
    updateViewMatrix();
}

void camera::SetGlobalTransform(const QMatrix4x4 &gt)
{
    m_GlobalTransform = gt;
    updateViewMatrix();
}

void camera::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;

    updateViewMatrix();
}

void camera::rotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateX * m_rotateY;

    updateViewMatrix();
}

const QMatrix4x4 &camera::getViewMatrix() const
{
    return m_ViewMatrix;
}

void camera::reset()
{
    m_scale = 1.0f;
    m_translate = QVector3D(0.0f, 0.0f, -5.0f);
    m_rotate = QQuaternion();

    updateViewMatrix();
}

const QVector3D &camera::getLocation() const
{
    return m_translate;
}


void camera::updateViewMatrix()
{
    m_ViewMatrix.setToIdentity();
    m_ViewMatrix.translate(m_translate);
    m_ViewMatrix.rotate(m_rotate);
    m_ViewMatrix.scale(m_scale);
    m_ViewMatrix = m_ViewMatrix * m_GlobalTransform.inverted();
}
