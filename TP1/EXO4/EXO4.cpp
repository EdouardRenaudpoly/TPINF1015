#include <random>
#include <iostream>
using namespace std;

static void ErreurCin(const string& messageErreur)
{
	cin.clear();
	cin.ignore(1000, '\n');
	cout << messageErreur << endl;
}
float GenererNombreAleatoire()
{
	default_random_engine aleatoire(random_device{}());
	uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	return distribution(aleatoire);
}
static void ApproximationPi()
{
	const float valeurPi = 3.141593;
	int nDansCercle = 0;
	int nTotal = 0;
	int nIterations;
	do
	{
		cout << "Quel est le nombre d'iterations souhaite : ";
		cin >> nIterations;
		if (cin.fail() || nIterations <= 0)
		{
			ErreurCin("Le nombre d'iterations doit etre strictement positif");
		}
	} while (cin.fail() || nIterations <= 0);
	for (int i = 0;i < nIterations;i++)
	{
		float x = GenererNombreAleatoire();
		float y = GenererNombreAleatoire();
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
	ApproximationPi();
}