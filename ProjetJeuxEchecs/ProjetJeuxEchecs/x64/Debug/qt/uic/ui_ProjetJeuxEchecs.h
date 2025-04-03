/********************************************************************************
** Form generated from reading UI file 'ProjetJeuxEchecs.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJETJEUXECHECS_H
#define UI_PROJETJEUXECHECS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjetJeuxEchecsClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ProjetJeuxEchecsClass)
    {
        if (ProjetJeuxEchecsClass->objectName().isEmpty())
            ProjetJeuxEchecsClass->setObjectName("ProjetJeuxEchecsClass");
        ProjetJeuxEchecsClass->resize(600, 400);
        menuBar = new QMenuBar(ProjetJeuxEchecsClass);
        menuBar->setObjectName("menuBar");
        ProjetJeuxEchecsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ProjetJeuxEchecsClass);
        mainToolBar->setObjectName("mainToolBar");
        ProjetJeuxEchecsClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ProjetJeuxEchecsClass);
        centralWidget->setObjectName("centralWidget");
        ProjetJeuxEchecsClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ProjetJeuxEchecsClass);
        statusBar->setObjectName("statusBar");
        ProjetJeuxEchecsClass->setStatusBar(statusBar);

        retranslateUi(ProjetJeuxEchecsClass);

        QMetaObject::connectSlotsByName(ProjetJeuxEchecsClass);
    } // setupUi

    void retranslateUi(QMainWindow *ProjetJeuxEchecsClass)
    {
        ProjetJeuxEchecsClass->setWindowTitle(QCoreApplication::translate("ProjetJeuxEchecsClass", "ProjetJeuxEchecs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjetJeuxEchecsClass: public Ui_ProjetJeuxEchecsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJETJEUXECHECS_H
