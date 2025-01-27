#include <iostream>
using namespace std;
struct Etudiant {
    unsigned matricule;
    double note;
    string nomComplet;
};

Etudiant trouverPlusLongNom(const Etudiant tableau[], int nEtudiants) 
{
    Etudiant etudiantAvecPlusLongNom = tableau[0];
    for (int i = 1; i < nEtudiants; i++) 
    {
        if (tableau[i].nomComplet.length() > etudiantAvecPlusLongNom.nomComplet.length()) 
        {
            etudiantAvecPlusLongNom = tableau[i];
        }
    }
    return etudiantAvecPlusLongNom;
}

int main() {
    // Exemple de tableau d'étudiants
    Etudiant groupe[] = {
        {12345, 85.0, "Alice Dupont"},
        {67890, 92.5, "Jean-Christophe Leblanc"},
        {11223, 78.0, "Marie Curie"},
        {44556, 88.0, "Nicolas de Condorcet"}
    };

    int nEtudiants = sizeof(groupe) / sizeof(groupe[0]);  // Calculer la taille du tableau

    try {
        Etudiant resultat = trouverPlusLongNom(groupe, nEtudiants);
        cout << "Étudiant avec le plus long nom : " << endl;
        cout << "Matricule: " << resultat.matricule << endl;
        cout << "Note: " << resultat.note << endl;
        cout << "Nom complet: " << resultat.nomComplet << endl;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}