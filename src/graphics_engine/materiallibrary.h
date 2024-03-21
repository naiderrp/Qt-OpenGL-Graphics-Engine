#ifndef MATERIALLIBRARY_H
#define MATERIALLIBRARY_H

#include "material.h"
#include <QVector>

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include <QDebug>

class material_library
{
public:

    material_library() = default;

    bool loadMaterialsFromFile(const QString &filename);

    void add(material* m);

    material* get(qsizetype index);

    material* get(const QString &name);

    bool contains(const QString &name);

    qsizetype size() { return m_Materials.size(); }

    material* operator[](qsizetype i);

    ~material_library();

private:
    QVector<material*> m_Materials;
};

#endif // MATERIALLIBRARY_H
