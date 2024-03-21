#include "object3dbuilder.h"

Object3dBuilder::Object3dBuilder() : mtl(0)
{
}

Object3dBuilder &Object3dBuilder::build_vertex_buffer(float width, float height, float depth)
{
    float w_by2 = width / 2.0f;

    float h_by2 = height / 2.0f;

    float d_by2 = depth / 2.0f;

    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, d_by2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, h_by2, -d_by2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(w_by2, h_by2, -d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, -d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, d_by2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-w_by2, -h_by2, -d_by2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, d_by2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(w_by2, -h_by2, -d_by2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::build_index_buffer()
{

    for(GLuint i = 0; i < 24; i += 4)
    {
        indexes.append(i + 0); indexes.append(i + 1); indexes.append(i + 2);
        indexes.append(i + 2); indexes.append(i + 1); indexes.append(i + 3);
    }

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::set_material_name(const QString &name)
{
    bind_material();

    mtl->setName(name);

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::add_diffuse_map(const QString &path)
{
    bind_material();

    mtl->setDiffuseMap(path);

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::add_normal_map(const QString &path)
{
    bind_material();

    mtl->setNormalMap(path);

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::add_shines(float shines)
{
    bind_material();

    mtl->setShines(shines);

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::add_diffuse_color(const QVector3D &color)
{
    bind_material();

    mtl->setDiffuseColor(color);

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::add_specular_color(const QVector3D &color)
{
    bind_material();

    mtl->setSpecularColor(color);

    return static_cast<Object3dBuilder&>(*this);
}

Object3dBuilder &Object3dBuilder::add_ambience_color(const QVector3D &color)
{
    bind_material();

    mtl->setAmbienceColor(color);

    return static_cast<Object3dBuilder&>(*this);
}

QVector<VertexData> &Object3dBuilder::getVertexBuff()
{
    return vertexes;
}

QVector<GLuint> &Object3dBuilder::getIndexBuff()
{
    return indexes;
}

void Object3dBuilder::reset()
{
    if (!mtl) return;

    mtl = 0;
}

material *Object3dBuilder::getMaterialFromFile(const QString& path, const QStringList &file_text_splitted_by_space)
{
    const QStringList& list = file_text_splitted_by_space;
    QFileInfo fileInfo(path);

    if (list.size() == 1) return mtl;

    bind_material(list[1]);

    parse_file_data(list, fileInfo);

    return mtl;
}

material *Object3dBuilder::getMaterial()
{
    return mtl;
}

void Object3dBuilder::parse_file_data(const QStringList &list, QFileInfo& fileInfo)
{
    if (list[0] == "Ns")
        mtl->setShines(list[1].toFloat());

    else if (list[0] == "Ka")
        mtl->setAmbienceColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

    else if (list[0] == "Kd")
        mtl->setDiffuseColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

    else if (list[0] == "Ks")
        mtl->setSpecularColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));

    else if (list[0] == "map_Kd")
        mtl->setDiffuseMap(QString("%1/%2").arg(fileInfo.absolutePath()).arg(list[1]));

    else if (list[0] == "map_Normal")
        mtl->setNormalMap(QString("%1/%2").arg(fileInfo.absolutePath()).arg(list[1]));
}


template<typename ...Args>
void Object3dBuilder::bind_material(Args&& ...args)
{
    if (!mtl)
        mtl = new material(std::forward<Args>(args)...);
}
