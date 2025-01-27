/**
* Programme de traitement de données qui crée un tableau de structures et y place le dictionnaire à partir du fichier .txt pour finalement afficher le mot le plus long de celui-ci
* \file   exo6.cpp
* \author Édouard Renaud et Zackary Labelle
* \date   27 janvier 2025
* Créé le 20 janvier 2025
*/

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct MotDictionnaire {
	string mot;
	string nature;
	string definition;
};

static void AfficherMotPlusLong(MotDictionnaire dictionnaire[], int nMots)
{
	MotDictionnaire motPlusLong = dictionnaire[0];
	for (int i = 1;i < nMots;i++)
	{
		if (dictionnaire[i].mot.length() > motPlusLong.mot.length())
		{
			motPlusLong = dictionnaire[i];
		}
	}
	cout << motPlusLong.mot << " (" << motPlusLong.nature << ") : " << motPlusLong.definition << endl;
}

static void GestionDictionnaire()
{
	const int nMots = 4;
	MotDictionnaire dictionnaire[nMots];
	ifstream fichier("dictionnaire.txt");
	string ligneActuelle;
	int indiceLigneActuelle = 0;
	while (getline(fichier, ligneActuelle)) //code modifié à partir de Stack Overflow : https://stackoverflow.com/questions/20756968/reading-multiple-lines-from-a-file-using-getline
	{
		istringstream flux(ligneActuelle);
		MotDictionnaire motActuel;
		getline(flux, motActuel.mot, '\t');
		getline(flux, motActuel.nature, '\t');
		getline(flux, motActuel.definition, '\t');
		dictionnaire[indiceLigneActuelle] = motActuel;
		indiceLigneActuelle++;
	}
	AfficherMotPlusLong(dictionnaire,nMots);
	fichier.close();
}

int main()
{
	GestionDictionnaire();
}