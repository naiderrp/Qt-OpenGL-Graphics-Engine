#include "dimensional.h"

#include <QDir>

dimensional::~dimensional()
{
    for(auto piece: object_pieces)
        delete piece;
}

bool dimensional::loadObjectFromFile(const QString &filename)
{
    QFile objfile(filename);

    if(! objfile.open(QFile::ReadOnly)) {

        qCritical() << "File not opened:" << filename;

        return false;
    }

    obj_path = filename;

    QTextStream input(&objfile);

    QVector<QVector3D> coords;
    QVector<QVector2D> texturcoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;

    QString mtlName;

    object3D* object = nullptr;

    while(!input.atEnd()) {

        auto str = input.readLine();

        if(str.isEmpty())
            continue;

        auto strlist = str.split(' '); strlist.removeAll("");

        auto key = strlist.at(0);

        if(key == "mtllib") {

            if(strlist.size() > 1) {

                auto file = QFileInfo(filename).absolutePath() + QDir::separator() + strlist.at(1);

                if(QFile(file).exists())
                    materials.loadMaterialsFromFile(file);
                else
                    qCritical() << "File not exists:" << file;
            }

            else
                qCritical() << "Error at line (count):" << str;
        }
        else if(key.toLower() == "v") {

            if(strlist.size() > 3)
                coords.append(QVector3D(strlist.at(1).toFloat(),
                                        strlist.at(2).toFloat(),
                                        strlist.at(3).toFloat()));

            else
                qCritical() << "Error at line (count):" << str;
        }

        else if(key.toLower() == "vt") {

            if(strlist.size() > 2)
                texturcoords.append(QVector2D(strlist.at(1).toFloat(),
                                              strlist.at(2).toFloat()));
            else
                qCritical() << "Error at line (count):" << str;
        }

        else if(key.toLower() == "vn") {

            if(strlist.size() == 4)
                normals.append(QVector3D(strlist.at(1).toFloat(),
                                         strlist.at(2).toFloat(),
                                         strlist.at(3).toFloat()));

            else
                qCritical() << "Error at line (count):" << str;
        }
        else if(key.toLower() == "f")

        for(int i = 1; i < strlist.size(); ++i) {

            auto v = strlist.at(i).split('/');

            if(v.size() == 3 && !v.at(1).isEmpty() && !v.at(2).isEmpty()) {
                vertexes.append(VertexData(coords.at(v.at(0).toInt() - 1),
                                           texturcoords.at(v.at(1).toInt() - 1),
                                           normals.at(v.at(2).toInt() - 1)));

                indexes.append(static_cast<GLuint>(indexes.size()));
            }
            else {
                qCritical() << "Unsupported OBJ data format:" << strlist.at(i);
                break;
            }
        }

        else if(key.toLower() == "usemtl")
        {
            if(strlist.size() > 1) {

                if(object) object->init(vertexes, indexes, materials.get(mtlName));
                mtlName = strlist.at(1);
                addObject(object);

                object = new object3D;
                vertexes.clear();
                indexes.clear();
            }
            else
                qCritical() << "Error at line (count):" << str;
        }
    }

    objfile.close();

    if(object)
        object->init(vertexes, indexes, materials.get(mtlName));

    addObject(object);

    return true;
}

void dimensional::addObject(object3D *obj)
{
    if(!obj) return;

    for(auto piece: object_pieces)
        if (piece == obj) return;

    object_pieces.append(obj);

    if (!materials.contains(obj->GetMaterial()->Name()))
        materials.add(obj->GetMaterial());
}

object3D *dimensional::getObject(qsizetype index)
{
    return object_pieces.at(index);
}

void dimensional::rotate(const QQuaternion &r)
{
    for(auto piece: object_pieces)
        piece->rotate(r);
}

void dimensional::translate(const QVector3D &t)
{
    for(auto piece: object_pieces)
        piece->translate(t);
}

void dimensional::scale(float s)
{
    for(auto piece: object_pieces)
        piece->scale(s);
}

void dimensional::SetGlobalTransform(const QMatrix4x4 &gt)
{
    for(auto piece: object_pieces)
        piece->SetGlobalTransform(gt);
}

void dimensional::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool using_textures)
{
    for(auto piece: object_pieces)
        piece->draw(program, functions, using_textures);
}

const QVector3D &dimensional::GetLocation() const
{
    return object_pieces.last()->GetLocation();
}

const QQuaternion &dimensional::GetRotation() const
{
    return object_pieces.last()->GetRotation();
}

float dimensional::GetScale() const
{
    return object_pieces.last()->GetScale();
}

const QString &dimensional::GetFilePath() const
{
    return obj_path;
}

material_library &dimensional::GetMaterials()
{
    return materials;
}
