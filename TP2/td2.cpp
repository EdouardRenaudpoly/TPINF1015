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

//constructeurs de la classe ListeFilms
ListeFilms::ListeFilms() //constructeur de base
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
        ajouterFilm(lireFilm(fichier)); ///TODO: Ajouter le film à la liste.
    }
}

///TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
void ListeFilms::ajouterFilm(Film* ptrFilm)
{
    if (capacite_ == nElements_)
    {
        if (capacite_ != 0) //Si jamais le nb d'elements vaut 0 alors si on multiplie par 2 ça change rien
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
            //Rempli le reste de la liste avec des nullptr
            nouveauTableau[i] = nullptr;
        }
        delete[] elements_;  // Libération de l'ancien tableau
        elements_ = nouveauTableau;
    }
    elements_[nElements_] = ptrFilm;
    nElements_++;
}

///TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
void ListeFilms::enleverFilm(Film* ptrFilm)
{
    // Parcourt les films avec un span
    for (Film*& film : creerSpanListeFilms(elements_, nElements_))
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
    //TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
    static const string ligneDeSeparation = "\n-----------------------------------------------------------\n";
    cout << ligneDeSeparation;
    for (Film* ptrFilm : creerSpanListeFilms(elements_, nElements_))
    {
        afficherFilm(*ptrFilm);
        cout << ligneDeSeparation;
    }
}

///TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
void ListeFilms::detruireListeFilms()
{
    for (Film* ptrFilm : creerSpanListeFilms(elements_, nElements_))
    {
        detruireFilm(ptrFilm);
    }
    delete[] elements_;
}

///TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
Acteur* ListeFilms::trouverActeur(string nomActeur) const
{
    for (Film* ptrFilm : creerSpanListeFilms(elements_, nElements_))
    {
        if (ptrFilm != nullptr)
        {
            for (Acteur* ptrActeur : creerSpanListeActeurs(ptrFilm->acteurs.elements, ptrFilm->acteurs.nElements))
            {
                if (ptrActeur->nom == nomActeur)
                    return ptrActeur;
            }
        }
    }
    return nullptr;
}

//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
Acteur* ListeFilms::lireActeur(istream& fichier)
{
    Acteur acteur;
    acteur.nom = lireString(fichier);
    acteur.anneeNaissance = lireUint16(fichier);
    acteur.sexe = lireUint8(fichier);
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
    film.acteurs.nElements = lireUint8(fichier);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
    film.acteurs.capacite_ = film.acteurs.nElements;
    Film* ptrFilm = new Film();
    *ptrFilm = film;
    ptrFilm->acteurs.elements = new Acteur * [film.acteurs.nElements];
    for (auto&& i : range(0, film.acteurs.nElements))
    {
        Acteur* ptrActeur = lireActeur(fichier); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
        ptrFilm->acteurs.elements[i] = ptrActeur;
        ptrActeur->joueDans.ajouterFilm(ptrFilm);
    }
    return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film.
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.
void ListeFilms::detruireFilm(Film* filmADetruire)
{
    Film filmActuel = *filmADetruire;
    for (auto i : range(filmActuel.acteurs.nElements - 1, -1, -1)) {
        filmActuel.acteurs.elements[i]->joueDans.enleverFilm(filmADetruire);

        if (filmActuel.acteurs.elements[i]->joueDans.getnElements() == 0) {
            if (filmActuel.acteurs.elements[i]->joueDans.getElements() != nullptr) {
                delete[] filmActuel.acteurs.elements[i]->joueDans.getElements();
            }
            cout << "Acteur Détruit : " << filmActuel.acteurs.elements[i]->nom << endl;
            delete filmActuel.acteurs.elements[i];
        }
    }
    delete[] filmActuel.acteurs.elements;
    delete filmADetruire;
}

void afficherActeur(const Acteur& acteur)
{
    cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

///TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
void ListeFilms::afficherFilm(const Film& film) const
{
    cout << film.titre << " (" << film.anneeSortie << ") - " << film.realisateur << " - Recette: " << film.recette << "M$";
    cout << "\n";
    for (Acteur* ptrActeur : creerSpanListeActeurs(film.acteurs.elements, film.acteurs.nElements))
        afficherActeur(*ptrActeur);
}

void afficherFilmographieActeur(const string& nomActeur, const ListeFilms& listeFilms)
{
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

    ListeFilms listeFilms = ListeFilms("films.bin"); //substitut de créerListe, un nouveau constructeur

    cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
    listeFilms.afficherFilm(*listeFilms.getElements()[0]);
    cout << ligneDeSeparation << "Les films sont:" << endl;
    listeFilms.afficherListeFilms();
    listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;
    cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
    afficherFilmographieActeur("Benedict Cumberbatch", listeFilms);
    Film* ptrAlien = listeFilms.getElements()[0];
    listeFilms.enleverFilm(ptrAlien);
    listeFilms.detruireFilm(ptrAlien);
    cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
    listeFilms.afficherListeFilms();
    listeFilms.detruireListeFilms();
}