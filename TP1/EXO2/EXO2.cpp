#include <iostream>
#include <cmath>
#include <limits>
using namespace std;

// Fonction pour afficher un message d'erreur
static void erreurCin(const string& messageErreur) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << messageErreur << endl;
}

// Fonction pour demander et valider une entrée réelle dans un intervalle
static float entreeValidee(const string& question, float borneInf, float borneSup) {
    float valeur;
    while (true) {
        cout << question;
        cin >> valeur;

        if (cin.fail() || valeur < borneInf || valeur > borneSup) 
        {
            erreurCin("Entree invalide. Veuillez saisir une valeur valide.");
        }
        else 
        {
            return valeur;
        }
    }
}

// Fonction principale pour calculer le remboursement de la dette
static void calculEmprunt() 
{
    static const int nombreMoisParAnnee = 12;
    static const int pourcenatge = 100;
    float sommeArgent = entreeValidee("Quelle est la somme d'argent qui vous a ete pretee : ", 0.01, numeric_limits<float>::infinity());
    float montantMensuel = entreeValidee("Quel est le montant que vous allez rembourser chaque mois : ", 0.01, numeric_limits<float>::infinity());
    float tauxAnnuel = entreeValidee("Quel est le taux d'interet annuel du preteur (entre 0 et 100) : ", 0.0, 100.0);

    float tauxMensuel = tauxAnnuel / nombreMoisParAnnee / pourcenatge;
    int nMois = 0;
    float totalInterets = 0.0;
    
    while (sommeArgent > 0) 
    {
        float interetMensuel = sommeArgent * tauxMensuel;
        totalInterets += interetMensuel;
        sommeArgent += interetMensuel;
        sommeArgent -= montantMensuel;
        nMois++;

        // Si le remboursement dépasse la dette, ajuster la dernière mensualité
        if (sommeArgent < 0) 
        {
            totalInterets += -sommeArgent;
            sommeArgent = 0;
        }
    }

    cout << "Nombre de mois necessaires pour rembourser la dette : " << nMois << endl;
    cout << "Somme totale des interets perçus par le preteur : " << totalInterets << " $" << endl;
}

int main() 
{
    calculEmprunt();
}