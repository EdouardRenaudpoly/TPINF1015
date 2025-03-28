/**
* Programme qui d�termine la hauteur atteinte par une balle en tenant compte de la hauteur initiale et du nombre de rebonds.
* \file   exo3.cpp
* \author Zackary Labelle et �douard Renaud
* \date   27 janvier 2025
* Cr�� le 20 janvier 2025
*/
#include <iostream>
#include <limits>
using namespace std;

static void erreurCin(const string& messageErreur)
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << messageErreur << endl;
}

static void hauteurDeRebond()
{
	int nRebonds = 0;
	float coefficientRebond = 0.0;
	float vitesseActuelle = 0.0;
	float hauteurActuelle = 0.0;
	const float g = 9.81;
	const string messageErreur = "Erreur : La valeur doit etre un nombre strictement positif.";
	const string messageErreurCoeficient = "Erreur : La valeur doit etre un nombre entre 0 et 1.";
	cout << "Donnez la hauteur initiale de la balle : ";
	cin >> hauteurActuelle;
	while (cin.fail() || hauteurActuelle <= 0) 
	{
		erreurCin(messageErreur);
		cin >> hauteurActuelle;
	}
	cout << "Donnez le nombre de rebonds souhaite : ";
	cin >> nRebonds;
	while (cin.fail() || nRebonds < 0)
	{
		erreurCin(messageErreur);
		cin >> nRebonds;
	}
	cout << "Donnez est le coefficient de rebond : ";
	cin >> coefficientRebond;
	while (cin.fail() || coefficientRebond > 1 || coefficientRebond < 0)
	{
		erreurCin(messageErreurCoeficient);
		cin >> coefficientRebond;
	}
	for (int i = 0; i < nRebonds; ++i)
	{
		vitesseActuelle = coefficientRebond * sqrt(2 * g * hauteurActuelle);
		hauteurActuelle = pow(vitesseActuelle, 2) / (2 * g);
	}
	cout << "La hauteur finale de la balle apres " << nRebonds << " rebond(s) est " << hauteurActuelle;
}
int main()
{
	hauteurDeRebond();
}