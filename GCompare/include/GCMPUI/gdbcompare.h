#ifndef GDBCOMPARE_H
#define GDBCOMPARE_H

#include <QtWidgets/QWidget>
#include "GCMPUI/gcmpframelesswidget.h"

//Ö÷´°Ìå
class GDBCompare : public GCMPFramelessWidget
{
    Q_OBJECT

public:
    GDBCompare(const QString strTitle, GCMPFrameTitleStyle oStyle, QWidget *pCenterWidget, QWidget* pParent = 0);
    ~GDBCompare();

private:

};

#endif // GDBCOMPARE_H
