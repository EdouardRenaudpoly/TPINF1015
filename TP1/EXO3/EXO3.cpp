//


#include <iostream>
#include <math.h>
using namespace std;

void HauteurDeRebond()
{
	int nbRebonds;
	float coefficientRebond;
	int vitesseActuelle;
	float hauteurActuelle;
	const float g = 9.81;
	cout << "Donnez la hauteur initiale de la balle : ";
	cin >> hauteurActuelle;
	cout << "Donnez le nombre de rebonds souhaite : ";
	cin >> nbRebonds;
	cout << "Donnez est le coefficient de rebond : ";
	cin >> coefficientRebond;
	for (int i = 0;i < nbRebonds;i++)
	{
		vitesseActuelle = coefficientRebond * sqrt(2 * g * hauteurActuelle);
		hauteurActuelle = pow(vitesseActuelle, 2) / (2 * g);
	}
	cout << "La hauteur finale de la balle apres " << nbRebonds << " rebond(s) est " << hauteurActuelle;
}
int main()
{
	HauteurDeRebond();
}