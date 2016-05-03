#include "GCMPUI/GCMPUiCommon.h"
#include "GCMPUI/gdbcompare.h"
#include "GCMPUI/gcmptabwidget.h"

GCMPUI_EXPORT QWidget* createMainFrame()
{
    QWidget* pCenter = new QWidget;
    return new GDBCompare("", gtsNone, pCenter, nullptr);
}
