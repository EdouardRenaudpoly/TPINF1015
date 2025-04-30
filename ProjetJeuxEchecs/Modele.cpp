//﻿ Ce fichier a pour but d'implémenter un namespace et des classes contenant toute la logique derriere le jeu d'échecs, soit le mouvement des pieces, 
// voir si le roi est en échec, voir si les coups sont légaux, etc. Le programme contient donc les pièces suivantes : le roi, la tour et le cavalier.
// Il contient également une classe Échiquier et c'est elle qui s'occupe de déplacer les pièces et voir si les mouvements sont valides. Finalement,
// la classe mouvement temporaire implémente un déplacement qui s'annule directement après que la classe soit détruite, ce qui est utile lors de situations
// d'échec du roi.
// \file   Modele.cpp
// \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
// \date   12 avril 2025
// Créé le 6 avril 2025
//

#include <math.h>
#include "Modele.h"
#include <string>
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
namespace Modele
{

    Roi::Roi(Position pos, bool estBlanc) : Piece(pos, estBlanc)
    {
        if (nRois >= MAX_ROIS)
        {
            throw TropDeRoisException();
        }
        nRois++;
    }
    Roi::~Roi()
    {
        nRois--;
    }
    int Roi::nRois = 0;

    bool Roi::estDeplacementValide(Position pos) const
    {
        return abs(pos.x - pos_.x) <= 1 && abs(pos.y - pos_.y) <= 1;
    }
    bool Cavalier::estDeplacementValide(Position pos) const
    {
        return (abs(pos.x - pos_.x) == 1 && abs(pos.y - pos_.y) == 2) || (abs(pos.x - pos_.x) == 2 && abs(pos.y - pos_.y) == 1);
    }
    bool Tour::estDeplacementValide(Position pos) const
    {
        return pos_.x == pos.x || pos_.y == pos.y;
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

        Position posRoi = roi->getPos();

        for (const auto& [pos, piece] : positionPieces_)
        {
            if (piece && piece->estBlanc() != blanc)
            {
                if (piece->estDeplacementValide(posRoi) && cheminEstLibre(piece, posRoi))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool Echiquier::cheminEstLibre(Piece* piece, Position posDestination)
    {
        Position posSource = piece->getPos();

        if (auto* tour = dynamic_cast<Tour*>(piece))
        {
            if (posSource.y == posDestination.y)
            {
                int minX = min(posSource.x, posDestination.x) + 1;
                int maxX = max(posSource.x, posDestination.x);

                for (int x = minX; x < maxX; ++x)
                {
                    if (getPiece(Position(x, posSource.y)))
                    {
                        return false;
                    }
                }

            }
            else
            {
                int minY = min(posSource.y, posDestination.y) + 1;
                int maxY = max(posSource.y, posDestination.y);

                for (int y = minY; y < maxY; ++y)
                {
                    if (getPiece(Position(posSource.x, y)))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    pair<bool,string> Echiquier::estDeplacementLegal(Piece* piece, Position posDestination)
    {
        if (!piece->estDeplacementValide(posDestination))
        {
            return make_pair(false,"Cette piece ne peut pas se deplacer ainsi");
        }

        Piece* cible = getPiece(posDestination);

        if ((cible && cible->estBlanc() == piece->estBlanc())
            || !cheminEstLibre(piece, posDestination))
        {
            return make_pair(false,"cette piece ne peut pas passer au-dessus des autres");
        }

        MouvementTemporaire mouvementTemp(this, piece, posDestination);

        return make_pair(!roiEnEchec(piece->estBlanc()),"Votre roi est ou serait en situation d'echec");
    }

    pair<bool,string> Echiquier::deplacerPiece(Position posSource, Position posDestination)
    {
        Piece* piece = getPiece(posSource);
        if (!piece || (posDestination == posSource))
        {
            return make_pair(false,"Vous ne pouvez pas deplacer la piece sur elle-meme");
        }

        pair<bool, string> deplacementValideEtMessage = estDeplacementLegal(piece, posDestination);
        if (!deplacementValideEtMessage.first)
        {
            return deplacementValideEtMessage;
        }

        deplacerSansVerification(piece, posDestination,false);

        emit pieceDeplacee(piece, posDestination);

        return make_pair(true,"");
    }

    void Echiquier::deplacerSansVerification(Piece* ptrPiece, Position pos,bool mouvementTemp)
    {
        using namespace std;
        positionPieces_.erase(ptrPiece->getPos());
        ptrPiece->changerPosition(pos);
        if (positionPieces_[pos])
        {
            if (mouvementTemp)
            {
                pieceTemporairementDelete_ = positionPieces_[pos];
            }
            else
            {
                pieceTemporairementDelete_ = nullptr;
                emit pieceCapturee(positionPieces_[pos]);
                delete positionPieces_[pos];
            }
        }
        positionPieces_[pos] = ptrPiece;
    }
    MouvementTemporaire::MouvementTemporaire(Echiquier* ptrEchiquier, Piece* ptrPiece, Position nouvellePos)
    {
        ptrEchiquier_ = ptrEchiquier;
        ptrPiece_ = ptrPiece;
        anciennePos_ = ptrPiece_->getPos();
        ptrEchiquier_->deplacerSansVerification(ptrPiece_, nouvellePos,true);
    }
    MouvementTemporaire::~MouvementTemporaire()
    {
        ptrEchiquier_->deplacerSansVerification(ptrPiece_, anciennePos_,true);
        if (ptrEchiquier_->getPieceTemp())
        {
            ptrEchiquier_->annulerCapture();

        }
    }

    void Echiquier::ajouterPiece(Piece* piece)
    {
        positionPieces_[piece->getPos()] = piece;
    }

    void Echiquier::enleverPieces()
    {
        for (auto& paire : positionPieces_)
        {
            delete paire.second;
        }

        positionPieces_.clear();
    }

}
QT_END_NAMESPACE