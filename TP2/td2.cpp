#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <span>

#include "cppitertools/range.hpp"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
    UInt8 valeur = 0;
    fichier.read((char*)&valeur, sizeof(valeur));
    return valeur;
}
UInt16 lireUint16(istream& fichier)
{
    UInt16 valeur = 0;
    fichier.read((char*)&valeur, sizeof(valeur));
    return valeur;
}
string lireString(istream& fichier)
{
    string texte;
    texte.resize(lireUint16(fichier));
    fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
    return texte;
}

#pragma endregion//}

// Fonction pour créer un span pour une ListeFilms
span<Film*> creerSpanListeFilms(Film** elements, size_t nElements) {
    return span<Film*>(elements, nElements);
}

// Fonction pour créer un span pour une liste d'acteurs
span<Acteur*> creerSpanListeActeurs(Acteur** elements, size_t nElements) {
    return span<Acteur*>(elements, nElements);
}

// Constructeurs de la classe ListeFilms
ListeFilms::ListeFilms() // Constructeur de base
{
    capacite_ = 0;
    nElements_ = 0;
    elements_ = nullptr;
}

ListeFilms::ListeFilms(string nomFichier)
    : ListeFilms()
{
    ifstream fichier(nomFichier, ios::binary);
    fichier.exceptions(ios::failbit);

    int nElements = lireUint16(fichier);

    for ([[maybe_unused]] int i : range(0, nElements))
    {
        ajouterFilm(lireFilm(fichier)); // Ajouter le film à la liste.
    }
}

void ListeFilms::ajouterFilm(Film* ptrFilm)
{
    if (ptrFilm == nullptr) {
        cerr << "Erreur : Tentative d'ajouter un film nul (nullptr)." << endl;
        return;
    }

    if (capacite_ == nElements_)
    {
        if (capacite_ != 0) // Si jamais le nb d'éléments vaut 0 alors si on multiplie par 2 ça change rien
        {
            capacite_ *= 2;
        }
        else
        {
            capacite_ = 1;
        }
        Film** nouveauTableau = new Film * [capacite_];
        for (auto&& i : range(0, nElements_))
        {
            nouveauTableau[i] = elements_[i];
        }
        for (auto&& i : range(nElements_, capacite_))
        {
            // Remplit le reste de la liste avec des nullptr
            nouveauTableau[i] = nullptr;
        }
        delete[] elements_;  // Libération de l'ancien tableau
        elements_ = nouveauTableau;
    }
    elements_[nElements_] = ptrFilm;
    nElements_++;
}

void ListeFilms::enleverFilm(Film* ptrFilm)
{
    if (ptrFilm == nullptr) {
        cerr << "Erreur : Tentative d'enlever un film nul (nullptr)." << endl;
        return;
    }

    auto filmsSpan = creerSpanListeFilms(elements_, nElements_);
    for (Film*& film : filmsSpan)
    {
        if (film == ptrFilm)
        {
            // Remplace le film à enlever par le dernier film de la liste
            film = elements_[nElements_ - 1];
            elements_[nElements_ - 1] = nullptr;
            nElements_--;
            break; // On sort de la boucle une fois le film enlevé
        }
    }
}

void ListeFilms::afficherListeFilms() const
{
    if (elements_ == nullptr) {
        cerr << "Erreur : La liste de films est vide (nullptr)." << endl;
        return;
    }

    static const string ligneDeSeparation = "\n-----------------------------------------------------------\n";
    cout << ligneDeSeparation;
    for (Film* ptrFilm : creerSpanListeFilms(elements_, nElements_))
    {
        if (ptrFilm != nullptr) {
            afficherFilm(*ptrFilm);
            cout << ligneDeSeparation;
        }
    }
}

void ListeFilms::detruireListeFilms()
{
    if (elements_ == nullptr) {
        return; // Rien à détruire si la liste est déjà vide
    }

    for (Film* ptrFilm : creerSpanListeFilms(elements_, nElements_))
    {
        if (ptrFilm != nullptr) {
            detruireFilm(ptrFilm);
        }
    }
    delete[] elements_;
    elements_ = nullptr;
    nElements_ = 0;
    capacite_ = 0;
}

Acteur* ListeFilms::trouverActeur(string nomActeur) const
{
    if (nomActeur.empty()) {
        cerr << "Erreur : Le nom de l'acteur est vide." << endl;
        return nullptr;
    }

    for (Film* ptrFilm : creerSpanListeFilms(elements_, nElements_))
    {
        if (ptrFilm != nullptr)
        {
            for (Acteur* ptrActeur : creerSpanListeActeurs(ptrFilm->acteurs.elements, ptrFilm->acteurs.nElements))
            {
                if (ptrActeur != nullptr && ptrActeur->nom == nomActeur)
                    return ptrActeur;
            }
        }
    }
    return nullptr;
}

