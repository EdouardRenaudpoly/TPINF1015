#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QPainter>
#include "ui_ProjetJeuxEchecs.h"
#include <iostream>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui 
{ 
	class ProjetJeuxEchecsClass; 
	class EchiquierWidget;
	class PieceWidget;
};
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Modele 
{
	class Piece;
	class Roi;
	class Cavalier;
	class Tour;
	class Echiquier;
	class TropDeRoisException;
	class MouvementTemporaire;
};
QT_END_NAMESPACE

class ProjetJeuxEchecs : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjetJeuxEchecs(QWidget *parent = nullptr);
    ~ProjetJeuxEchecs();

private:
    Ui::ProjetJeuxEchecsClass *ui;
};

class EchiquierWidget : QWidget
{
	Q_OBJECT
public:
	explicit EchiquierWidget(QWidget* parent = nullptr);
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QPixmap echiquierPixMap_;
};

class PieceWidget : QLabel
{
	Q_OBJECT
public:
	PieceWidget(Piece* pieceModele, QWidget* parent = nullptr);
private:
	Piece* pieceModele_;
};

class TropDeRoisException : public std::exception {};

class Piece : public QObject 
{
	Q_OBJECT
public:
	Piece(int x, int y, bool estBlanc) : x_(x), y_(y), estBlanc_(estBlanc){}
	virtual ~Piece() = default;

	int getX() const { return x_; }
	int getY() const { return y_; }

	void changerPosition(int newX, int newY) {
		x_ = newX;
		y_ = newY;
	}

	virtual bool estMouvementValide(int x, int y) const = 0; // Méthode virtuelle pure
protected:
	int x_;
	int y_;
	bool estBlanc_;
};

class Roi : public Piece
{
public:
	Roi(int x, int y, bool estBlanc);
	~Roi();
	bool estMouvementValide(int x, int y) const;
private:
	static int nRois;
};

class Cavalier : public Piece
{
public:
	Cavalier(int x, int y, bool estBlanc) : Piece(x,y,estBlanc){}
	bool estMouvementValide(int x, int y) const;
};

class Tour : public Piece
{
public:
	Tour(int x, int y, bool estBlanc) : Piece(x,y,estBlanc){}
	bool estMouvementValide(int x, int y) const;
};

class Echiquier : public QObject {
	Q_OBJECT
public:
	Echiquier() = default;

public slots:
	void deplacerPiece(Piece* ptrPiece, int x, int y);
	void deplacerSansVerification(Piece* ptrPiece, int x, int y);
signals:
	void pieceDeplacee(int x, int y);
private:
	std::map<std::pair<int,int>,Piece*> positionPieces_;
};

class MouvementTemporaire
{
public:
	MouvementTemporaire(Echiquier* ptrEchiquier, Piece* ptrPiece, int nouveauX, int nouveauY);
	~MouvementTemporaire();
private:
	Echiquier* ptrEchiquier_;
	Piece* ptrPiece_;
	int ancienX_;
	int ancienY_;
};
