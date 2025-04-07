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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjetJeuxEchecsClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ProjetJeuxEchecsClass)
    {
        if (ProjetJeuxEchecsClass->objectName().isEmpty())
            ProjetJeuxEchecsClass->setObjectName("ProjetJeuxEchecsClass");
        ProjetJeuxEchecsClass->resize(1074, 857);
        centralWidget = new QWidget(ProjetJeuxEchecsClass);
        centralWidget->setObjectName("centralWidget");
        ProjetJeuxEchecsClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ProjetJeuxEchecsClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1074, 33));
        ProjetJeuxEchecsClass->setMenuBar(menuBar);
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
