
// Source chatGPT

#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

static void ErreurCin(const string& messageErreur) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << messageErreur << endl;
}

static float EntreeValidee(const string& question, float borneInf, float borneSup) {
    float valeur;
    while (true) {
        cout << question;
        cin >> valeur;

        if (cin.fail() || valeur < borneInf || valeur > borneSup) 
        {
            ErreurCin("Entree invalide. Veuillez saisir une valeur valide.");
        }
        else 
        {
            return valeur;
        }
    }
}

static void CalculEmprunt() {
    float sommeArgent = EntreeValidee("Quelle est la somme d'argent qui vous a ete pretee : ", 0.01, numeric_limits<float>::infinity());
    float montantMensuel = EntreeValidee("Quel est le montant que vous allez rembourser chaque mois : ", 0.01, numeric_limits<float>::infinity());
    float tauxAnnuel = EntreeValidee("Quel est le taux d'interet annuel du preteur (entre 0 et 100) : ", 0.0, 100.0);



    cout << "Nombre de mois necessaires pour rembourser la dette : " << nombreMois << endl;
    cout << "Somme totale des interets perçus par le preteur : " << totalInterets << " $" << endl;
}

int main() 
{
    CalculEmprunt();
    return 0;
}