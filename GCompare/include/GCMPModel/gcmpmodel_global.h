#ifndef GCMPMODEL_GLOBAL_H
#define GCMPMODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef GCMPMODEL_LIB
# define GCMPMODEL_EXPORT Q_DECL_EXPORT
#else
# define GCMPMODEL_EXPORT Q_DECL_IMPORT
#endif

#endif // GCMPMODEL_GLOBAL_H
