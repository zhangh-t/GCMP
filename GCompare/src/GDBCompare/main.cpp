#include "GDBCompare/Ui/gdbcompare.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GDBCompare w("GGDB Comparer", 0);
    w.show();
    return a.exec();
}
