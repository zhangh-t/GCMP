#include <QtWidgets/QApplication>
#include "GCMPUI/GCMPUiCommon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget* pMainWidget = createMainFrame();
    pMainWidget->show();
    int nRet = a.exec();
    return nRet;
}
