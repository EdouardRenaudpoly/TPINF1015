// TP1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include <iostream>
#include <math.h>
using namespace std;

void PremierOuNon()
{
    int nombre=0;
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
void CalculEmprunt()
{
    float sommeArgent=0.0;
    float montantMensuel=0.0;
    float tauxAnnuel=0.0;
    do {
        cout << "Quelle est la somme d'argent qui vous a ete pretee : " << endl;
        cin >> sommeArgent;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entree invalide, la somme d'argent doit etre un nombre." << endl;
            continue;
        }
        else if (sommeArgent <= 0)
        {
            cout << "Entree invalide, la somme d'argent doit etre entiere et positive." << endl;
            continue;
        }
        cin.ignore();
        cout << "Quel est le montant que vous allez rembourser chaque mois : " << endl;
        cin >> montantMensuel;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entree invalide, le montant d'argent doit etre un nombre." << endl;
            continue;
        }
        else if (montantMensuel <= 0)
        {
            cout << "Entree invalide, le montant d'argent doit etre entier et positif." << endl;
            continue;
        }
        cin.ignore();
        cout << "Quel est le taux d'interet annuel du preteur : " << endl;
        cin >> tauxAnnuel;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entree invalide, le taux doit etre un nombre." << endl;
            continue;
        }
        else if (tauxAnnuel < 0 || tauxAnnuel >100)
        {
            cout << "Entree invalide, le montant d'argent doit etre entier et positif." << endl;
            continue;
        }

    } while (cin.fail() || sommeArgent <= 0 || montantMensuel <= 0 || tauxAnnuel < 0 || tauxAnnuel > 100);
}
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
        hauteurActuelle = pow(vitesseActuelle, 2)/(2*g);
    }
    cout << "La hauteur finale de la balle apres " << nbRebonds << " rebond(s) est " << hauteurActuelle;
}
int main()
{
    //PremierOuNon();
    //CalculEmprunt();
    HauteurDeRebond();
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
