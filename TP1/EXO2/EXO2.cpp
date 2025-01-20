//


#include <iostream>
#include <math.h>
using namespace std;

static void ErreurCin(string messageErreur)
{
	cin.clear();
	cin.ignore(1000, '\n');
	cout << messageErreur << endl;
}
static void QuestionMontant(string question, float montant)
{
	cin.ignore();
	cout << question << endl;
	cin >> montant;
}
static void CalculEmprunt()
{
	float sommeArgent = 0.0;
	float montantMensuel = 0.0;
	float tauxAnnuel = 0.0;
	string messageErreur = "Entree invalide, le montant d'argent doit etre entier et positif.";

	do {
		QuestionMontant("Quelle est la somme d'argent qui vous a ete pretee : ", sommeArgent);

		if (cin.fail() || sommeArgent <= 0)
		{
			ErreurCin(messageErreur);
			continue;
		}
		QuestionMontant("Quel est le montant que vous allez rembourser chaque mois : ", montantMensuel);

		if (cin.fail() || montantMensuel <= 0)
		{
			ErreurCin(messageErreur);
			continue;
		}
		QuestionMontant("Quel est le taux d'interet annuel du preteur : ", tauxAnnuel);

		if (cin.fail() || tauxAnnuel < 0 || tauxAnnuel >100)
		{
			ErreurCin(messageErreur);
		}

	} while (cin.fail() || sommeArgent <= 0 || montantMensuel <= 0 || tauxAnnuel < 0 || tauxAnnuel > 100);
}
int main() 
{
	CalculEmprunt();
}