#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include "ui_ProjetJeuxEchecs.h"
#include <iostream>
#include <map>

using namespace std;

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

class ProjetJeuxEchecs;

class Piece : public QObject
{
	Q_OBJECT
public:
	Piece(int x, int y, bool estBlanc) : x_(x), y_(y), estBlanc_(estBlanc) {}
	virtual ~Piece() = default;

	int getX() const { return x_; }
	int getY() const { return y_; }
	bool estBlanc() const { return estBlanc_; }

	void changerPosition(int newX, int newY) {
		x_ = newX;
		y_ = newY;
	}

	virtual bool estDeplacementValide(int x, int y) const = 0; // Méthode virtuelle pure
protected:
	int x_;
	int y_;
	bool estBlanc_;
};

class PieceWidget : public QLabel
{
	Q_OBJECT
public:
	PieceWidget(Piece* pieceModele, QWidget* parent = nullptr);
private:
	Piece* pieceModele_;
};

class Echiquier : public QObject {
	Q_OBJECT
public:
	Echiquier() = default;
	~Echiquier()
	{
		for (auto& paire : positionPieces_)
		{
			delete paire.second;
		}

		positionPieces_.clear();
	}
	bool roiEnEchec(bool blanc);
	bool estDeplacementLegal(Piece* piece, int destX, int destY);
	bool cheminEstLibre(Piece* piece, int destX, int destY);
public slots:
	bool deplacerPiece(int srcX, int srcY, int destX, int destY);
	void deplacerSansVerification(Piece* ptrPiece, int x, int y);
	void ajouterPiece(Piece* piece);
	void enleverPieces();
	Piece* getPiece(int x, int y) 
	{
		return positionPieces_[make_pair(x,y)];
	}
signals:
	void pieceDeplacee(int x, int y);
private:
	map<pair<int, int>, Piece*> positionPieces_;
};

class EchiquierWidget : public QWidget
{
	Q_OBJECT
public:
	explicit EchiquierWidget(QWidget* parent = nullptr, Echiquier* ptrEchiquier=nullptr, ProjetJeuxEchecs* projetJeuxEchecs=nullptr);
	void chargerPartie(int numPartie);
	void ajouterPiece(Piece* piece);
	void reset();
	void mettreAJour();
	~EchiquierWidget()
	{
		delete ptrEchiquier_;
	}
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	QPixmap echiquierPixMap_;
	QMap<QPair<int, int>, PieceWidget*> pieceWidgets_;
	Echiquier* ptrEchiquier_;
	QGridLayout* grille_;
	QPoint caseSelectionnee_; 
	ProjetJeuxEchecs* projetJeuxEchecs_;
	bool attenteDeuxiemeClic_ = false;
};

class ProjetJeuxEchecs : public QMainWindow
{
	Q_OBJECT

public:
	explicit ProjetJeuxEchecs(QWidget* parent = nullptr);
	bool getTourAuxBlancs()
	{
		return tourAuxBlancs_;
	}
	void changerTour();
	~ProjetJeuxEchecs();

private:
	Ui::ProjetJeuxEchecsClass* ui;
	EchiquierWidget* echiquierWidget_;
	QLabel* infoTourLabel_;
	QPushButton* boutonEndgame1_;
	QPushButton* boutonEndgame2_;
	QPushButton* boutonEndgame3_;
	QPushButton* boutonReset_;
	bool tourAuxBlancs_ = true;
	int indexPartieActuelle = 0;
};

class TropDeRoisException : public std::exception {};

class Roi : public Piece
{
public:
	Roi(int x, int y, bool estBlanc);
	~Roi();
	bool estDeplacementValide(int x, int y) const override;
private:
	static int nRois;
};

class Cavalier : public Piece
{
public:
	Cavalier(int x, int y, bool estBlanc) : Piece(x,y,estBlanc){}
	bool estDeplacementValide(int x, int y) const override;
};

class Tour : public Piece
{
public:
	Tour(int x, int y, bool estBlanc) : Piece(x,y,estBlanc){}
	bool estDeplacementValide(int x, int y) const override;
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

