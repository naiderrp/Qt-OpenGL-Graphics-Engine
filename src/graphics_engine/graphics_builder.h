#ifndef GRAPHICS_BUILDER_H
#define GRAPHICS_BUILDER_H

#include "material.h"

#include <QString>

class GraphicsBuilder
{
public:

    virtual material* getMaterial() = 0;

    virtual material *getMaterialFromFile(const QString& path, const QStringList &file_text_splitted_by_space) = 0;

    virtual ~GraphicsBuilder() = default;
 };


#endif // GRAPHICS_BUILDER_H
