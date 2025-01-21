//Exercice 3


#include <iostream>
#include <math.h>
#include <limits>
using namespace std;

static void ErreurCin(const string& messageErreur)
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << messageErreur << endl;
}

static void HauteurDeRebond()
{
	int nRebonds;
	float coefficientRebond;
	float vitesseActuelle;
	float hauteurActuelle;
	const float g = 9.81;
	const string messageErreur = "Erreur : La valeur doit etre un nombre strictement positif.";
	const string messageErreurCoeficient = "Erreur : La valeur doit etre un nombre entre 0 et 1.";
	cout << "Donnez la hauteur initiale de la balle : ";
	cin >> hauteurActuelle;
	while(cin.fail() || hauteurActuelle <= 0) 
	{
		ErreurCin(messageErreur);
		cin >> hauteurActuelle;
	}
	cout << "Donnez le nombre de rebonds souhaite : ";
	cin >> nRebonds;
	while (cin.fail() || nRebonds < 0)
	{
		ErreurCin(messageErreur);
		cin >> nRebonds;
	}
	cout << "Donnez est le coefficient de rebond : ";
	cin >> coefficientRebond;
	while (cin.fail() || coefficientRebond > 1 || coefficientRebond < 0)
	{
		ErreurCin(messageErreurCoeficient);
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
	HauteurDeRebond();
	return 0;
}