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
struct Film; struct Acteur;
class ListeFilms {
public:
	ListeFilms();
	ListeFilms(string nomFichier);
	void ajouterFilm(shared_ptr<Film> ptrFilm);
	void enleverFilm(shared_ptr<Film> ptrFilm);
	void afficherListeFilms() const;
	shared_ptr<Acteur> trouverActeur(string nomActeur) const;
	shared_ptr<Film> lireFilm(istream& fichier);
	shared_ptr<Acteur> lireActeur(istream& fichier);
	span<shared_ptr<Film>> creerSpanListeFilms() const;
	shared_ptr<Film> chercherFilm(function<bool(const shared_ptr<Film>&)> critere) const;
	shared_ptr<Film> operator[](int index);
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

struct ListeActeurs
{
	ListeActeurs(int nElementsActeurs = 0)
	{
		nElements = nElementsActeurs;
		capacite = nElementsActeurs;
		elements = make_unique<shared_ptr<Acteur>[]>(capacite);
	}
	int capacite, nElements;
	unique_ptr<shared_ptr<Acteur>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	span<shared_ptr<Acteur>> creerSpanListeActeurs() const
	{
		return span<shared_ptr<Acteur>>(elements.get(), nElements);
	}
};
struct Film
{
	string titre = "";
	string realisateur = "";
	int anneeSortie = 0;
	int recette = 0;
	ListeActeurs acteurs;
	/*Film& operator= (Film&& autre) noexcept;*/
	Film()
	{

	}
	/*Film(Film&& autre) noexcept;*/
	Film(const Film& autre);
};
ostream& operator<<(ostream& os,const Film& film);