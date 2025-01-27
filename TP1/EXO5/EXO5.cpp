// Exercice 5 du TP1

#include <iostream>
#include <math.h>
#include <limits>
#include <vector>
#include "EXO5.h"
using namespace std;

static void erreurCin(const string& messageErreur)
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << messageErreur;
}
static bool estPair(int valeur) 
{
	return valeur % 2 == 0;
}
static void CreerTableau(int tableauDebut[], int grosseurTableau)
{
	const string message = "Veuillez entrer un nombre positif : ";
	const string messageErreur = "Erreur : Veuillez entrer un nombre posistif : ";

	for (int i = 0; i < grosseurTableau; ++i)
	{
		cout << message;
		cin >> tableauDebut[i];
		while (cin.fail() || tableauDebut[i] < 0)
		{
			erreurCin(messageErreur);
			cin >> tableauDebut[i];
		}
	}
}
static void AfficherTableau(int tableau[])
{
	for (int i = 0; i < 10;++i)
	{
		if (i == 0)
			cout << "{" << tableau[i] << ", ";
		else if (i == 10 - 1)
			cout << tableau[i] << "}" << endl;
		else
			cout << tableau[i] << ", ";
	}
}
static void PlacerValeurs(int tableauDebut[], int tableauFin[], int grosseurTableau)
{
	int compteur = 0;

	for (int i = 0;i < grosseurTableau; ++i)
	{
		if (estPair(tableauDebut[i]))
			tableauFin[compteur++] = tableauDebut[i];
	}
	for (int i = 0; i < grosseurTableau; ++i)
	{
		if (!estPair(tableauDebut[i]))
			tableauFin[compteur++] = tableauDebut[i];
	}
}

static void TrierTableau() 
{
	const int grosseurTableau = 10;
	int tableauDebut[grosseurTableau] = {};
	int tableauFin[grosseurTableau] = {};

	CreerTableau(tableauDebut, grosseurTableau);
	PlacerValeurs(tableauDebut, tableauFin, grosseurTableau);

	cout << "Tableau avant le triage : ";
	AfficherTableau(tableauDebut);
	
	cout << "Tableau après le triage : ";
	AfficherTableau(tableauFin);
}

int main()
{
	TrierTableau();
	return 0;
}