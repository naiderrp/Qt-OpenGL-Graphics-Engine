#include "object3d.h"
#include "material.h"

#include<QOpenGLTexture>
#include<QOpenGLShaderProgram>
#include<QOpenGLFunctions>

object3D::object3D()
    : m_VertexBuffer(QOpenGLBuffer::VertexBuffer), m_IndexBuffer(QOpenGLBuffer::IndexBuffer),
    m_DiffuseMap(0), m_NormalMap(0), m_Scale(1.0f)
{
}

object3D::object3D(const QVector<VertexData> &vert, const QVector<GLuint> &ind, material *mtl)
    : object3D()
{
    init(vert, ind, mtl);
}

object3D::~object3D()
{
    free();
}

void object3D::free()
{
    if (m_VertexBuffer.isCreated())
        m_VertexBuffer.destroy();

    if (m_IndexBuffer.isCreated())
        m_IndexBuffer.destroy();

    if (m_DiffuseMap != 0)
        if (m_DiffuseMap->isCreated())
            delete m_DiffuseMap;

    if (m_NormalMap != 0)
        if (m_NormalMap->isCreated())
            delete m_NormalMap;
}

void object3D::init(const QVector<VertexData> &vertexes, const QVector<GLuint> &indexes, material *mtl)
{
    free();

    AllocateVertexBuffer(vertexes);
    AllocateIndexBuffer(indexes);

    m_Material = *mtl;
    basic_material = m_Material;

    if (m_Material.isDiffuseMapSet())
    {
        m_DiffuseMap = new QOpenGLTexture(m_Material.getDiffuseMap().mirrored());
        SetMapFilters(m_DiffuseMap);
    }

    if (m_Material.isNormalMapSet())
    {
        m_NormalMap = new QOpenGLTexture(m_Material.getNormalMap().mirrored());
        SetMapFilters(m_NormalMap);
    }
}

const QVector3D& object3D::GetLocation() const
{
    return m_Translate;
}

const QQuaternion &object3D::GetRotation() const
{
    return m_Rotate;
}

float object3D::GetScale() const
{
    return m_Scale;
}

material *object3D::GetMaterial()
{
    return &m_Material;
}

void object3D::reset_material()
{
    m_Material = basic_material;
}

void object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool using_textures)
{
    if(!m_VertexBuffer.isCreated() || !m_IndexBuffer.isCreated()) return;

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_Translate);
    modelMatrix.rotate(m_Rotate);
    modelMatrix.scale(m_Scale);
    modelMatrix = m_GlobalTransform * modelMatrix;

    program->setUniformValue("u_modelMatrix", modelMatrix);

    program->setUniformValue("u_usingNormalMap", false);
    program->setUniformValue("u_usingDiffuseMap", false);

    if (using_textures) {

        if (m_Material.isDiffuseMapSet()) {

            m_DiffuseMap->bind(0);
            program->setUniformValue("u_diffuseMap", 0);
            program->setUniformValue("u_usingDiffuseMap", true);
        }

        if (m_Material.isNormalMapSet()) {

            m_NormalMap->bind(1);
            program->setUniformValue("u_normalMap", 1);
            program->setUniformValue("u_usingNormalMap", true);
        }
    }

    program->setUniformValue("u_MaterialProperty.diffuseColor", m_Material.DiffuseColor());
    program->setUniformValue("u_MaterialProperty.ambienceColor", m_Material.AmbienceColor());
    program->setUniformValue("u_MaterialProperty.specularColor", m_Material.SpecularColor());
    program->setUniformValue("u_MaterialProperty.shines", m_Material.Shines());

    m_VertexBuffer.bind();

    int offset = 0;

    int vertloc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertloc);
    program->setAttributeBuffer(vertloc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texloc = program->attributeLocation("a_textcoord");
    program->enableAttributeArray(texloc);
    program->setAttributeBuffer(texloc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normloc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normloc);
    program->setAttributeBuffer(normloc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_IndexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    m_VertexBuffer.release();
    m_IndexBuffer.release();

    if (m_Material.isDiffuseMapSet()) m_DiffuseMap->release();
    if (m_Material.isNormalMapSet()) m_NormalMap->release();
}

void object3D::rotate(const QQuaternion &r)
{
    m_Rotate *= r;
}

void object3D::translate(const QVector3D &t)
{
    m_Translate += t;
}

void object3D::scale(float s)
{
    m_Scale *= s;
}

void object3D::SetGlobalTransform(const QMatrix4x4 &gt)
{
    m_GlobalTransform = gt;
}

void object3D::AllocateVertexBuffer(const QVector<VertexData> &vertexes)
{
    if (m_VertexBuffer.isCreated()) m_VertexBuffer.destroy();

    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    m_VertexBuffer.release();
}

void object3D::AllocateIndexBuffer(const QVector<GLuint> &indexes)
{
    if (m_IndexBuffer.isCreated()) m_IndexBuffer.destroy();

    m_IndexBuffer.create();
    m_IndexBuffer.bind();
    m_IndexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_IndexBuffer.release();
}

void object3D::SetMapFilters(QOpenGLTexture *map)
{
    map->setMinificationFilter(QOpenGLTexture::Nearest);

    map->setMinificationFilter(QOpenGLTexture::Linear);

    map->setWrapMode(QOpenGLTexture::Repeat);
}
