#include "material.h"

material::material() : isUsingDiffuseMap(false), isUsingNormalMap(false)
{
}

material::material(const QString& name) : m_Name(name), isUsingDiffuseMap(false), isUsingNormalMap (false)
{
}

material::material(const material &other) : m_Name(other.m_Name),
    m_DiffuseColor(other.m_DiffuseColor), m_AmbienceColor(other.m_AmbienceColor), m_SpecularColor(other.m_SpecularColor),
    m_Shines(other.m_Shines), isUsingDiffuseMap(other.isUsingDiffuseMap), isUsingNormalMap(other.isUsingNormalMap),
    DiffuseMapPath(other.DiffuseMapPath), NormalMapPath(other.NormalMapPath),
    diffuseMap(DiffuseMapPath), normalMap(NormalMapPath)
{
}

material &material::operator=(const material &other)
{
    m_Name = other.m_Name;

    m_DiffuseColor = other.m_DiffuseColor;
    m_AmbienceColor = other.m_AmbienceColor;
    m_SpecularColor = other.m_SpecularColor;

    m_Shines = other.m_Shines;

    isUsingDiffuseMap = other.isUsingDiffuseMap;
    isUsingNormalMap = other.isUsingNormalMap;

    DiffuseMapPath = other.DiffuseMapPath;
    NormalMapPath = other.NormalMapPath;

    diffuseMap = other.diffuseMap.copy();
    normalMap = other.normalMap.copy();

    return *this;
}

QString material::Name()
{
    return m_Name;
}

void material::setName(const QString &n)
{
    m_Name = n;
}

QVector3D material::DiffuseColor() const
{
    return m_DiffuseColor;
}

void material::setDiffuseColor(const QVector3D &dc)
{
    m_DiffuseColor = dc;
}

QVector3D material::AmbienceColor() const
{
    return m_AmbienceColor;
}

void material::setAmbienceColor(const QVector3D &ac)
{
    m_AmbienceColor = ac;
}

QVector3D material::SpecularColor() const
{
    return m_SpecularColor;
}

void material::setSpecularColor(const QVector3D &sc)
{
    m_SpecularColor = sc;
}

float material::Shines() const
{
    return m_Shines;
}

void material::setShines(const float &s)
{
    m_Shines = s;
}

void material::setDiffuseMap(const QString &path)
{
    diffuseMap = QImage(path);
    DiffuseMapPath = path;

    isUsingDiffuseMap = true;
}

const QImage &material::getDiffuseMap() const
{
    return diffuseMap;
}

const QString &material::getDiffuseMapPath() const
{
    return DiffuseMapPath;
}

bool material::isDiffuseMapSet() const
{
    return isUsingDiffuseMap;
}

void material::setNormalMap(const QString &path)
{
    normalMap = QImage(path);
    NormalMapPath = path;

    isUsingNormalMap = true;
}

const QImage &material::getNormalMap() const
{
    return normalMap;
}

const QString &material::getNormalMapPath() const
{
    return NormalMapPath;
}

bool material::isNormalMapSet() const
{
    return isUsingNormalMap;
}

void material::enableDiffuseMap()
{
    isUsingDiffuseMap = true;
}

void material::enableNormalMap()
{
    isUsingNormalMap = true;
}

void material::disableDiffuseMap()
{
    isUsingDiffuseMap = false;
}

void material::disableNormalMap()
{
    isUsingNormalMap = false;
}
