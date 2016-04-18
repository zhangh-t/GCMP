#include "gdbcompare.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GDBCompare w;
    w.show();
    return a.exec();
}
