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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjetJeuxEchecsClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ProjetJeuxEchecsClass)
    {
        if (ProjetJeuxEchecsClass->objectName().isEmpty())
            ProjetJeuxEchecsClass->setObjectName("ProjetJeuxEchecsClass");
        ProjetJeuxEchecsClass->resize(1074, 857);
        centralWidget = new QWidget(ProjetJeuxEchecsClass);
        centralWidget->setObjectName("centralWidget");
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(130, 40, 671, 671));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/images/echiquier.png")));
        ProjetJeuxEchecsClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ProjetJeuxEchecsClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1074, 33));
        ProjetJeuxEchecsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ProjetJeuxEchecsClass);
        mainToolBar->setObjectName("mainToolBar");
        ProjetJeuxEchecsClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ProjetJeuxEchecsClass);
        statusBar->setObjectName("statusBar");
        ProjetJeuxEchecsClass->setStatusBar(statusBar);

        retranslateUi(ProjetJeuxEchecsClass);

        QMetaObject::connectSlotsByName(ProjetJeuxEchecsClass);
    } // setupUi

    void retranslateUi(QMainWindow *ProjetJeuxEchecsClass)
    {
        ProjetJeuxEchecsClass->setWindowTitle(QCoreApplication::translate("ProjetJeuxEchecsClass", "ProjetJeuxEchecs", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ProjetJeuxEchecsClass: public Ui_ProjetJeuxEchecsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJETJEUXECHECS_H
