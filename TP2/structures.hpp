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
	ListeFilms() : capacite_(0), nElements_(0), elements_(make_unique<shared_ptr<Film>[]>(0)) {}
	ListeFilms(string nomFichier);
	void ajouterFilm(Film* ptrFilm);
	void enleverFilm(Film* ptrFilm);
	void afficherListeFilms() const;
	void detruireListeFilms();
	Acteur* trouverActeur(string nomActeur) const;
	void detruireFilm(Film* filmADetruire);
	void afficherFilm(const Film& film) const;
	Film* lireFilm(istream& fichier);
	Acteur* lireActeur(istream& fichier);
	span<Film*> creerSpanListeFilms() const;
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

//struct ListeActeurs 
//{
//	ListeActeurs() : capacite(0), nElements(0), elements(make_unique<shared_ptr<Acteur>[]>(0)) {}
//
//	int capacite, nElements;
//	unique_ptr<shared_ptr<Acteur>[]> elements;
//
//	span<shared_ptr<Acteur>> creerSpanListeActeurs() const {
//		return span<shared_ptr<Acteur>>(elements.get(), nElements);
//	}
//};

struct ListeActeurs
{
	ListeActeurs(int nElementsActeurs = 0)
	{
		nElements = nElementsActeurs;
		capacite = nElementsActeurs;
		elements = make_unique<Acteur * []>(capacite);
	}
	int capacite, nElements;
	unique_ptr<Acteur* []> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	span<Acteur*> creerSpanListeActeurs() const
	{
		return span<Acteur*>(elements.get(), nElements);
	}
};
struct Film
{
	string titre = "";
	string realisateur = "";
	int anneeSortie = 0;
	int recette = 0;
	ListeActeurs acteurs;
};