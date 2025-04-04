﻿#include "ProjetJeuxEchecs.h"
#include <math.h>
#include <QMessageBox>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QWidget>
#include <iostream>
static constexpr int MAX_ROIS = 2;
static constexpr int TAILLE_COTE_ECHIQUIER = 400;
static constexpr int TAILLE_CASE = TAILLE_COTE_ECHIQUIER / 8;

//Fonctions des classes du namespace UI
ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProjetJeuxEchecsClass())
{
    ui->setupUi(this);
}

ProjetJeuxEchecs::~ProjetJeuxEchecs()
{
    delete ui;
}

EchiquierWidget::EchiquierWidget(QWidget* parent) : QWidget(parent), echiquierPixMap_(":/images/echiquier.png")
{
    setFixedSize(TAILLE_COTE_ECHIQUIER, TAILLE_COTE_ECHIQUIER);
}
void EchiquierWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (!echiquierPixMap_.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), echiquierPixMap_);
    }
}

QString getImagePathForPiece(Piece* piece)
{
    if (dynamic_cast<Roi*>(piece))
    {
        return ":/images/roi.png";
    }
    else if (dynamic_cast<Tour*>(piece))
    {
        return ":/images/tour.png";
    }
    else
    {
        return ":/images/cavalier.png";
    }
}

PieceWidget::PieceWidget(Piece* pieceModele, QWidget* parent) : QLabel(parent), pieceModele_(pieceModele)
{
    setPixmap(QPixmap(getImagePathForPiece(pieceModele_)).scaled(TAILLE_CASE, TAILLE_CASE));
    setFixedSize(TAILLE_CASE, TAILLE_CASE);
}

//Fonctions des classes du namespace Modele
Roi::Roi(int x, int y,bool estBlanc) : Piece(x,y,estBlanc)
{
    if (nRois >= MAX_ROIS)
    {
        throw new TropDeRoisException;
    }
    nRois++;
}
Roi::~Roi()
{
    nRois--;
}
int Roi::nRois = 0;

bool Roi::estMouvementValide(int x, int y) const
{
    return abs(x - x_) <= 1 && abs(y - y_) <= 1;
}
bool Cavalier::estMouvementValide(int x, int y) const
{
    return (abs(x - x_) == 1 && abs(y-y_) == 3) || (abs(x - x_) == 3 && abs(y - y_) == 1);
}
bool Tour::estMouvementValide(int x, int y) const
{
    return x_ == x || y_ == y;
}
void Echiquier::deplacerPiece(Piece* ptrPiece, int x, int y)
{
    if (ptrPiece->estMouvementValide(x, y))
    {
        //regarder si y'a une piece dans le chemin
        deplacerSansVerification(ptrPiece, x, y);
    }
    else
    {
        QMessageBox::critical(nullptr, "Erreur", "Ce mouvement n'est pas valide");
    }
}
void Echiquier::deplacerSansVerification(Piece* ptrPiece, int x, int y)
{
    using namespace std;
    positionPieces_[pair(ptrPiece->getX(), ptrPiece->getY())] = nullptr;
    ptrPiece->changerPosition(x, y);
    positionPieces_[pair(x, y)] = ptrPiece;
}
MouvementTemporaire::MouvementTemporaire(Echiquier* ptrEchiquier,Piece* ptrPiece, int nouveauX, int nouveauY)
{
    ptrEchiquier_ = ptrEchiquier;
    ptrPiece_ = ptrPiece;

    ancienX_ = ptrPiece_->getX();
    ancienY_ = ptrPiece_->getY();
}
MouvementTemporaire::~MouvementTemporaire()
{
    ptrEchiquier_->deplacerSansVerification(ptrPiece_, ancienX_, ancienY_);
}
