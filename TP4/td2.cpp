/**
* Ce programme a pour but de créer une bibliothèque contenant des livres et des films qui sont tous les deux des enfants de la classe bibliothèque. Nous avons donc créé les classes Film et Livre avec une encapsulation appropriée.
* La bibliothèque est construite en ajoutant les films de la ListeFilms et en lisant le contenu des livres depuis le fichier livres.txt. La classe Item ainsi que ses enfants implémentent l'interface affichable ce qui fait en sorte
* que nous pouvons afficher correctement des items. Nous avons également modifié la classe afficherListeFilms pour qu'elle puisse afficher une liste d'items. Finalement, le code implémente la classe LivreFilm qui hérite de Livre
* et de Film et nous l'avons ajouté à la bibliothèque en vérifiant que le tout s'affiche correctement.
* \file   td4.cpp
* \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
* \date   18 mars 2025
* Créé le 9 mars 2025
*/

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
#include <iomanip>

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
Item::Item(const Item& autre)
{
    titre_ = autre.titre_;
    annee_ = autre.annee_;
}
ostream& operator<<(ostream& os, const Item& item)
{
    item.afficher(os);
    return os;
}
void Item::afficher(ostream& os) const
{
    os << titre_ << " (" << annee_ << ") - ";
}
bool Item::validerTitre(const string& titre)
{
    return titre == titre_;
}
// Fonction pour créer un span pour une ListeFilms
span<shared_ptr<Film>> ListeFilms::creerSpan() const
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

void afficherListeItems(const vector<shared_ptr<Item>>& bibliotheque)
{
    //TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
    static const string ligneDeSeparation = "\n-----------------------------------------------------------\n";
    cout << ligneDeSeparation;
    for (auto&& ptrItem : bibliotheque)
    {
        cout << *ptrItem;
        cout << ligneDeSeparation;
    }
}

///TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
shared_ptr<Acteur> ListeFilms::trouverActeur(string nomActeur) const
{
    for (shared_ptr<Film> ptrFilm : creerSpan())
    {
        if (ptrFilm != nullptr)
        {
            for (shared_ptr<Acteur> ptrActeur : ptrFilm->acteurs_.creerSpan())
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
    string titre = lireString(fichier);
    string realisateur = lireString(fichier);
    int anneeSortie = lireUint16(fichier);
    int recette = lireUint16(fichier);
    int nElements = lireUint8(fichier);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
    shared_ptr<Film> ptrFilm = make_shared<Film>(Film(titre,anneeSortie,realisateur,recette,ListeActeurs(nElements)));
    for (auto&& ptrActeur : ptrFilm->acteurs_.creerSpan())
    {
        ptrActeur = lireActeur(fichier); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
    }
    return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film.
}
template<typename T>
Liste<T>::Liste(int nElementsActeurs)
{
    nElements_ = nElementsActeurs;
    capacite_ = nElementsActeurs;
    elements_ = make_unique<shared_ptr<T>[]>(capacite_);
}
template<typename T>
span<shared_ptr<T>> Liste<T>::creerSpan() const
{
    return span<shared_ptr<T>>(elements_.get(), nElements_);
}
template<typename T>
Liste<T>::Liste(const Liste& autre)
{
    *this = autre;
}
template<typename T>
Liste& Liste<T>::operator= (const Liste& autre) noexcept
{
    nElements_ = autre.nElements_;
    capacite_ = autre.capacite_;
    elements_ = make_unique<shared_ptr<T>[]>(capacite_);
    for (auto&& i : range(0, autre.nElements_))
    {
        elements_[i] = autre.elements_[i];
    }
    return *this;
}
template<typename T>
shared_ptr<T>& Liste<T>::operator[](int index)
{
    return elements_[index];
}

Film::Film(const Film& autre) : Item(autre)
{
    realisateur_ = autre.realisateur_;
    recette_ = autre.recette_;
    acteurs_ = autre.acteurs_;
}

ostream& afficherActeur(ostream& os, const Acteur& acteur) 
{
    os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
    return os;
}
void Livre::afficherSansItem(ostream& os) const 
{
    os << auteur_ << " - Millions de copies vendues: " << millionsCopiesVendues_ << " - Nombre de pages: " << nPages_;
    os << "\n";
}
void Livre::afficher(ostream& os) const 
{
    Item::afficher(os);
    afficherSansItem(os);
}
void Film::afficherSansItem(ostream& os) const
{
    os << realisateur_ << " - Recette: " << recette_ << "M$";
    os << "\n";
    for (shared_ptr<Acteur> ptrActeur : acteurs_.creerSpan())
        afficherActeur(os, *ptrActeur);
}
void Film::afficher(ostream& os) const
{
    Item::afficher(os);
    afficherSansItem(os);
}

vector<shared_ptr<Item>> construireBibliotheque(ListeFilms& listeFilms)
{
    vector<shared_ptr<Item>> bibliotheque = {};
    for (auto&& film : listeFilms.creerSpan()) 
    {
        bibliotheque.push_back(make_unique<Film>(*film));
    }
    ifstream fichier("livres.txt");
    string ligneActuelle = "";
    string auteur;
    int millionsCopiesVendues;
    int nPages;
    string titre;
    int annee;
    istringstream flux(ligneActuelle);
    while (fichier >> quoted(titre) >> annee >> quoted(auteur) >> millionsCopiesVendues >> nPages) 
    {
        Livre livreActuel = Livre(titre, annee, auteur, millionsCopiesVendues, nPages);
        bibliotheque.push_back(make_shared<Livre>(livreActuel));
    }
    return bibliotheque;
}
shared_ptr<Item> trouverItem(const vector<shared_ptr<Item>>& bibliotheque, const string& titre)
{
    for (auto& ptrItem : bibliotheque)
    {
        if (ptrItem->validerTitre(titre))
        {
            return ptrItem;
        }
    }
    return nullptr;
}
FilmLivre initialiserFilmLivre(vector<shared_ptr<Item>> bibliotheque)
{
    shared_ptr<Film> filmHobbit = dynamic_pointer_cast<Film>(trouverItem(bibliotheque, "Le Hobbit : La Bataille des Cinq Armées"));
    shared_ptr<Livre> livreHobbit = dynamic_pointer_cast<Livre>(trouverItem(bibliotheque, "The Hobbit"));
    FilmLivre filmLivreHobbit = FilmLivre(*filmHobbit, *livreHobbit);
    return filmLivreHobbit;
}
void FilmLivre::afficher(ostream& os) const
{
    Item::afficher(os);
    Film::afficherSansItem(os);
    Livre::afficherSansItem(os);
}
int main()
{
    bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

    static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

    ListeFilms listeFilms = ListeFilms("films.bin"); //substitut de créerListe, un nouveau constructeur

    vector<shared_ptr<Item>> bibliotheque = construireBibliotheque(listeFilms);

    FilmLivre hobbit = initialiserFilmLivre(bibliotheque);
    bibliotheque.push_back(make_shared<FilmLivre>(hobbit));

    afficherListeItems(bibliotheque);
    //Pour la couverture de code
    shared_ptr<Film> filmNonExistant = dynamic_pointer_cast<Film>(trouverItem(bibliotheque, "Skibidi Toilet : Attack of the Cameraman"));
}