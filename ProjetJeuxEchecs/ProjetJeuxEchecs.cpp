#include "ProjetJeuxEchecs.h"
#include <math.h>
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

Roi::Roi(int x, int y)
{
    x_ = x;
    y_ = y;
    if (nRois >= MAX_ROIS)
    {
        throw new TropDeRoisException;
    }
}
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
