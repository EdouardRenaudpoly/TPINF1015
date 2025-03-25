/**
* Ce programme a pour but de continuer la bibliotheque et de nous familiariser avec les conteneurs. La première partie consiste à implémenter des copies dans l'ordre original ou inversé des éléments de la bibliothèque avec
* des vecteurs ou des forward_list. On implémente également begin et end dans la classe générique Liste pour pouvoir itérer dedans. Par la suite, il faut implémenter des conteneurs pour permettre de trier les items et pour
* pouvoir trouver un Item facilement. Finalement, nous avons implémenté des algorithmes dont un qui copie tous les items de la bibliothèque qui sont des films et un qui fait la somme des recettes de la bibliothèque copiée juste avant
* \file   td5.cpp
* \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
* \date   23 mars 2025
* Créé le 21 mars 2025
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
#include <forward_list>
#include <set>
#include <unordered_set>

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
    os << titre_;
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
template<std::ranges::input_range T>
void afficherListeItems(const T& bibliotheque)
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
Liste<T>& Liste<T>::operator= (const Liste<T>& autre) noexcept
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

ostream& operator<<(ostream& os, const Acteur& acteur) 
{
    os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
    return os;
}
void Livre::afficherSansItem(ostream& os) const 
{
    os << ", de "<<auteur_;
    os << "\n";
}
void Film::afficherPourFilmLivre(ostream& os) const
{
    os << ", de " << realisateur_;
}
void Livre::afficher(ostream& os) const 
{
    Item::afficher(os);
    afficherSansItem(os);
}
void Film::afficherSansItem(ostream& os) const
{
    os << ", par " << realisateur_;
    os << "\n";
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
        bibliotheque.push_back(make_shared<Film>(*film));
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
    Film::afficherPourFilmLivre(os);
    Livre::afficherSansItem(os);
}
template<std::ranges::input_range T>
forward_list<shared_ptr<Item>> copierPointeurs(const T& bibliotheque)
{
    forward_list<shared_ptr<Item>> listeCopiee;
    forward_list<shared_ptr<Item>>::iterator it = listeCopiee.before_begin();
    for (auto&& ptrItem : bibliotheque) 
    {
        it = listeCopiee.insert_after(it, ptrItem);
    }
    return listeCopiee;
}
forward_list<shared_ptr<Item>> copierOrdreInverse(const forward_list<shared_ptr<Item>>& bibliotheque)
{
    forward_list<shared_ptr<Item>> listeInversee;
    for (auto&& ptrItem : bibliotheque)
    {
        listeInversee.push_front(ptrItem);
    }
    return listeInversee;
}
vector<shared_ptr<Item>> copierOrdreInverseVecteur(const forward_list<shared_ptr<Item>>& bibliotheque)
{
    vector<shared_ptr<Item>> vecteurInverse(distance(bibliotheque.begin(),bibliotheque.end()));  //ligne reprise depuis le site : https://how.dev/answers/what-is-the-distance-function-in-cpp
    vector<shared_ptr<Item>>::reverse_iterator it = vecteurInverse.rbegin();
    for (auto&& ptrItem : bibliotheque)
    {
        *it = ptrItem;
        it++;
    }
    return vecteurInverse;
}
int main()
{
    bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

    static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

    ListeFilms listeFilms = ListeFilms("films.bin"); //substitut de créerListe, un nouveau constructeur

    vector<shared_ptr<Item>> bibliotheque = construireBibliotheque(listeFilms);

    FilmLivre hobbit = initialiserFilmLivre(bibliotheque);
    shared_ptr<FilmLivre> ptrHobbit = make_shared<FilmLivre>(hobbit);
    bibliotheque.push_back(ptrHobbit);

    afficherListeItems(bibliotheque);
    cout << ligneDeSeparation << endl;
    forward_list<shared_ptr<Item>> biblioForwardList = copierPointeurs(bibliotheque); //1.1
    afficherListeItems(biblioForwardList);
    cout << ligneDeSeparation << endl;
    forward_list<shared_ptr<Item>> biblioInversee = copierOrdreInverse(biblioForwardList); // 1.2
    afficherListeItems(biblioInversee);
    cout << ligneDeSeparation << endl;
    forward_list<shared_ptr<Item>> biblioCopiee = copierPointeurs(biblioForwardList); //1.3
    afficherListeItems(biblioCopiee);
    cout << ligneDeSeparation << endl;
    vector<shared_ptr<Item>> vecteurCopie = copierOrdreInverseVecteur(biblioForwardList); //1.4
    afficherListeItems(vecteurCopie);
    cout << ligneDeSeparation << endl;
    Film film = *dynamic_pointer_cast<Film>(bibliotheque[0]);
    for (auto&& acteur : film.acteurs_) //1.5
    {
        cout << *acteur;
    }
    cout << ligneDeSeparation << endl;
    auto fonctionTri = [](const shared_ptr<Item>& a,const shared_ptr<Item>& b) {return a->titre_ < b->titre_;};
    set <shared_ptr<Item>, decltype(fonctionTri)> bibliothequeTriee(fonctionTri);
    for (auto&& ptrItem : bibliotheque)
    {
        bibliothequeTriee.insert(ptrItem);
    }
    afficherListeItems(bibliothequeTriee); //2.1
    cout << ligneDeSeparation << endl;
    unordered_set<shared_ptr<Item>> bibliothequeTrouve;
    for (auto&& ptrItem : bibliotheque)
    {
        bibliothequeTrouve.insert(ptrItem);
    }
    
    string nomRecherche="The Hobbit";
    auto it = find_if(bibliothequeTrouve.begin(), bibliothequeTrouve.end(),
        [&nomRecherche](const shared_ptr<Item>& item) {
            return item->titre_ == nomRecherche;
        });
    //2.2
    if (it != bibliothequeTrouve.end())
    {
        cout << "Film Trouvé !" << endl;
        cout << **it;
    }
    else
    {
        cout << "Film non trouvé" << endl;
    }
    cout << ligneDeSeparation << endl;


    //Pour la couverture de code
    shared_ptr<Film> filmNonExistant = dynamic_pointer_cast<Film>(trouverItem(bibliotheque, "Skibidi Toilet : Attack of the Cameraman"));
}