#ifndef OBJECT3D_H
#define OBJECT3D_H

#include<QOpenGLBuffer>
#include<QMatrix4x4>
#include<QVector2D>

#include "tools.h"

class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLFunctions;
class material;

#include "graphics_engine.h"
#include "object3dbuilder.h"

using tools::VertexData;

class GRAPHICS_ENGINE_EXPORT object3D : public WorldEngineObject
{
public:
    object3D();

    object3D(const QVector<VertexData> &vert, const QVector<GLuint> &ind, material* mtl);

    virtual ~object3D();

    void init(const QVector<VertexData> &vert, const QVector<GLuint> &ind, material* mtl);

    const QVector3D &GetLocation() const;

    const QQuaternion& GetRotation() const;

    float GetScale() const;

    material* GetMaterial();

    void reset_material();

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions, bool using_textures = true) override;

    void rotate(const QQuaternion &r) override;

    void translate(const QVector3D &t) override;

    void scale(float s) override;

    void SetGlobalTransform(const QMatrix4x4 &gt) override;

protected:
    void free();

    void SetMapFilters(QOpenGLTexture *map);

    void AllocateVertexBuffer(const QVector<VertexData> &vertexes);
    void AllocateIndexBuffer(const QVector<GLuint> &indexes);

private:
    QOpenGLBuffer m_VertexBuffer;
    QOpenGLBuffer m_IndexBuffer;

    QOpenGLTexture* m_DiffuseMap;
    QOpenGLTexture* m_NormalMap;

    QQuaternion m_Rotate;

    QVector3D m_Translate;

    float m_Scale;

    QMatrix4x4 m_GlobalTransform;

    material m_Material;
    material basic_material;
};

#endif // OBJECT3D_H
