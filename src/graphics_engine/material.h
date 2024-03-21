#ifndef MATERIAL_H
#define MATERIAL_H

#include "graphics_engine_global.h"

#include <QString>
#include <QVector3D>
#include <QImage>


class GRAPHICS_ENGINE_EXPORT material
{
public:

    material();
    material(const QString& name);
    material(const material& other);

    material& operator=(const material& other);

    ~material() = default;

    QString Name();
    void setName(const QString &n);

    QVector3D DiffuseColor() const;
    void setDiffuseColor(const QVector3D &dc);

    QVector3D AmbienceColor() const;
    void setAmbienceColor(const QVector3D &ac);

    QVector3D SpecularColor() const;
    void setSpecularColor(const QVector3D &sc);

    float Shines() const;
    void setShines(const float &s);

    void setDiffuseMap(const QString &path);
    const QImage &getDiffuseMap() const;
    const QString& getDiffuseMapPath() const;

    bool isDiffuseMapSet() const;

    void setNormalMap(const QString &path);
    const QImage &getNormalMap() const;
    const QString& getNormalMapPath() const;

    bool isNormalMapSet() const;

    void enableDiffuseMap();
    void enableNormalMap();

    void disableDiffuseMap();
    void disableNormalMap();

private:
    QString m_Name;

    QVector3D m_DiffuseColor;
    QVector3D m_AmbienceColor;
    QVector3D m_SpecularColor;

    float m_Shines;

    bool isUsingDiffuseMap;
    bool isUsingNormalMap;

    QString DiffuseMapPath;
    QString NormalMapPath;

    QImage diffuseMap;
    QImage normalMap;

};


#endif // MATERIAL_H
