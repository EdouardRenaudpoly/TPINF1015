#include <iostream>
using namespace std;
class Matrice
{
	public:

		Matrice(int largeur, int hauteur)
		{
			largeur_ = largeur;
			hauteur_ = hauteur;
			valeurs_ = new double [hauteur_ * largeur_];
		}
		~Matrice() { delete[] valeurs_; }
		double& accede(int ligne, int colonne)
		{
			return accede_interne(ligne, colonne);
		}
		const double& accede(int ligne, int colonne) const
		{
			return accede_interne(ligne, colonne);
		}
	private:
		double& accede_interne(int ligne, int colonne) const
		{
			return valeurs_[ligne * largeur_ + colonne];
		}
		int largeur_, hauteur_;
		double* valeurs_;
};

int main()
{
	Matrice matrice = { 30,20};
	const Matrice& m = matrice;
	// Initialise chaque valeur de la matrice à zéro:
	for (int ligne = 0; ligne < 20; ligne++)
		for (int colonne = 0; colonne < 30; colonne++)
			matrice.accede(ligne,colonne) = 4*ligne+8;
	cout << m.accede(7, 4) << endl;
}