#ifndef GRAPHICS_ENGINE_GLOBAL_H
#define GRAPHICS_ENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GRAPHICS_ENGINE_LIBRARY)
#  define GRAPHICS_ENGINE_EXPORT Q_DECL_EXPORT
#else
#  define GRAPHICS_ENGINE_EXPORT Q_DECL_IMPORT
#endif

#endif // GRAPHICS_ENGINE_GLOBAL_H
