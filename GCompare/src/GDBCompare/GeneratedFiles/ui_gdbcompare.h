/********************************************************************************
** Form generated from reading UI file 'gdbcompare.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GDBCOMPARE_H
#define UI_GDBCOMPARE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GDBCompareClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GDBCompareClass)
    {
        if (GDBCompareClass->objectName().isEmpty())
            GDBCompareClass->setObjectName(QStringLiteral("GDBCompareClass"));
        GDBCompareClass->resize(600, 400);
        menuBar = new QMenuBar(GDBCompareClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        GDBCompareClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GDBCompareClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GDBCompareClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GDBCompareClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GDBCompareClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GDBCompareClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GDBCompareClass->setStatusBar(statusBar);

        retranslateUi(GDBCompareClass);

        QMetaObject::connectSlotsByName(GDBCompareClass);
    } // setupUi

    void retranslateUi(QMainWindow *GDBCompareClass)
    {
        GDBCompareClass->setWindowTitle(QApplication::translate("GDBCompareClass", "GDBCompare", 0));
    } // retranslateUi

};

namespace Ui {
    class GDBCompareClass: public Ui_GDBCompareClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GDBCOMPARE_H
