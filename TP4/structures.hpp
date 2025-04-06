/**
* Ces classes sont les mêmes que pour le td précédent, nous avons modifié la classe Liste pour pouvoir itérer dedans à l'aide d'un for classique, les méthodes de classe begin et end on été ajoutées.
* \file   structures.hpp
* \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
* \date   23 mars 2025
* Créé le 21 mars 2025
*/
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

class Item : Affichable
{
public:
	Item(string titre = "", int annee = 0) : titre_(titre), annee_(annee) {}
	Item(const Item& autre);
	friend ostream& operator<<(ostream& os, const Item& item);
	void afficher(ostream& os) const override;
	bool validerTitre(const string& titre);
	string titre_;
	int annee_;

};

class Film; struct Acteur; class Livre;

class ListeFilms {
public:
	ListeFilms();
	ListeFilms(string nomFichier);
	void ajouterFilm(shared_ptr<Film> ptrFilm);
	shared_ptr<Acteur> trouverActeur(string nomActeur) const;
	shared_ptr<Film> lireFilm(istream& fichier);
	shared_ptr<Acteur> lireActeur(istream& fichier);
	span<shared_ptr<Film>> creerSpan() const;
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
ostream& operator<<(ostream& os, const Acteur& acteur);
template<typename T>
class Liste
{
public:
	Liste(int nElementsActeurs = 0);
	span<shared_ptr<T>> creerSpan() const;
	Liste(const Liste& autre);
	Liste& operator= (const Liste& autre) noexcept;
	shared_ptr<T>& operator[](int index);

	shared_ptr<T>* begin() 
	{
		return &elements_[0];
	}

	shared_ptr<T>* end() 
	{
		return &elements_[nElements_];
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
	void afficher(ostream& os) const;
	void afficherSansItem(ostream& os) const;
	void afficherPourFilmLivre(ostream& os) const;
	string realisateur_ = "";
	int recette_ = 0;
	ListeActeurs acteurs_;
};

class Livre : virtual public Item
{
public:
	Livre(string titre, int annee, string auteur, int millionsCopiesVendues, int nPages) : Item(titre, annee), auteur_(auteur), millionsCopiesVendues_(millionsCopiesVendues), nPages_(nPages) {}
	void afficher(ostream& os) const;
	void afficherSansItem(ostream& os) const;
	string auteur_;
	int millionsCopiesVendues_;
	int nPages_;
};
class FilmLivre : public Film, public Livre
{
public:
	FilmLivre(const Film& film, const Livre& livre) : Item(film), Film(film), Livre(livre) {}
	void afficher(ostream& os) const;
};
