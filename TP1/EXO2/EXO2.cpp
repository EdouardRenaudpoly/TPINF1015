/**
* Programme qui d�termine le nombre de mois n�cessaires pour rembourser une dette ainsi que la somme des int�r�ts per�us par le pr�teur.
* \file   exo2.cpp
* \author Zackary Labelle et �douard Renaud
* \date   27 janvier 2025
* Cr�� le 20 janvier 2025
*/
#include <iostream>
#include <cmath>
#include <limits>
using namespace std;

static void erreurCin(const string& messageErreur) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << messageErreur << endl;
}

static float EntreeValidee(const string& question, float borneInf, float borneSup) {
    float valeur = 0.0;
    bool estFini = false;
    while (!estFini) 
    {
        cout << question;
        cin >> valeur;

        if (cin.fail() || valeur < borneInf || valeur > borneSup) 
        {
            erreurCin("Entree invalide. Veuillez saisir une valeur valide.");
        }
        else 
        {
           estFini = true;
        }
    }
    return valeur;
}

static void calculEmprunt() 
{
    static const int nombreMois = 12;
    static const int pourcentage = 100;
    float sommeArgent = EntreeValidee("Quelle est la somme d'argent qui vous a ete pretee : ", 0.01, numeric_limits<float>::infinity());
    float montantMensuel = EntreeValidee("Quel est le montant que vous allez rembourser chaque mois : ", 0.01, numeric_limits<float>::infinity());
    float tauxAnnuel = EntreeValidee("Quel est le taux d'interet annuel du preteur (entre 0 et 100) : ", 0.0, 100.0);

    float tauxMensuel = tauxAnnuel / nombreMois / pourcentage;

    int nMois = 0;
    float totalInterets = 0.0;

    while (sommeArgent > 0) 
    {
        float interetMensuel = sommeArgent * tauxMensuel;
        totalInterets += interetMensuel;
        sommeArgent += interetMensuel;
        sommeArgent -= montantMensuel;
        ++nMois;

        
        if (sommeArgent < 0) // Si le remboursement d�passe la dette, ajuster la derni�re mensualit�
        {
            totalInterets += -sommeArgent;
            sommeArgent = 0;
        }
    }

    cout << "Nombre de mois necessaires pour rembourser la dette : " << nMois << endl;
    cout << "Somme totale des interets per�us par le preteur : " << totalInterets << " $" << endl;
}

int main() 
{
    calculEmprunt();
}