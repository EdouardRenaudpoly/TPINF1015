// TP1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include <iostream>
#include <math.h>
using namespace std;

void PremierOuNon()
{
    int nombre;
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


// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
