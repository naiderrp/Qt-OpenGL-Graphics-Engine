#include "group.h"

group::group() : m_scale(1.0)
{
}

void group::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures)
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->draw(program, functions, usingTextures);
}

void group::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());

}

void group::translate(const QVector3D &t)
{
    m_translate += t;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}

void group::scale(float s)
{
    m_scale *= s;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}

void group::SetGlobalTransform(const QMatrix4x4 &q)
{
    m_globalTransforms = q;

    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}

void group::addObject(WorldEngineObject *obj)
{
    objects.append(obj);

    objects.last()->SetGlobalTransform(getLocalMatrix());
}

void group::delObject(qsizetype index)
{
    objects.remove(index);
}

WorldEngineObject *group::getObj(qsizetype index)
{
    return objects[index];
}

WorldEngineObject *group::first()
{
    return objects.first();
}

WorldEngineObject *group::last()
{
    return objects.last();
}

const QVector<WorldEngineObject *> &group::get_objects() const
{
    return objects;
}

qsizetype group::size() const
{
    return objects.size();
}

bool group::empty() const
{
    return objects.isEmpty();
}

bool group::erase(WorldEngineObject *obj)
{
    return objects.removeAll(obj) > 0 ? true : false;
}

void group::erase(qsizetype index)
{
    objects.remove(index);
}

QMatrix4x4 group::getLocalMatrix()
{
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransforms * localMatrix;

    return localMatrix;
}

void group::updateGlobalTranslate()
{
    for (qsizetype i = 0; i < objects.size(); ++i)
        objects[i]->SetGlobalTransform(getLocalMatrix());
}
