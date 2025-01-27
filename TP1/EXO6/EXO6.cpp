#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct MotDictionnaire 
{
	string mot;
	string nature;
	string definition;
};

static void afficherMotPlusLong(MotDictionnaire dictionnaire[], int nMots)
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

static void gererDictionnaire()
{
	const int nMots = 4;
	MotDictionnaire dictionnaire[nMots];
	ifstream fichier("dictionnaire.txt");
	string ligneActuelle;
	int indiceLigneActuelle = 0;
	while (getline(fichier, ligneActuelle)) 
	{
		istringstream flux(ligneActuelle);
		MotDictionnaire motActuel;
		getline(flux, motActuel.mot, '\t');
		getline(flux, motActuel.nature, '\t');
		getline(flux, motActuel.definition, '\t');
		dictionnaire[indiceLigneActuelle] = motActuel;
		indiceLigneActuelle++;
	}
	afficherMotPlusLong(dictionnaire,nMots);
	fichier.close();
}

int main()
{
	gererDictionnaire();
}