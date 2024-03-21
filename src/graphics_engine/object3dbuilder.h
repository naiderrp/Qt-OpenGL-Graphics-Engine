#ifndef OBJECT3DBUILDER_H
#define OBJECT3DBUILDER_H

#include "graphics_engine_global.h"

#include "tools.h"
#include "materiallibrary.h"

#include <QOpenGLBuffer>
#include <QFileInfo>

using tools::VertexData;

class GRAPHICS_ENGINE_EXPORT Object3dBuilder
{
public:
    Object3dBuilder();

    Object3dBuilder& build_vertex_buffer(float width, float height, float depth);
    Object3dBuilder& build_index_buffer();

    Object3dBuilder& set_material_name(const QString& name);

    Object3dBuilder& add_diffuse_map(const QString &path);
    Object3dBuilder& add_normal_map(const QString &path);

    Object3dBuilder& add_shines(float shines);

    Object3dBuilder& add_diffuse_color(const QVector3D& color);
    Object3dBuilder& add_specular_color(const QVector3D& color);
    Object3dBuilder& add_ambience_color(const QVector3D& color);

    QVector<VertexData> &getVertexBuff();
    QVector<GLuint> &getIndexBuff();

    void reset();

    material* getMaterialFromFile(const QString& path, const QStringList &file_text_splitted_by_space);

    material* getMaterial();

    ~Object3dBuilder() = default;

protected:

    template<typename ...Args>
    void bind_material(Args&& ...args);

    void parse_file_data(const QStringList &list, QFileInfo& fileInfo);

private:

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    material *mtl;
};

#endif // OBJECT3DBUILDER_H
