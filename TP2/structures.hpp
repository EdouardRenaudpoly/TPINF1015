#pragma once
// Structures mémoires pour une collection de films.

#include <string>
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
struct Film; struct Acteur;
class ListeFilms {
public:
	ListeFilms();
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
	int capacite_, nElements_;
	Film** elements_;
};
struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};// Permet d'utiliser les types alors qu'ils seront défini après.
struct ListeActeurs {
	int capacite_, nElements;
	Acteur** elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};
struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};