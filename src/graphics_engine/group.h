#ifndef GROUP_H
#define GROUP_H

#include "graphics_engine.h"

#include <QVector>

class GRAPHICS_ENGINE_EXPORT group : public WorldEngineObject
{
public:

    group();

    virtual ~group() = default;

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool usingTextures = true);

    void rotate(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(float s);

    void SetGlobalTransform(const QMatrix4x4 &q);

    void rotateObject(WorldEngineObject *obj);

    void addObject(WorldEngineObject *obj);
    void delObject(qsizetype index);

    WorldEngineObject *getObj(qsizetype index);

    WorldEngineObject *first();
    WorldEngineObject *last();

    const QVector<WorldEngineObject*> &get_objects() const;

    qsizetype size() const;

    bool erase(WorldEngineObject* obj);
    void erase(qsizetype index);

    bool empty() const;

protected:

    QMatrix4x4 getLocalMatrix();

    void updateGlobalTranslate();

private:

    QQuaternion m_rotate;
    QVector3D m_translate;

    float m_scale;

    QMatrix4x4 m_globalTransforms;

    QVector<WorldEngineObject *> objects;
};


#endif // GROUP_H
