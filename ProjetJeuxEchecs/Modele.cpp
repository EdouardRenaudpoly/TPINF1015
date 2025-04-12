#include <math.h>
#include "Modele.h"
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

    Roi::Roi(int x, int y, bool estBlanc) : Piece(x, y, estBlanc)
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
        return (abs(x - x_) == 1 && abs(y - y_) == 2) || (abs(x - x_) == 2 && abs(y - y_) == 1);
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
                if (piece->estDeplacementValide(roiX, roiY) && cheminEstLibre(piece, roiX, roiY))
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
        if (!piece || !estDeplacementLegal(piece, destX, destY) || (srcX == destX && srcY == destY))
        {
            QMessageBox::critical(nullptr, "Erreur", "Cette action n'est pas valide");
            return false;
        }

        deplacerSansVerification(piece, destX, destY);

        return true;
    }

    void Echiquier::deplacerSansVerification(Piece* ptrPiece, int x, int y)
    {
        using namespace std;
        positionPieces_.erase({ ptrPiece->getX(), ptrPiece->getY() });
        ptrPiece->changerPosition(x, y);
        if (positionPieces_[pair(x, y)])
        {
            delete positionPieces_[pair(x, y)];
        }
        positionPieces_[pair(x, y)] = ptrPiece;
    }
    MouvementTemporaire::MouvementTemporaire(Echiquier* ptrEchiquier, Piece* ptrPiece, int nouveauX, int nouveauY)
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

    void Echiquier::ajouterPiece(Piece* piece)
    {
        pair<int, int> paire = make_pair(piece->getX(), piece->getY());
        positionPieces_[paire] = piece;
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