
// Source chatGPT

#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

// Fonction pour afficher un message d'erreur
static void ErreurCin(const string& messageErreur) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << messageErreur << endl;
}

// Fonction pour demander et valider une entr�e r�elle dans un intervalle
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

// Fonction principale pour calculer le remboursement de la dette
static void CalculEmprunt() {
    // Variables
    float sommeArgent = EntreeValidee("Quelle est la somme d'argent qui vous a ete pretee : ", 0.01, numeric_limits<float>::infinity());
    float montantMensuel = EntreeValidee("Quel est le montant que vous allez rembourser chaque mois : ", 0.01, numeric_limits<float>::infinity());
    float tauxAnnuel = EntreeValidee("Quel est le taux d'interet annuel du preteur (entre 0 et 100) : ", 0.0, 100.0);

    // Calcul du taux mensuel
    float tauxMensuel = tauxAnnuel / 12 / 100;

    // Variables de suivi
    int nombreMois = 0;
    float totalInterets = 0.0;

    // Simulation du remboursement mois par mois
    while (sommeArgent > 0) 
    {
        float interetMensuel = sommeArgent * tauxMensuel;  // Int�r�t pour le mois courant
        totalInterets += interetMensuel;                  // Ajouter � la somme des int�r�ts
        sommeArgent += interetMensuel;                   // Ajouter l'int�r�t � la dette
        sommeArgent -= montantMensuel;                   // Rembourser la mensualit�
        nombreMois++;

        // Si le remboursement d�passe la dette, ajuster la derni�re mensualit�
        if (sommeArgent < 0) 
        {
            totalInterets += sommeArgent; // R�duire les int�r�ts si le dernier paiement couvre plus
            sommeArgent = 0;
        }
    }

    // Affichage des r�sultats
    cout << "Nombre de mois necessaires pour rembourser la dette : " << nombreMois << endl;
    cout << "Somme totale des interets per�us par le preteur : " << totalInterets << " $" << endl;
}

int main() 
{
    CalculEmprunt();
    return 0;
}