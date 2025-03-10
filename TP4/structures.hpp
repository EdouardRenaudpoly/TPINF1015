#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <span>
#include <memory>
#include "cppitertools/range.hpp"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;

class Item
{
public:
	Item(string titre = "", int annee = 0)
	{
		titre_ = titre;
		annee_ = annee;
	}
	Item(const Item& autre) 
	{
		titre_ = autre.titre_;
		annee_ = autre.annee_;
	}
	friend ostream& operator<<(ostream& os, const Item& item) 
	{
		return os << item.titre_ << " (" << item.annee_ << ") - ";
	}
private:
	string titre_;
	int annee_;

};

struct Film; struct Acteur; struct Livre;

class ListeFilms {
public:
	ListeFilms();
	ListeFilms(string nomFichier);
	void ajouterFilm(shared_ptr<Film> ptrFilm);
	void enleverFilm(shared_ptr<Film> ptrFilm);
	void afficher() const;
	shared_ptr<Acteur> trouverActeur(string nomActeur) const;
	shared_ptr<Film> lireFilm(istream& fichier);
	shared_ptr<Acteur> lireActeur(istream& fichier);
	span<shared_ptr<Film>> creerSpan() const;
	shared_ptr<Film> chercherFilm(function<bool(const shared_ptr<Film>&)> critere) const;
	shared_ptr<Film>& operator[](int index);
private:
	int capacite_ = 0;
	int nElements_ = 0;
	unique_ptr<shared_ptr<Film>[]> elements_;
};
struct Acteur
{
	string nom = "";
	int anneeNaissance = 0;
	char sexe = 'U';
};// Permet d'utiliser les types alors qu'ils seront défini après.
template<typename T>
class Liste
{
public:
	Liste(int nElementsActeurs = 0)
	{
		nElements_ = nElementsActeurs;
		capacite_ = nElementsActeurs;
		elements_ = make_unique<shared_ptr<T>[]>(capacite_);
	}
	span<shared_ptr<T>> creerSpan() const
	{
		return span<shared_ptr<T>>(elements_.get(), nElements_);
	}
	Liste(const Liste& autre)
	{
		*this = autre;
	}
	Liste& operator= (const Liste& autre) noexcept
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
	shared_ptr<T>& operator[](int index)
	{
		return elements_[index];
	}
private:
	int capacite_, nElements_;
	unique_ptr<shared_ptr<T>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};
using ListeActeurs = Liste<Acteur>;

class Film : public Item
{
public:
	Film(istream& fichier)
	{
		string titre = lireString(fichier);
		realisateur_ = lireString(fichier);
		int anneeSortie = lireUint16(fichier);
		recette_ = lireUint16(fichier);
		int nElements = lireUint8(fichier);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
		acteurs_ = ListeActeurs(nElements);
		Item(titre, anneeSortie);
	}
	Film(string titre, int annee, string realisateur, int recette, ListeActeurs acteurs) : Item(titre, annee)
	{
		realisateur_ = realisateur;
		recette_ = recette;
		acteurs_ = move(acteurs);
	}
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(string nomActeur) const;
	friend shared_ptr<Film> ListeFilms::lireFilm(istream& fichier);
	Film(const Film& autre);
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(string nomActeur) const;
	friend shared_ptr<Film> ListeFilms::lireFilm(istream& fichier);
	friend ostream& operator<<(ostream& os, const Film& film);
private:
	string realisateur_ = "";
	int recette_ = 0;
	ListeActeurs acteurs_;
};

class Livre : public Item
{
public:
	Livre(string titre, int annee, string auteur, int millionsCopiesVendues, int nPages) : Item(titre, annee)
	{
		auteur_ = auteur;
		millionsCopiesVendues_ = millionsCopiesVendues;
		nPages_ = nPages;
	}
private:
	string auteur_;
	int millionsCopiesVendues_;
	int nPages_;
};