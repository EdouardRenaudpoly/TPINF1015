/**
* Programme qui demande le nombre d'it�rations souhait� par l'utilisateur et retourne la valeur approxim�e de pi selon la m�thode de Monte-Carlo
* \file   exo4.cpp
* \author �douard Renaud et Zackary Labelle
* \date   27 janvier 2025
* Cr�� le 20 janvier 2025
*/
#include <random>
#include <iostream>
using namespace std;

static void erreurCin(const string& messageErreur)
{
	cin.clear();
	cin.ignore(1000, '\n');
	cout << messageErreur << endl;
}
static float genererNombreAleatoire()
{
	default_random_engine aleatoire(random_device{}());
	uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	return distribution(aleatoire);
}
static void approximationPi()
{
	const float valeurPi = 3.141593;
	int nDansCercle = 0;
	int nTotal = 0;
	int nIterations = 0;
	do
	{
		cout << "Quel est le nombre d'iterations souhaite : ";
		cin >> nIterations;
		if (cin.fail() || nIterations <= 0)
		{
			erreurCin("Le nombre d'iterations doit etre strictement positif");
		}
	} while (cin.fail() || nIterations <= 0);
	for (int i = 0;i < nIterations;i++)
	{
		float x = genererNombreAleatoire();
		float y = genererNombreAleatoire();
		if ((pow(x,2)+pow(y,2)) < 1)
		{
			nDansCercle++;
		}
		nTotal++;
	}
	float approximationPi = (static_cast<float>(nDansCercle) / nTotal) * 4;
	cout << "La valeur de pi que nous avons approxime est : " << approximationPi << endl;
	float ecartRelatif = abs((approximationPi-valeurPi) / valeurPi) * 100;
	cout << "L'ecart relatif est de : " << ecartRelatif;
}

int main()
{
	approximationPi();
}