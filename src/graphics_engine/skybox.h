#ifndef SKYBOX_H
#define SKYBOX_H

#include "graphics_engine.h"
#include "object3d.h"

#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>
#include <QImage>

class GRAPHICS_ENGINE_EXPORT skybox : public WorldEngineObject
{
public:

    skybox();
    skybox(float width, const QString &img);

    void init(float width, const QString &img);

    virtual ~skybox();

    const QString& GetTexture() const;
    const QVector3D &getLocation() const;

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions*functions = nullptr, bool using_textures = true) override;

    void rotate(const QQuaternion &r) override;

    void translate(const QVector3D &t) override;

    void scale(float s) override;

    void SetGlobalTransform(const QMatrix4x4 &gt) override;

    float width() const;

protected:
    QVector<VertexData> init_vertexes(float width_div_2);
    void init_indexes();

private:
    object3D* cube;
    QString texture;

    QVector<GLuint> indexes;

    float m_width;
};

#endif // SKYBOX_H
