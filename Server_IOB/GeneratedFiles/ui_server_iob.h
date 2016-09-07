/********************************************************************************
** Form generated from reading UI file 'server_iob.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_IOB_H
#define UI_SERVER_IOB_H

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

class Ui_Server_IOBClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Server_IOBClass)
    {
        if (Server_IOBClass->objectName().isEmpty())
            Server_IOBClass->setObjectName(QStringLiteral("Server_IOBClass"));
        Server_IOBClass->resize(600, 400);
        menuBar = new QMenuBar(Server_IOBClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        Server_IOBClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Server_IOBClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Server_IOBClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Server_IOBClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Server_IOBClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Server_IOBClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Server_IOBClass->setStatusBar(statusBar);

        retranslateUi(Server_IOBClass);

        QMetaObject::connectSlotsByName(Server_IOBClass);
    } // setupUi

    void retranslateUi(QMainWindow *Server_IOBClass)
    {
        Server_IOBClass->setWindowTitle(QApplication::translate("Server_IOBClass", "Server_IOB", 0));
    } // retranslateUi

};

namespace Ui {
    class Server_IOBClass: public Ui_Server_IOBClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_IOB_H
