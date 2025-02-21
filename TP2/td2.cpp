#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <span>
#include <memory> //TP3
#include <sstream>

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
span<shared_ptr<Film>> ListeFilms::creerSpanListeFilms() const
{
    return span<shared_ptr<Film>>(elements_.get(), nElements_);
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
void ListeFilms::ajouterFilm(shared_ptr<Film> ptrFilm)
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
        unique_ptr<shared_ptr<Film>[]> nouveauTableau = make_unique<shared_ptr<Film>[]>(capacite_);
        for (auto&& i : range(0, nElements_))
        {
            nouveauTableau[i] = elements_[i];
            elements_[i] = nullptr;
        }
        for (auto&& i : range(nElements_, capacite_))
        {
            //Rempli le reste de la liste avec des nullptr
            nouveauTableau[i] = nullptr;
        }
        elements_ = move(nouveauTableau);
    }
    elements_[nElements_] = ptrFilm;
    nElements_++;
}

///TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
void ListeFilms::enleverFilm(shared_ptr<Film> ptrFilm)
{
    // Parcourt les films avec un span
    for (shared_ptr<Film>& film : creerSpanListeFilms())
    {
        if (film == ptrFilm)
        {
            // Remplace le film à enlever par le dernier film de la liste
            film = elements_[nElements_ - 1] ;
            elements_[nElements_ - 1] = nullptr;
            nElements_--;
            return; // On sort de la fonction une fois le film enlevé
        }
    }
    cout << "Aucun film ne correspond à celui envoyé en paramètre" << endl;
}

void ListeFilms::afficherListeFilms() const
{
    //TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
    static const string ligneDeSeparation = "\n-----------------------------------------------------------\n";
    cout << ligneDeSeparation;
    for (shared_ptr<Film> ptrFilm : creerSpanListeFilms())
    {
        cout << *ptrFilm;
        cout << ligneDeSeparation;
    }
}

///TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
shared_ptr<Acteur> ListeFilms::trouverActeur(string nomActeur) const
{
    for (shared_ptr<Film> ptrFilm : creerSpanListeFilms())
    {
        if (ptrFilm != nullptr)
        {
            for (shared_ptr<Acteur> ptrActeur : ptrFilm->acteurs.creerSpanListeActeurs())
            {
                if (ptrActeur->nom == nomActeur)
                    return ptrActeur;
            }
        }
    }
    return nullptr;
}

//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
shared_ptr<Acteur> ListeFilms::lireActeur(istream& fichier)
{
    Acteur acteur;
    acteur.nom = lireString(fichier);
    acteur.anneeNaissance = lireUint16(fichier);
    acteur.sexe = lireUint8(fichier);
    shared_ptr<Acteur> ptrActeur = trouverActeur(acteur.nom);
    if (ptrActeur != nullptr)
    {
        return ptrActeur;
    }
    else
    {
        ptrActeur = make_shared<Acteur>(acteur);
        cout << "Acteur créé : " << acteur.nom << endl;
        return ptrActeur;
    }
}

shared_ptr<Film> ListeFilms::lireFilm(istream& fichier)
{
    Film film = {};
    film.titre = lireString(fichier);
    film.realisateur = lireString(fichier);
    film.anneeSortie = lireUint16(fichier);
    film.recette = lireUint16(fichier);
    int nElements = lireUint8(fichier);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
    film.acteurs = ListeActeurs(nElements);
    shared_ptr<Film> ptrFilm = make_shared<Film>(move(film));
    for ([[maybe_unused]] int i : range(0, nElements))
    {
        shared_ptr<Acteur> ptrActeur = lireActeur(fichier); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
        ptrFilm->acteurs.elements[i]=ptrActeur;
    }
    return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film.
}
shared_ptr<Film>& ListeFilms::operator[](int index)
{
    return elements_[index];
}
//Film& Film::operator= (Film&& autre) noexcept {
//    titre=autre.titre;
//    realisateur = autre.realisateur; 
//    anneeSortie = autre.anneeSortie;
//    recette = autre.recette;
//    acteurs = move(autre.acteurs);
//    return *this;
//}
//Film::Film(Film&& autre) noexcept {
//    *this = move(autre);
//}
Film::Film(const Film& autre)
{
    titre = autre.titre;
    realisateur = autre.realisateur;
    anneeSortie = autre.anneeSortie;
    recette = autre.recette;
    acteurs = autre.acteurs;
}

ostream& afficherActeur(ostream& os, const Acteur& acteur) {
    os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
    return os;
}

///TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
ostream& operator<<(ostream& os, const Film& film)
{
    os << film.titre << " (" << film.anneeSortie << ") - " << film.realisateur << " - Recette: " << film.recette << "M$";
    os << "\n";
    for (shared_ptr<Acteur> ptrActeur : film.acteurs.creerSpanListeActeurs())
        afficherActeur(os,*ptrActeur);
    return os;
}

shared_ptr<Film> ListeFilms::chercherFilm(function<bool(const shared_ptr<Film>&)> critere) const {
    for (const auto& film : creerSpanListeFilms()) {
        if (critere(film)) {
            return film;
        }
    }
    return nullptr;
}

int main()
{
    bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

    static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

    ListeFilms listeFilms = ListeFilms("films.bin"); //substitut de créerListe, un nouveau constructeur

    cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
    cout<<*listeFilms[0];
    cout << ligneDeSeparation << "Les films sont:" << endl;
    listeFilms.afficherListeFilms();
    listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;

    Film skylien = *listeFilms[0];
    skylien.titre = "Skylien";
    skylien.acteurs[0] = listeFilms[1]->acteurs[0];
    skylien.acteurs[0]->nom = "Daniel Wroughton Craig";
    cout << ligneDeSeparation << "Modifications TP3" << "\n";
    cout << skylien;
    cout << *listeFilms[0];
    cout << *listeFilms[1];

    cout << "\nRecherche du film avec une recette de 955M$ :" << endl;
    shared_ptr<Film> filmCherche = listeFilms.chercherFilm([](const shared_ptr<Film>& film) 
    {
        return film->recette == 955;
    });

    if (filmCherche) 
    {
        cout << "Film trouvé : " << *filmCherche << endl;
    }

    shared_ptr<Film> ptrAlien = listeFilms[0];
    listeFilms.enleverFilm(ptrAlien);
    cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
    listeFilms.afficherListeFilms();
    listeFilms.enleverFilm(nullptr);
    //Validation classe générique Liste
    Liste<string> listeTextes(2);
    listeTextes[0] = make_shared<string>("Salut");
    listeTextes[1] = make_shared<string>("Bonjour");
    Liste<string> listeTextes2 = listeTextes;
    listeTextes[0] = make_shared<string>("Allo");
    *listeTextes[1] = "Salut2";
    cout << "listeTextes[0] : " << *listeTextes[0] << endl;
    cout << "listeTextes[1] : " << *listeTextes[1] << endl;
    cout << "listeTextes2[0] : " << *listeTextes2[0] << endl;
    cout << "listeTextes2[1] : " << *listeTextes2[1] << endl;
    //Pour la couverture de code
    shared_ptr<Film> filmInexistant = listeFilms.chercherFilm([](const shared_ptr<Film>& film)
        {
            return film->recette == 34095867;
        });
}