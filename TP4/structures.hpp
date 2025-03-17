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
class Affichable {
public:
	virtual void afficher(ostream& os) const = 0;
	virtual ~Affichable() = default;  
};
ostream& operator<<(ostream& os, const Affichable& obj) {
	obj.afficher(os);
	return os;
}
class Item : Affichable
{
public:
	Item(string titre = "", int annee = 0) : titre_(titre), annee_(annee) {}
	Item(const Item& autre) 
	{
		titre_ = autre.titre_;
		annee_ = autre.annee_;
	}
	friend ostream& operator<<(ostream& os, const Item& item) 
	{
		item.afficher(os);
		return os;
	}
	void afficher(ostream& os) const override
	{
		os << titre_ << " (" << annee_ << ") - ";
	}
	bool validerTitre(const string& titre)
	{
		return titre == titre_;
	}
private:
	string titre_;
	int annee_;

};

class Film; struct Acteur; class Livre;

class ListeFilms {
public:
	ListeFilms();
	ListeFilms(string nomFichier);
	void ajouterFilm(shared_ptr<Film> ptrFilm);
	void enleverFilm(shared_ptr<Film> ptrFilm);
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
	Liste(int nElementsActeurs = 0) :nElements_(nElementsActeurs), capacite_(nElementsActeurs)
	{
		elements_ = make_unique<shared_ptr<T>>(elements_.get(), nElements_);
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
	unique_ptr<shared_ptr<T>[]> elements_;
};
using ListeActeurs = Liste<Acteur>;

class Film : virtual public Item
{
public:
	Film(string titre, int annee, string realisateur, int recette, ListeActeurs acteurs) : Item(titre, annee), realisateur_(realisateur), recette_(recette), acteurs_(acteurs) {}
	Film(const Film& autre);
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(string nomActeur) const;
	friend shared_ptr<Film> ListeFilms::lireFilm(istream& fichier);
	friend ostream& operator<<(ostream& os, const Film& film);
	void afficher(ostream& os) const;
private:
	string realisateur_ = "";
	int recette_ = 0;
	ListeActeurs acteurs_;
};

class Livre : virtual public Item
{
public:
	Livre(string titre, int annee, string auteur, int millionsCopiesVendues, int nPages) : Item(titre, annee), auteur_(auteur), millionsCopiesVendues_(millionsCopiesVendues), nPages_(nPages) {}
	void afficher(ostream& os) const;
private:
	string auteur_;
	int millionsCopiesVendues_;
	int nPages_;
};
ostream& operator<<(ostream& os, const Livre& livre)
{
	livre.afficher(os);
	return os;
}
class FilmLivre : public Film, public Livre
{
public:
	FilmLivre(const Film& film, const Livre& livre) : Item(film), Film(film), Livre(livre) {}
	void afficher(ostream& os) const
	{
		Livre::afficher(os);
		Film::afficher(os);
	}
};
ostream& operator<<(ostream& os, const FilmLivre& filmLivre)
{
	filmLivre.afficher(os);
	return os;
}
