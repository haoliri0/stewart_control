/********************************************************************************
** Form generated from reading UI file 'ldata.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LDATA_H
#define UI_LDATA_H

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

class Ui_LdataClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *LdataClass)
    {
        if (LdataClass->objectName().isEmpty())
            LdataClass->setObjectName(QStringLiteral("LdataClass"));
        LdataClass->resize(600, 400);
        menuBar = new QMenuBar(LdataClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        LdataClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(LdataClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        LdataClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(LdataClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        LdataClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(LdataClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        LdataClass->setStatusBar(statusBar);

        retranslateUi(LdataClass);

        QMetaObject::connectSlotsByName(LdataClass);
    } // setupUi

    void retranslateUi(QMainWindow *LdataClass)
    {
        LdataClass->setWindowTitle(QApplication::translate("LdataClass", "Ldata", 0));
    } // retranslateUi

};

namespace Ui {
    class LdataClass: public Ui_LdataClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LDATA_H
