#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProjetJeuxEchecs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
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
};
QT_END_NAMESPACE

class ProjetJeuxEchecs : public QMainWindow
{
    Q_OBJECT

public:
    ProjetJeuxEchecs(QWidget *parent = nullptr);
    ~ProjetJeuxEchecs();

private:
    Ui::ProjetJeuxEchecsClass *ui;
};

class TropDeRoisException : public std::exception {};

class Piece : public QObject {
	Q_OBJECT

public:
	Piece() = default;
	virtual ~Piece() = default;

	virtual bool estMouvementValide(int x, int y) const = 0;
};

class Roi : public Piece
{
public:
	Roi(int x, int y);
	bool estMouvementValide(int x, int y) const;
private:
	static int nRois;
	int x_;
	int y_;
};

class Cavalier : public Piece
{
public:
	Cavalier(int x, int y) : x_(x), y_(y){}
	bool estMouvementValide(int x, int y) const;
private:
	int x_;
	int y_;
};

class Tour : public Piece
{
public:
	Tour(int x, int y) : x_(x), y_(y){}
	bool estMouvementValide(int x, int y) const;
private:
	int x_;
	int y_;
};

class Echiquier : public QObject {
	Q_OBJECT
public:
	Echiquier() = default;

public slots:
	void deplacerPiece(Piece* ptrPiece, int x, int y);
signals:
	void pieceDeplacee(int x, int y);
};
