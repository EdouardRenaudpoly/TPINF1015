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
static constexpr int N_CASES_COTE = 8;

using namespace std;

void Echiquier::ajouterPiece(Piece* piece)
{
    pair<int, int> paire = make_pair(piece->getX(), piece->getY());
    positionPieces_[paire] = piece;
}

void EchiquierWidget::mettreAJour()
{
    for (auto widget : pieceWidgets_) 
    {
        grille_->removeWidget(widget); 
        widget->deleteLater();   
    }
    pieceWidgets_.clear();

    for (int x = 0; x < N_CASES_COTE; ++x)
    {
        for (int y = 0; y < N_CASES_COTE; ++y) 
        {
            Piece* piece = ptrEchiquier_->getPiece(x, y);
            if (piece)
            {
                auto* widget = new PieceWidget(piece, this);
                grille_->addWidget(widget, y, x); // ligne/colonne
                pieceWidgets_[qMakePair(x, y)] = widget;
            }
        }
    }
}

void Echiquier::enleverPieces()
{
    for (auto& paire : positionPieces_)
    {
        delete paire.second;
    }

    positionPieces_.clear();
}

void EchiquierWidget::reset() {
    qDeleteAll(pieceWidgets_);
    pieceWidgets_.clear();
    ptrEchiquier_->enleverPieces();
    if (!projetJeuxEchecs_->getTourAuxBlancs())
    {
        projetJeuxEchecs_->changerTour();
    }
    mettreAJour();
}

void EchiquierWidget::ajouterPiece(Piece* piece)
{
    auto* pieceWidget = new PieceWidget(piece, this);
    grille_->addWidget(pieceWidget, piece->getY(), piece->getX());
    pieceWidgets_[qMakePair(piece->getX(), piece->getY())] = pieceWidget;
    ptrEchiquier_->ajouterPiece(piece);
} 

void EchiquierWidget::chargerPartie(int numPartie)
{
    reset();
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
    echiquierWidget_ = new EchiquierWidget(this,new Echiquier(),this);

    infoTourLabel_ = new QLabel("Tour des blancs", this);
    
    boutonEndgame1_ = new QPushButton("Endgame 1", this);
    boutonEndgame2_ = new QPushButton("Endgame 2", this);
    boutonEndgame3_ = new QPushButton("Endgame 3", this);
    boutonReset_ = new QPushButton("Recommencer partie", this);

    QHBoxLayout* boutonsLayout = new QHBoxLayout;
    boutonsLayout->addWidget(boutonEndgame1_);
    boutonsLayout->addWidget(boutonEndgame2_);
    boutonsLayout->addWidget(boutonEndgame3_);
    boutonsLayout->addWidget(boutonReset_);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(boutonsLayout);
    mainLayout->addWidget(infoTourLabel_);
    mainLayout->addWidget(echiquierWidget_);
    ui->centralWidget->setLayout(mainLayout);

    connect(boutonEndgame1_, &QPushButton::clicked, this, [=]() {
        indexPartieActuelle = 1;
        echiquierWidget_->chargerPartie(1);
        });

    connect(boutonEndgame2_, &QPushButton::clicked, this, [=]() {
        indexPartieActuelle = 2;
        echiquierWidget_->chargerPartie(2);
        });

    connect(boutonEndgame3_, &QPushButton::clicked, this, [=]() {
        indexPartieActuelle = 3;
        echiquierWidget_->chargerPartie(3);
        });

    connect(boutonReset_, &QPushButton::clicked, this, [=]() {
        echiquierWidget_->chargerPartie(indexPartieActuelle);
        });
}

void ProjetJeuxEchecs::changerTour()
{
    tourAuxBlancs_ = !tourAuxBlancs_;
    if (tourAuxBlancs_) 
    {
        infoTourLabel_->setText("Tour des blancs");
    }
    else 
    {
        infoTourLabel_->setText("Tour des noirs");
    }
}


ProjetJeuxEchecs::~ProjetJeuxEchecs()
{
    delete ui;
    delete echiquierWidget_;
}


