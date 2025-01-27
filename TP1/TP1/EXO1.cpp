/**
* Programme qui détermine si un nombre est premier ou non, et si non, affiche le plus petit entier qui est est un diviseur de ce nombre.
* \file   exo1.cpp
* \author Zackary Labelle et Édouard Renaud
* \date   27 janvier 2025
* Créé le 20 janvier 2025
*/
#include <iostream>
#include <math.h>
using namespace std;

static void PremierOuNon()
{
	int nombre = 0;
	do {
		cout << "Entrer un nombre entier positif : ";
		cin >> nombre;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Entrée invalide, le nombre doit etre entier et positif." << endl;
		}
		else if (nombre <= 0)
		{
			cout << "Entrée invalide, le nombre doit etre entier et positif." << endl;
		}

	} while (cin.fail() || nombre <= 0);
	if (nombre == 2)
	{
		cout << "Ce nombre est premier.";
		return;
	}
	else if (nombre % 2 == 0)
	{
		cout << "Ce nombre n'est pas premier puisqu'il est divisible par 2.";
		return;
	}
	else if (nombre == 1)
	{
		cout << "Ce nombre n'est pas un nombre premier, il ne possede qu'un diviseur.";
		return;
	}
	else
	{
		int diviseurMaximal = sqrt(nombre);
		int diviseurActuel = 3;
		for (int diviseurActuel = 3; diviseurActuel <= diviseurMaximal; diviseurActuel += 2)
		{
			if (nombre % diviseurActuel == 0)
			{
				cout << "Ce nombre n'est pas premier puisqu'il est divisible par " << diviseurActuel << "." << endl;
				return;
			}
		}
		cout << "Ce nombre est premier.";
	}
}

int main()
{
	PremierOuNon();
}