// Ce fichier contient les en-têtes du namespace et de toutes les fonctions de la partie modèle de notre projet, ces fonctions implétentent toute la logique
// derrière le jeu d'échecs et sont définies dans Modele.cpp. Il contient le roi, le cavalier et la tour ainsi que l'échiquier qui implémente les déplacements
// et la classe mouvement temporaire implémentant le RAII. Il contient également l'exception TropDeRoisException qui s'active lorsque le compteur d'instance
// de roi devient supérieur à 2.
// \file   Modele.h
// \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
// \date   12 avril 2025
// Créé le 6 avril 2025
//

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
namespace Modele
{
	struct Position {
		int x;
		int y;

		Position() : x(0), y(0) {}

		Position(int initial_x, int initial_y) : x(initial_x), y(initial_y) {}

		bool operator==(const Position& other) const 
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(const Position& other) const 
		{
			return !(*this == other);
		}

		bool operator<(const Position& other) const 
		{
			if (x < other.x) return true;
			if (other.x < x) return false;
			return y < other.y;
		}
	};
	
	class Piece
	{
	public:
		Piece(Position pos, bool estBlanc) : pos_(pos), estBlanc_(estBlanc) {}
		virtual ~Piece() = default;

		Position getPos() const
		{
			return pos_;
		}
		int getX() const { return pos_.x; }
		int getY() const { return pos_.y; }
		bool estBlanc() const { return estBlanc_; }

		void changerPosition(Position nouvellePos) 
		{
			pos_ = nouvellePos;
		}

		virtual bool estDeplacementValide(Position pos) const = 0; // Méthode virtuelle pure
	protected:
		Position pos_;
		bool estBlanc_;
	};

	class Roi : public Piece
	{
	public:
		Roi(Position pos, bool estBlanc);
		~Roi();
		bool estDeplacementValide(Position pos) const override;
	private:
		static int nRois;
	};

	class Cavalier : public Piece
	{
	public:
		Cavalier(Position pos, bool estBlanc) : Piece(pos, estBlanc) {}
		bool estDeplacementValide(Position pos) const override;
	};

	class Tour : public Piece
	{
	public:
		Tour(Position pos, bool estBlanc) : Piece(pos, estBlanc) {}
		bool estDeplacementValide(Position pos) const override;
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
		pair<bool,string> estDeplacementLegal(Piece* piece, Position posDestination);
		bool cheminEstLibre(Piece* piece, Position posDestination);
		pair<bool,string> deplacerPiece(Position posSource, Position posDestination);
		void deplacerSansVerification(Piece* ptrPiece, Position posDeplacement,bool mouvementTemp);
		void ajouterPiece(Piece* piece);
		void enleverPieces();
		Piece* getPiece(Position pos)
		{
			return positionPieces_[pos];
		}
		void annulerCapture()
		{
			positionPieces_[pieceTemporairementDelete_->getPos()] = pieceTemporairementDelete_;
		}
		Piece* getPieceTemp()
		{
			return pieceTemporairementDelete_;
		}
	signals:
		void pieceDeplacee(Piece* piece, Position nouvellePos);
		void pieceCapturee(Piece* piece);
	private:
		map<Position, Piece*> positionPieces_;
		Piece* pieceTemporairementDelete_ = nullptr;
	};

	class MouvementTemporaire
	{
	public:
		MouvementTemporaire(Echiquier* ptrEchiquier, Piece* ptrPiece, Position nouvellePos);
		~MouvementTemporaire();
	private:
		Echiquier* ptrEchiquier_;
		Piece* ptrPiece_;
		Position anciennePos_;
	};

	class TropDeRoisException : public std::exception {};

};
QT_END_NAMESPACE