#ifndef DIMENSIONAL_H
#define DIMENSIONAL_H

#include "object3d.h"

#include <QVector>

#include <QFile>
#include <QFileInfo>

class GRAPHICS_ENGINE_EXPORT dimensional : public WorldEngineObject
{
public:

    dimensional() = default;

    virtual ~dimensional();

    bool loadObjectFromFile(const QString &filename);

    void addObject(object3D* obj);
    object3D* getObject(qsizetype index);

    const QVector3D& GetLocation() const;
    const QQuaternion& GetRotation() const;
    float GetScale() const;

    const QString& GetFilePath() const;

    material_library& GetMaterials();

    void rotate(const QQuaternion &r) override;

    void translate(const QVector3D &t) override;

    void scale(float s) override;

    void SetGlobalTransform(const QMatrix4x4 &gt) override;

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions, bool using_textures = true) override;

private:
    QVector<object3D*> object_pieces;

    material_library materials;

    QString obj_path;
};

#endif // DIMENSIONAL_H
