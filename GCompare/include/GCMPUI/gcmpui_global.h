#ifndef GCMPUI_GLOBAL_H
#define GCMPUI_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef GCMPUI_LIB
# define GCMPUI_EXPORT Q_DECL_EXPORT
#else
# define GCMPUI_EXPORT Q_DECL_IMPORT
#endif

#endif // GCMPUI_GLOBAL_H
