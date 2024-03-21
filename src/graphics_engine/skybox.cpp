#include "skybox.h"

float pos = 0.0f;

skybox::skybox() : cube(0)
{
}

skybox::skybox(float width, const QString &img) : cube(0)
{
    init(width, img);
}

void skybox::init(float width, const QString &img)
{
    texture = img;

    m_width = width;

    if (indexes.isEmpty()) init_indexes();

    auto vertexes = init_vertexes(width / 2.0f);

    Object3dBuilder background;

    background.add_diffuse_map(img).
               add_shines(96.0f).
               add_diffuse_color(QVector3D(1.0, 1.0, 1.0)).
               add_specular_color(QVector3D(1.0, 1.0, 1.0)).
               add_ambience_color(QVector3D(1.0, 1.0, 1.0));

    if (cube != 0) delete cube;

    cube = new object3D(vertexes, indexes, background.getMaterial());
}

skybox::~skybox()
{
    if (cube != 0) delete cube;
}

const QString &skybox::GetTexture() const
{
    return texture;
}

const QVector3D &skybox::getLocation() const
{
    return cube->GetLocation();
}

void skybox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool using_textures)
{
    cube->draw(program, functions, using_textures);
}

void skybox::rotate(const QQuaternion &r)
{
    cube->rotate(r);
}

void skybox::translate(const QVector3D &t)
{
    cube->translate(t);
}

void skybox::scale(float s)
{
    cube->scale(s);
}

void skybox::SetGlobalTransform(const QMatrix4x4 &gt)
{
    cube->SetGlobalTransform(gt);
}

float skybox::width() const
{
    return m_width;
}

QVector<VertexData> skybox::init_vertexes(float width_div_2)
{
    QVector<VertexData> vertexes; vertexes.reserve(24);

    // сзади
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(1.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(3.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(3.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    // справа
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(3.0f / 4.0f, 2.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(3.0f / 4.0f, 1.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(2.0f / 4.0f, 2.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(2.0f / 4.0f, 1.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    // над
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(2.0f / 4.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(2.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(1.0f / 4.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    // перед
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(2.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(2.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    // слева
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0f, 2.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f / 4.0f, 2.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0f, 1.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f / 4.0f, 1.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    // под
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0f / 4.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(2.0f / 4.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(2.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 1.0f, 0.0f)));

    return vertexes;
}

void skybox::init_indexes()
{
    indexes.clear();

    for(GLuint i = 0; i < 24; i += 4)
    {
        indexes.append(i + 0); indexes.append(i + 2); indexes.append(i + 1);
        indexes.append(i + 2); indexes.append(i + 3); indexes.append(i + 1);
    }
}
