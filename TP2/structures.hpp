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
struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.
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

struct Acteur
{
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
class ListeFilms {
public:
	ListeFilms() //constructeur de base
	{
		capacite_ = 0;
		nElements_ = 0;
		elements_ = nullptr;
	}
	ListeFilms(int capacite, int nElements, Film** elements) //constructeur avec paramètres
		: ListeFilms() 
	{
		if (nElements<capacite && nElements>=0 && capacite>=0)
		{
			capacite_ = capacite;
			nElements_ = nElements;
			if (capacite_ > 0)
			{
				Film** elements_ = new Film * [capacite];
				for (auto&& i : range(0, nElements))
				{
					if (elements[i] == nullptr)
					{
						nElements--;
					}
					elements_[i] = elements[i];
				}
			}
			else
			{
				elements_ = nullptr;
			}
		}
	}
	///TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
	void ajouterFilm(Film* ptrFilm)
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
	void enleverFilm(Film* ptrFilm)
	{
		bool filmEnleve = false;
		int index = 0; /// À regarder il faut changer le while pour une boucle for
		while (!filmEnleve || index < nElements_)
		{
			if (elements_[index] == ptrFilm)
			{
				elements_[index] = elements_[nElements_ - 1];
				elements_[nElements_ - 1] = nullptr;
				nElements_--;
				filmEnleve = true;
			}
			index++;
		}
	}
	void afficherListeFilms() const
	{
		//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
		static const string ligneDeSeparation = "\n-----------------------------------------------------------\n";
		cout << ligneDeSeparation;
		///TODO: Changer le for pour utiliser un span. // Je pense que c good
		for (Film* ptrFilm : span(elements_, nElements_))
		{
			///TODO: Afficher le film.
			afficherFilm(*ptrFilm);
			cout << ligneDeSeparation;
		}
	}
	///TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
	void detruireListeFilms()
	{
		///Pt pas un span
		for (Film* ptrFilm : span(elements_, nElements_))
		{
			detruireFilm(ptrFilm);
		}
		delete[] elements_;
	}
	///TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
	Acteur* trouverActeur(string nomActeur) const
	{
		for (Film* ptrFilm : span(elements_, nElements_))
		{
			for (Acteur* ptrActeur : span(ptrFilm->acteurs.elements, ptrFilm->acteurs.nElements))
			{
				if (ptrActeur->nom == nomActeur)
					return ptrActeur;
			}
		}
		return nullptr;
	}
	Acteur* lireActeur(istream& fichier)
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
		//TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
	}
	Film* lireFilm(istream& fichier)
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
			//TODO: Ajouter le film à la liste des films dans lesquels l'acteur joue.
			ptrActeur->joueDans.ajouterFilm(ptrFilm);
		}
		return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film.
	}
	int getCapacite()
	{
		return capacite_;
	}
	int getnElements()
	{
		return nElements_;
	}
	Film** getElements()
	{
		return elements_;
	}
private:
	int capacite_, nElements_;
	Film** elements_;
};