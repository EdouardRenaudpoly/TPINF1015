#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include "ui_ProjetJeuxEchecs.h"
#include "Modele.h"
#include <iostream>
#include <map>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
	class EchiquierWidget;

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

	class PieceWidget : public QLabel
	{
		Q_OBJECT
	public:
		PieceWidget(Modele::Piece* pieceModele, QWidget* parent = nullptr);
	public slots:
		void surPieceDeplacee(Modele::Piece* piece, int x, int y);
	signals:
		void demanderDeplacerWidget(PieceWidget* pieceWidget, int x, int y);
	private:
		Modele::Piece* pieceModele_;
	};

	class EchiquierWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit EchiquierWidget(QWidget* parent = nullptr, Modele::Echiquier* ptrEchiquier = nullptr, ProjetJeuxEchecs* projetJeuxEchecs = nullptr);
		void chargerPartie(int numPartie);
		void ajouterPiece(Modele::Piece* piece);
		void deplacerPieceWidget(PieceWidget* widget, int x, int y);
		void reinitialiserPositions();
		//void mettreAJour();
		~EchiquierWidget()
		{
			delete ptrEchiquier_;
		}
	private slots:
		void surPieceCapturee(Modele::Piece* piece);
	protected:
		void paintEvent(QPaintEvent* event) override;
	private:
		QPixmap echiquierPixMap_;
		QMap<QPair<int, int>, PieceWidget*> pieceWidgets_;
		Modele::Echiquier* ptrEchiquier_;
		QGridLayout* grille_;
		QPoint caseSelectionnee_;
		ProjetJeuxEchecs* projetJeuxEchecs_;
		bool attenteDeuxiemeClic_ = false;
	};
};
QT_END_NAMESPACE
