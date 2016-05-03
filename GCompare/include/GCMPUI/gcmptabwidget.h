#ifndef GCMPTABWIDGET_H
#define GCMPTABWIDGET_H

#include <QTabWidget>
#include "GCMPUI/gcmpui_global.h"

class GCMPUI_EXPORT GCMPTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    GCMPTabWidget(QWidget *parent = 0);
    ~GCMPTabWidget();

private:

};

#endif // GCMPTABWIDGET_H