EchiquierWidget::EchiquierWidget(QWidget* parent, Echiquier* ptrEchiquier, ProjetJeuxEchecs* projetJeuxEchecs)
    : QWidget(parent), echiquierPixMap_(":/images/images/echiquier.png"), projetJeuxEchecs_(projetJeuxEchecs)
{
    ptrEchiquier_ = ptrEchiquier;
    setFixedSize(TAILLE_COTE_ECHIQUIER, TAILLE_COTE_ECHIQUIER);

    grille_ = new QGridLayout(this);
    grille_->setSpacing(0);
    grille_->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < N_CASES_COTE; ++i) {
        for (int j = 0; j < N_CASES_COTE; ++j) {
            QPushButton* button = new QPushButton(this);

            button->setStyleSheet("background-color: transparent; border: none;");
            button->setFixedSize(TAILLE_COTE_ECHIQUIER / N_CASES_COTE, TAILLE_COTE_ECHIQUIER / N_CASES_COTE);
            grille_->addWidget(button, i, j);

            connect(button, &QPushButton::clicked, [=, this] 
            {
                QPoint pos(j, i);

                if (!attenteDeuxiemeClic_)
                {
                    Piece* pieceSelectionnee = ptrEchiquier_->getPiece(pos.x(), pos.y());
                    if (pieceSelectionnee && pieceSelectionnee->estBlanc() == projetJeuxEchecs_->getTourAuxBlancs())
                    {
                        caseSelectionnee_ = pos;
                        attenteDeuxiemeClic_ = true;
                        qDebug() << "Sélectionnée : " << pos;
                    }
                }
                else 
                {
                    QPoint destination = pos;
                    if (ptrEchiquier_->deplacerPiece(caseSelectionnee_.x(), caseSelectionnee_.y(),destination.x(), destination.y())) 
                    {
                        mettreAJour();
                        projetJeuxEchecs_->changerTour();
                    }
                    else 
                    {
                        qDebug() << "Déplacement invalide.";
                    }
                    attenteDeuxiemeClic_ = false;
                }
            });
        }
    }

    setLayout(grille_);
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

bool Roi::estDeplacementValide(int x, int y) const
{
    return abs(x - x_) <= 1 && abs(y - y_) <= 1;
}
bool Cavalier::estDeplacementValide(int x, int y) const
{
    return (abs(x - x_) == 1 && abs(y-y_) == 2) || (abs(x - x_) == 2 && abs(y - y_) == 1);
}
bool Tour::estDeplacementValide(int x, int y) const
{
    return x_ == x || y_ == y;
}

bool Echiquier::roiEnEchec(bool blanc) 
{
    Piece* roi = nullptr;

    for (const auto& [pos, piece] : positionPieces_)
    {
        if (piece && piece->estBlanc() == blanc && dynamic_cast<Roi*>(piece))
        {
            roi = piece;
            break;
        }
    }

    if (!roi)
    {
        return false;
    }

    int roiX = roi->getX();
    int roiY = roi->getY();

    for (const auto& [pos, piece] : positionPieces_)
    {
        if (piece && piece->estBlanc() != blanc)
        {
            if (piece->estDeplacementValide(roiX, roiY) && cheminEstLibre(piece,roiX,roiY))
                return true;
        }
    }

    return false;
}

bool Echiquier::cheminEstLibre(Piece* piece, int destX, int destY) 
{
    int srcX = piece->getX();
    int srcY = piece->getY();

    if (auto* tour = dynamic_cast<Tour*>(piece)) 
    {
        if (srcY == destY) 
        {
            int minX = min(srcX, destX) + 1;
            int maxX = max(srcX, destX);

            for (int x = minX; x < maxX; ++x)
            {
                if (getPiece(x, srcY))
                {
                    return false;
                }
            }

        }
        else 
        {
            int minY = min(srcY, destY) + 1;
            int maxY = max(srcY, destY);
            for (int y = minY; y < maxY; ++y)
            {
                if (getPiece(srcX, y))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Echiquier::estDeplacementLegal(Piece* piece, int destX, int destY)
{
    if (!piece->estDeplacementValide(destX, destY))
    {
        return false;
    }

    Piece* cible = getPiece(destX, destY);

    if ((cible && cible->estBlanc() == piece->estBlanc()) 
            || !cheminEstLibre(piece, destX, destY))
    {
        return false;
    }

    MouvementTemporaire mouvementTemp(this, piece, destX, destY);

    return !roiEnEchec(piece->estBlanc());
}

bool Echiquier::deplacerPiece(int srcX, int srcY, int destX, int destY)
{
    Piece* piece = getPiece(srcX, srcY);
    if (!piece || !estDeplacementLegal(piece, destX, destY) || (srcX==destX && srcY==destY))
    {
        QMessageBox::critical(nullptr, "Erreur", "Ce deplacement n'est pas valide");
        return false;
    }

    deplacerSansVerification(piece, destX, destY);

    return true;
}

void Echiquier::deplacerSansVerification(Piece* ptrPiece, int x, int y)
{
    using namespace std;
    positionPieces_.erase({ ptrPiece->getX(), ptrPiece->getY()});
    ptrPiece->changerPosition(x, y);
    if (positionPieces_[pair(x, y)])
    {
        delete positionPieces_[pair(x, y)];
    }
    positionPieces_[pair(x, y)] = ptrPiece;
}
MouvementTemporaire::MouvementTemporaire(Echiquier* ptrEchiquier,Piece* ptrPiece, int nouveauX, int nouveauY)
{
    ptrEchiquier_ = ptrEchiquier;
    ptrPiece_ = ptrPiece;
    ancienX_ = ptrPiece_->getX();
    ancienY_ = ptrPiece_->getY();
    ptrEchiquier_->deplacerSansVerification(ptrPiece_, nouveauX, nouveauY);
}
MouvementTemporaire::~MouvementTemporaire()
{
    ptrEchiquier_->deplacerSansVerification(ptrPiece_, ancienX_, ancienY_);
}
