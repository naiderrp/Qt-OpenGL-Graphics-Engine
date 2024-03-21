#ifndef TOOLS_H
#define TOOLS_H

#include <QVector3D>

namespace tools {

struct VertexData {

    VertexData() = default;

    VertexData(QVector3D p, QVector2D t, QVector3D n) : position (p),
        texCoord(t), normal(n) {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;

    QVector3D tangent;
    QVector3D bitangent;
};

}
#endif // TOOLS_H
