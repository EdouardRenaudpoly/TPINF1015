#include "Ui.h"
#include "Modele.h"
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

QT_BEGIN_NAMESPACE
namespace Ui
{
    QString getImagePathForPiece(Modele::Piece* piece)
    {
        using namespace Modele;
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

    void EchiquierWidget::deplacerPieceWidget(PieceWidget* widget, int x, int y)
    {
        grille_->removeWidget(widget);
        grille_->addWidget(widget, y, x); // Qt : ligne, colonne
    }

    void PieceWidget::surPieceDeplacee(Modele::Piece* piece, int x, int y)
    {
        if (piece == pieceModele_)
        {
            emit demanderDeplacerWidget(this, x, y);
        }
    }

    PieceWidget::PieceWidget(Modele::Piece* pieceModele, QWidget* parent) : QLabel(parent), pieceModele_(pieceModele)
    {
        
        setPixmap(QPixmap(getImagePathForPiece(pieceModele_)).scaled(TAILLE_CASE, TAILLE_CASE));
        setFixedSize(TAILLE_CASE, TAILLE_CASE);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    //void EchiquierWidget::mettreAJour()
    //{
    //    using namespace Modele;
    //    for (auto widget : pieceWidgets_)
    //    {
    //        grille_->removeWidget(widget);
    //        widget->deleteLater();
    //    }
    //    pieceWidgets_.clear();

    //    for (int x = 0; x < N_CASES_COTE; ++x)
    //    {
    //        for (int y = 0; y < N_CASES_COTE; ++y)
    //        {
    //            Piece* piece = ptrEchiquier_->getPiece(x, y);
    //            if (piece)
    //            {
    //                auto* widget = new PieceWidget(piece, this);
    //                grille_->addWidget(widget, y, x); // ligne/colonne
    //                pieceWidgets_[qMakePair(x, y)] = widget;
    //            }
    //        }
    //    }
    //}

    void EchiquierWidget::reinitialiserPositions() {
        qDeleteAll(pieceWidgets_);
        pieceWidgets_.clear();
        ptrEchiquier_->enleverPieces();
        if (!projetJeuxEchecs_->getTourAuxBlancs())
        {
            projetJeuxEchecs_->changerTour();
        }
    }

    void EchiquierWidget::ajouterPiece(Modele::Piece* piece)
    {
        using namespace Modele;
        auto* pieceWidget = new PieceWidget(piece, this);
        grille_->addWidget(pieceWidget, piece->getY(), piece->getX());
        pieceWidgets_[qMakePair(piece->getX(), piece->getY())] = pieceWidget;

        connect(ptrEchiquier_, &Modele::Echiquier::pieceDeplacee,
            pieceWidget, &PieceWidget::surPieceDeplacee);

        connect(pieceWidget, &PieceWidget::demanderDeplacerWidget,
            this, &EchiquierWidget::deplacerPieceWidget);

        ptrEchiquier_->ajouterPiece(piece);
    }

    void EchiquierWidget::chargerPartie(int numPartie)
    {
        using namespace Modele;
        reinitialiserPositions();
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
                ajouterPiece(new Roi(2, 2, true));
                ajouterPiece(new Roi(6, 6, false));
                ajouterPiece(new Cavalier(1, 3, true));
                ajouterPiece(new Cavalier(5, 4, false));
                ajouterPiece(new Tour(2, 7, true));
                ajouterPiece(new Tour(7, 2, false));
                break;
            }
            case 3:
            {
                ajouterPiece(new Roi(4, 4, true));
                ajouterPiece(new Roi(7, 7, false));
                ajouterPiece(new Cavalier(3, 5, true));
                ajouterPiece(new Cavalier(6, 6, false));
                ajouterPiece(new Tour(0, 3, true));
                ajouterPiece(new Tour(7, 1, false));
                break;
            }
            }
        }
        catch (TropDeRoisException)
        {
            QMessageBox::critical(nullptr, "Erreur", "Trop de rois sont actuellement sur l'echiquier");

        }
    }

    void EchiquierWidget::surPieceCapturee(Modele::Piece* piece)
    {
        auto pos = qMakePair(piece->getX(), piece->getY());
        auto widget = pieceWidgets_.value(pos);
        if (widget)
        {
            grille_->removeWidget(widget);
            widget->deleteLater();
            pieceWidgets_.remove(pos);
        }
    }

    EchiquierWidget::EchiquierWidget(QWidget* parent, Modele::Echiquier* ptrEchiquier, ProjetJeuxEchecs* projetJeuxEchecs)
        : QWidget(parent), echiquierPixMap_(":/images/images/echiquier.png"), projetJeuxEchecs_(projetJeuxEchecs)
    {
        using namespace Modele;

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
                            if (ptrEchiquier_->deplacerPiece(caseSelectionnee_.x(), caseSelectionnee_.y(), destination.x(), destination.y()))
                            {

                                auto widget = pieceWidgets_.value(qMakePair(caseSelectionnee_.x(), caseSelectionnee_.y()));

                                if (widget)
                                {
                                    pieceWidgets_.remove(qMakePair(caseSelectionnee_.x(), caseSelectionnee_.y()));
                                    pieceWidgets_[qMakePair(destination.x(), destination.y())] = widget;

                                    deplacerPieceWidget(widget, destination.x(), destination.y());

                                    connect(ptrEchiquier_, &Modele::Echiquier::pieceDeplacee,
                                        widget, &PieceWidget::surPieceDeplacee);
                                }
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
        connect(ptrEchiquier_, &Modele::Echiquier::pieceCapturee,
            this, &EchiquierWidget::surPieceCapturee);

        setLayout(grille_);
    }


    void EchiquierWidget::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);
        if (!echiquierPixMap_.isNull()) {
            painter.drawPixmap(0, 0, width(), height(), echiquierPixMap_);
        }
    }

    ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::ProjetJeuxEchecsClass())
    {
        using namespace Modele;

        ui->setupUi(this);
        echiquierWidget_ = new EchiquierWidget(this, new Echiquier(), this);

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

}
QT_END_NAMESPACE