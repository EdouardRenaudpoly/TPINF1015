﻿// Ce fichier contient les en-têtes du namespace et de toutes les fonctions de la partie modèle de notre projet, ces fonctions implétentent toute la logique
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
		Cavalier(int x, int y, bool estBlanc) : Piece(x, y, estBlanc) {}
		bool estDeplacementValide(int x, int y) const override;
	};

	class Tour : public Piece
	{
	public:
		Tour(int x, int y, bool estBlanc) : Piece(x, y, estBlanc) {}
		bool estDeplacementValide(int x, int y) const override;
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
		pair<bool,string> estDeplacementLegal(Piece* piece, int destX, int destY);
		bool cheminEstLibre(Piece* piece, int destX, int destY);
		pair<bool,string> deplacerPiece(int srcX, int srcY, int destX, int destY);
		void deplacerSansVerification(Piece* ptrPiece, int x, int y);
		void ajouterPiece(Piece* piece);
		void enleverPieces();
		Piece* getPiece(int x, int y)
		{
			return positionPieces_[make_pair(x, y)];
		}
	signals:
		void pieceDeplacee(Piece* piece, int nouveauX, int nouveauY);
		void pieceCapturee(Piece* piece);
	private:
		map<pair<int, int>, Piece*> positionPieces_;
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

	class TropDeRoisException : public std::exception {};

};
QT_END_NAMESPACE