Acteur* ListeFilms::lireActeur(istream& fichier)
{
    Acteur acteur;
    acteur.nom = lireString(fichier);
    acteur.anneeNaissance = lireUint16(fichier);
    acteur.sexe = lireUint8(fichier);

    if (acteur.nom.empty()) {
        cerr << "Erreur : Le nom de l'acteur est vide." << endl;
        return nullptr;
    }

    Acteur* ptrActeur = trouverActeur(acteur.nom);
    if (ptrActeur != nullptr)
    {
        return ptrActeur;
    }
    else
    {
        ptrActeur = new Acteur();
        *ptrActeur = acteur;
        ptrActeur->joueDans = ListeFilms();
        cout << "Acteur créé : " << acteur.nom << endl;
        return ptrActeur;
    }
}

Film* ListeFilms::lireFilm(istream& fichier)
{
    Film film = {};
    film.titre = lireString(fichier);
    film.realisateur = lireString(fichier);
    film.anneeSortie = lireUint16(fichier);
    film.recette = lireUint16(fichier);
    film.acteurs.nElements = lireUint8(fichier);  // NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.
    film.acteurs.capacite_ = film.acteurs.nElements;

    if (film.titre.empty()) {
        cerr << "Erreur : Le titre du film est vide." << endl;
        return nullptr;
    }

    Film* ptrFilm = new Film();
    *ptrFilm = film;
    ptrFilm->acteurs.elements = new Acteur * [film.acteurs.nElements];
    for (auto&& i : range(0, film.acteurs.nElements))
    {
        Acteur* ptrActeur = lireActeur(fichier); // Placer l'acteur au bon endroit dans les acteurs du film.
        if (ptrActeur != nullptr) {
            ptrFilm->acteurs.elements[i] = ptrActeur;
            ptrActeur->joueDans.ajouterFilm(ptrFilm);
        }
    }
    return ptrFilm;
}

void ListeFilms::detruireFilm(Film* filmADetruire)
{
    if (filmADetruire == nullptr) {
        cerr << "Erreur : Tentative de détruire un film nul (nullptr)." << endl;
        return;
    }

    Film filmActuel = *filmADetruire;
    for (auto i : range(filmActuel.acteurs.nElements - 1, -1, -1)) {
        if (filmActuel.acteurs.elements[i] != nullptr) {
            filmActuel.acteurs.elements[i]->joueDans.enleverFilm(filmADetruire);

            if (filmActuel.acteurs.elements[i]->joueDans.getnElements() == 0) {
                if (filmActuel.acteurs.elements[i]->joueDans.getElements() != nullptr) {
                    delete[] filmActuel.acteurs.elements[i]->joueDans.getElements();
                }
                cout << "Acteur Détruit : " << filmActuel.acteurs.elements[i]->nom << endl;
                delete filmActuel.acteurs.elements[i];
            }
        }
    }
    delete[] filmActuel.acteurs.elements;
    delete filmADetruire;
}

void afficherActeur(const Acteur& acteur)
{
    if (acteur.nom.empty()) {
        cerr << "Erreur : Le nom de l'acteur est vide." << endl;
        return;
    }

    cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

void ListeFilms::afficherFilm(const Film& film) const
{
    if (film.titre.empty()) {
        cerr << "Erreur : Le titre du film est vide." << endl;
        return;
    }

    cout << film.titre << " (" << film.anneeSortie << ") - " << film.realisateur << " - Recette: " << film.recette << "M$";
    cout << "\n";
    for (Acteur* ptrActeur : creerSpanListeActeurs(film.acteurs.elements, film.acteurs.nElements))
    {
        if (ptrActeur != nullptr) {
            afficherActeur(*ptrActeur);
        }
    }
}

void afficherFilmographieActeur(const string& nomActeur, const ListeFilms& listeFilms)
{
    if (nomActeur.empty()) {
        cerr << "Erreur : Le nom de l'acteur est vide." << endl;
        return;
    }

    const Acteur* acteur = listeFilms.trouverActeur(nomActeur);
    if (acteur == nullptr)
        cout << "Aucun acteur de ce nom" << endl;
    else
        acteur->joueDans.afficherListeFilms();
}

int main()
{
    bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

    static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

    ListeFilms listeFilms = ListeFilms("films.bin"); // Substitut de créerListe, un nouveau constructeur

    cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
    if (listeFilms.getElements() != nullptr && listeFilms.getElements()[0] != nullptr) {
        listeFilms.afficherFilm(*listeFilms.getElements()[0]);
    }
    cout << ligneDeSeparation << "Les films sont:" << endl;
    listeFilms.afficherListeFilms();

    Acteur* acteur = listeFilms.trouverActeur("Benedict Cumberbatch");
    if (acteur != nullptr) {
        acteur->anneeNaissance = 1976;
    }

    cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
    afficherFilmographieActeur("Benedict Cumberbatch", listeFilms);

    Film* ptrAlien = listeFilms.getElements()[0];
    if (ptrAlien != nullptr) {
        listeFilms.enleverFilm(ptrAlien);
        listeFilms.detruireFilm(ptrAlien);
    }

    cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
    listeFilms.afficherListeFilms();

    listeFilms.detruireListeFilms();
}