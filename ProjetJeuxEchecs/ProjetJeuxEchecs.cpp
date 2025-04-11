#include "ProjetJeuxEchecs.h"
#include <math.h>
#include <QMessageBox>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include <iostream>
static constexpr int MAX_ROIS = 2;
static constexpr int TAILLE_COTE_ECHIQUIER = 800;
static constexpr int TAILLE_CASE = TAILLE_COTE_ECHIQUIER / 8;

void Echiquier::ajouterPiece(Piece* piece)
{
    using namespace std;
    pair<int, int> paire = make_pair(piece->getX(), piece->getY());
    positionPieces_[paire] = piece;
}
void EchiquierWidget::ajouterPiece(Piece* piece)
{
    auto* pieceWidget = new PieceWidget(piece, this);
    pieceWidget->move(piece->getX() * TAILLE_CASE, piece->getY() * TAILLE_CASE); 
    pieceWidget->show();
    pieceWidgets_.push_back(pieceWidget);
    ptrEchiquier_->ajouterPiece(piece);
} 

void Echiquier::enleverPieces()
{
    for (auto& paire : positionPieces_)
    {
        delete paire.second;
    }

    positionPieces_.clear();
}

void EchiquierWidget::chargerPartie(int numPartie)
{
    ptrEchiquier_->enleverPieces();
    try
    {
        switch (numPartie)
        {
            case 1:
            {
                ajouterPiece(new Roi(0, 0, true));
                ajouterPiece(new Roi(1, 0, true));
                ajouterPiece(new Roi(3, 4, false));
                ajouterPiece(new Cavalier(1, 1, true));
                ajouterPiece(new Cavalier(5, 5, false));
                ajouterPiece(new Tour(0, 7, true));
                ajouterPiece(new Tour(7, 0, false));
                break;
            }
            case 2:
            {
                ajouterPiece(new Roi(2,2, true));
                ajouterPiece(new Roi(6,6, false));
                ajouterPiece(new Cavalier(1,3, true));
                ajouterPiece(new Cavalier(5,4, false));
                ajouterPiece(new Tour(2,7, true));
                ajouterPiece(new Tour(7,2, false));
                break;
            }
            case 3:
            {
                ajouterPiece(new Roi(4,4, true));
                ajouterPiece(new Roi(7,7, false));
                ajouterPiece(new Cavalier(3,5, true));
                ajouterPiece(new Cavalier(6,6, false));
                ajouterPiece(new Tour(0,3, true));
                ajouterPiece(new Tour(7,1, false));
                break;
            }
        }
    }
    catch (TropDeRoisException)
    {
        QMessageBox::critical(nullptr, "Erreur", "Trop de rois sont actuellement sur l'echiquier");
    }
}

ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ProjetJeuxEchecsClass())
{
    ui->setupUi(this);
    echiquierWidget_ = new EchiquierWidget(this,new Echiquier());
    //echiquierWidget_->chargerPartie(2);

    infoTourLabel_ = new QLabel("Tour des blancs", this);
    
    boutonEndgame1_ = new QPushButton("Endgame 1", this);
    boutonEndgame2_ = new QPushButton("Endgame 2", this);
    boutonEndgame3_ = new QPushButton("Endgame 3", this);

    QHBoxLayout* boutonsLayout = new QHBoxLayout;
    boutonsLayout->addWidget(boutonEndgame1_);
    boutonsLayout->addWidget(boutonEndgame2_);
    boutonsLayout->addWidget(boutonEndgame3_);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(boutonsLayout);
    mainLayout->addWidget(infoTourLabel_);
    mainLayout->addWidget(echiquierWidget_);
    ui->centralWidget->setLayout(mainLayout);

    connect(boutonEndgame1_, &QPushButton::clicked, this, [=]() {
        echiquierWidget_->chargerPartie(1);
        });

    connect(boutonEndgame2_, &QPushButton::clicked, this, [=]() {
        echiquierWidget_->chargerPartie(2);
        });

    connect(boutonEndgame3_, &QPushButton::clicked, this, [=]() {
        echiquierWidget_->chargerPartie(3);
        });
}

void ProjetJeuxEchecs::changerTour()
{
    tourAuxBlancs_ = !tourAuxBlancs_;
}


ProjetJeuxEchecs::~ProjetJeuxEchecs()
{
    delete ui;
    delete echiquierWidget_;
}


EchiquierWidget::EchiquierWidget(QWidget* parent, Echiquier* ptrEchiquier)
    : QWidget(parent), echiquierPixMap_(":/images/images/echiquier.png")
{
    ptrEchiquier_ = ptrEchiquier;
    setFixedSize(TAILLE_COTE_ECHIQUIER, TAILLE_COTE_ECHIQUIER);

    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            QPushButton* button = new QPushButton(this);

            button->setStyleSheet("background-color: transparent; border: none;");
            button->setFixedSize(TAILLE_COTE_ECHIQUIER / 8, TAILLE_COTE_ECHIQUIER / 8);
            layout->addWidget(button, i, j);

            connect(button, &QPushButton::clicked, [=] {
                qDebug() << "Case " << i << ", " << j << " cliquée";
                });
        }
    }

    setLayout(layout);
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
        if (piece->estBlanc())
        {
            return ":/images/images/roi_blanc.png";
        }
        else
        {
            return ":/images/images/roi_noir.png";
        }
    }
    else if (dynamic_cast<Tour*>(piece))
    {
        if (piece->estBlanc())
        {
            return ":/images/images/tour_blanche.png";
        }
        else
        {
            return ":/images/images/tour_noire.png";
        }
    }
    else
    {
        if (piece->estBlanc())
        {
            return ":/images/images/cavalier_blanc.png";
        }
        else
        {
            return ":/images/images/cavalier_noir.png";
        }
    }
}

PieceWidget::PieceWidget(Piece* pieceModele, QWidget* parent) : QLabel(parent), pieceModele_(pieceModele)
{
    setPixmap(QPixmap(getImagePathForPiece(pieceModele_)).scaled(TAILLE_CASE, TAILLE_CASE));
    setFixedSize(TAILLE_CASE, TAILLE_CASE);
    setAttribute(Qt::WA_TransparentForMouseEvents); // optionnel si tu veux laisser passer les clics
}


//Fonctions des classes du namespace Modele
Roi::Roi(int x, int y,bool estBlanc) : Piece(x,y,estBlanc)
{
    nRois++;
    if (nRois > MAX_ROIS)
    {
        throw TropDeRoisException();
    }
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
