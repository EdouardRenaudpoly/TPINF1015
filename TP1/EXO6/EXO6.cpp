#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct MotDictionnaire {
	std::string mot;
	std::string nature;
	std::string definition;
};

void GestionDictionnaire()
{
	const int nMots = 4;
	MotDictionnaire dictionnaire[nMots];
	ifstream fichier("dictionnaire.txt");
	string ligneActuelle;
	int indiceLigneActuelle = 0;
	while (getline(fichier, ligneActuelle)) { // Lit le fichier ligne par ligne
		std::cout << ligneActuelle << std::endl;    // Affiche chaque ligne lue
	}
	fichier.close();
}

int main()
{
	GestionDictionnaire();
}