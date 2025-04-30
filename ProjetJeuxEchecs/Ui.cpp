//Ce fichier a pour but d'implémenter un namespace et des classes qui permettent d'afficher notre jeu d'échecs de façon adéquate. Ces classes réagissent aux
//différents signaux et implémentent les boutons dans l'interface usager pour que l'utilisateur puisse jouer au jeu correctement. Ces méthodes s'occupent
//également d'afficher les différentes exceptions et messages d'erreur causé par les actions de l'usager ou l'initialisation des rois. Elle contient les
//Widgets pour l'échiquier, les pieces et les différents boutons et connecte les signaux avec la partie modèle.
//\file   Ui.cpp
//\author Édouard Renaud (2384807) et Zackary Labelle (2386427)
//\date   12 avril 2025
//Créé le 6 avril 2025

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
#include <QString>

#include <iostream>

static constexpr int MAX_ROIS = 2;
static constexpr int TAILLE_COTE_ECHIQUIER = 800;
static constexpr int TAILLE_CASE = TAILLE_COTE_ECHIQUIER / 8;
static constexpr int N_CASES_COTE = 8;
static constexpr int N_END_GAME = 3;

using namespace std;

struct EndgameConfig {
    QString texteBouton;
    int indexPartie;
};


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

    void EchiquierWidget::deplacerPieceWidget(PieceWidget* widget, Modele::Position pos)
    {
        grille_->removeWidget(widget);
        grille_->addWidget(widget, pos.y, pos.x); // Qt : ligne, colonne
    }

    void PieceWidget::surPieceDeplacee(Modele::Piece* piece, Modele::Position pos)
    {
        if (piece == pieceModele_)
        {
            emit demanderDeplacerWidget(this, pos);
        }
    }

    PieceWidget::PieceWidget(Modele::Piece* pieceModele, QWidget* parent) : QLabel(parent), pieceModele_(pieceModele)
    {
        
        setPixmap(QPixmap(getImagePathForPiece(pieceModele_)).scaled(TAILLE_CASE, TAILLE_CASE));
        setFixedSize(TAILLE_CASE, TAILLE_CASE);
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    void EchiquierWidget::reinitialiserPositions() {
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
        pieceWidgets_[piece->getPos()] = pieceWidget;

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
        vector<vector<tuple<string, int, int, bool>>> pieces = {
            {
                {"Roi", 0, 0, true}, {"Roi", 1, 4, false},
                {"Cavalier", 1, 1, true}, {"Cavalier", 5, 5, false},
                {"Tour", 0, 7, true}, {"Tour", 7, 0, false}
            },

            {
                {"Roi", 2, 2, true}, {"Roi", 6, 6, false},
                {"Cavalier", 1, 3, true}, {"Cavalier", 5, 4, false},
                {"Tour", 2, 7, true}, {"Tour", 7, 2, false}
            },

            {
                {"Roi", 4, 4, true}, {"Roi", 7, 7, false},
                {"Cavalier", 3, 5, true}, {"Cavalier", 6, 6, false},
                {"Tour", 0, 3, true}, {"Tour", 7, 1, false}
            }

        };
        vector<tuple<string, int, int, bool>> piecesActuelles = pieces[numPartie - 1];
        for (const auto& pieceData : piecesActuelles)
        {
            const auto& [nomPiece, x, y, estBlanc] = pieceData;

                if (nomPiece == "Roi")
                {
                    try
                    {
                        ajouterPiece(new Roi(Position(x, y), estBlanc));
                    }
                    catch (TropDeRoisException)
                    {
                        QMessageBox::critical(nullptr, "Erreur", "Trop de rois sont actuellement sur l'echiquier");
                    }
                }
                else if (nomPiece == "Cavalier")
                {
                    ajouterPiece(new Cavalier(Position(x, y), estBlanc));
                }
                else if (nomPiece == "Tour")
                {
                    ajouterPiece(new Tour(Position(x, y), estBlanc));
                }
        }
    }

    void EchiquierWidget::surPieceCapturee(Modele::Piece* piece)
    {
        auto widget = pieceWidgets_[piece->getPos()];
        if (widget)
        {
            grille_->removeWidget(widget);
            widget->deleteLater();
            pieceWidgets_.erase(piece->getPos());
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
                        Position pos(j, i);

                        if (!attenteDeuxiemeClic_)
                        {
                            Piece* pieceSelectionnee = ptrEchiquier_->getPiece(pos);
                            if (pieceSelectionnee && pieceSelectionnee->estBlanc() == projetJeuxEchecs_->getTourAuxBlancs())
                            {
                                caseSelectionnee_ = pos;
                                attenteDeuxiemeClic_ = true;
                            }
                        }
                        else
                        {
                            Position destination = pos;
                            pair<bool, string> messageEtDeplacement = ptrEchiquier_->deplacerPiece(caseSelectionnee_, destination);
                            if (messageEtDeplacement.first)
                            {

                                auto widget = pieceWidgets_[caseSelectionnee_];

                                if (widget)
                                {
                                    pieceWidgets_.erase(caseSelectionnee_);
                                    pieceWidgets_[destination] = widget;

                                    deplacerPieceWidget(widget, destination);

                                    connect(ptrEchiquier_, &Modele::Echiquier::pieceDeplacee,
                                        widget, &PieceWidget::surPieceDeplacee);
                                }
                                projetJeuxEchecs_->changerTour();
                            }
                            else
                            {
                                QMessageBox::critical(this, "Erreur", messageEtDeplacement.second.c_str());
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

    //Dans notre cas, il est justifié de ne pas utiliser le paramètre event de paintEvent, car nous redessinons systématiquement l'ensemble de l'échiquier 
    //sans nous soucier de la zone spécifique à mettre à jour. Le paramètre event sert surtout à optimiser le dessin en ne redessinant qu'une portion 
    //précise (event->rect()), ce qui est utile pour des interfaces complexes ou très lourdes à dessiner mais pas vraiment ici. 

    void EchiquierWidget::paintEvent([[maybe_unused]] QPaintEvent* event)
    {
        QPainter painter(this);
        if (!echiquierPixMap_.isNull()) 
        {
            painter.drawPixmap(0, 0, width(), height(), echiquierPixMap_);
        }
    }

    void ProjetJeuxEchecs::creerWidgets()
    {
        using namespace Modele;

        echiquierWidget_ = new EchiquierWidget(this, new Echiquier(), this);
        infoTourLabel_ = new QLabel("Tour des blancs", this);
        QFont labelFont = infoTourLabel_->font();
        labelFont.setPointSize(20);
        infoTourLabel_->setFont(labelFont);

        for (int i = 0; i < N_END_GAME; i++)
        {
            boutonsEndGame_.push_back(new QPushButton(QString("Endgame ") + QString::number(i+1), this));
        }
        boutonReset_ = new QPushButton("Recommencer partie", this);
    }

    void ProjetJeuxEchecs::creerLayouts()
    {
        QHBoxLayout* boutonsLayout = new QHBoxLayout;
        for (int i = 0; i < N_END_GAME; i++)
        {
            boutonsLayout->addWidget(boutonsEndGame_[i]);
        }
        boutonsLayout->addWidget(boutonReset_);

        QVBoxLayout* leftLayout = new QVBoxLayout;
        leftLayout->addLayout(boutonsLayout);
        leftLayout->addWidget(echiquierWidget_);

        QVBoxLayout* rightLayout = new QVBoxLayout;
        rightLayout->addWidget(infoTourLabel_);

        QHBoxLayout* mainLayout = new QHBoxLayout;
        mainLayout->addLayout(leftLayout);
        mainLayout->addLayout(rightLayout);

        ui->centralWidget->setLayout(mainLayout);
    }

    void ProjetJeuxEchecs::creerConnections()
    {
        for (int i = 0; i < N_END_GAME; i++)
        {
            connect(boutonsEndGame_[i], &QPushButton::clicked, this, [=]() {
                indexPartieActuelle_ = i+1;
                echiquierWidget_->chargerPartie(i+1);
                });
        }

        connect(boutonReset_, &QPushButton::clicked, this, [=]() {
            echiquierWidget_->chargerPartie(indexPartieActuelle_);
            });
    }

    ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::ProjetJeuxEchecsClass())
    {
        ui->setupUi(this);

        creerWidgets();
        creerLayouts();
        creerConnections();
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