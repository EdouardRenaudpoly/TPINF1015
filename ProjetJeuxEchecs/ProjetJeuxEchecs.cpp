#include "ProjetJeuxEchecs.h"
#include <math.h>
#include <QMessageBox>
#include <QString>
static constexpr int MAX_ROIS = 2;


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

Roi::Roi(int x, int y,bool estBlanc) : x_(x), y_(y), estBlanc_(estBlanc)
{
    if (nRois >= MAX_ROIS)
    {
        throw new TropDeRoisException;
    }
    nRois++;
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

    }
    else
    {
        QMessageBox::critical(nullptr, "Erreur", "Trop de rois sont actuellement sur l'échiquier");
    }
}
