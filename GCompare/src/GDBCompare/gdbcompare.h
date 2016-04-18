#ifndef GDBCOMPARE_H
#define GDBCOMPARE_H

#include <QtWidgets/QMainWindow>
#include "ui_gdbcompare.h"

class GDBCompare : public QMainWindow
{
    Q_OBJECT

public:
    GDBCompare(QWidget *parent = 0);
    ~GDBCompare();

private:
    Ui::GDBCompareClass ui;
};

#endif // GDBCOMPARE_H
