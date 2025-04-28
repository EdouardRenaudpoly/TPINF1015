//Le fichier main.cpp initialise et lance l'application de jeu d'échecs.Il construit l'échiquier, place les pièces à leurs positions de départ, et gère 
// l'interface graphique pour permettre aux joueurs d'interagir avec le plateau. Le main sert de point d'entrée au programme et orchestre la boucle principale du jeu en
// plus de rouler les tests écrits à l'avance pour s'assurer du bon fonctionnement du jeu
// \file   main.cpp
// \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
// \date   12 avril 2025
// Créé le 6 avril 2025
//

#include "Ui.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QString>
using namespace Ui;

#if __has_include("bibliotheque_cours.hpp")
#include "bibliotheque_cours.hpp"
#define BIBLIOTHEQUE_COURS_INCLUS
using bibliotheque_cours::cdbg;
#else
auto& cdbg = clog;
#endif

#if __has_include("verification_allocation.hpp")
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette ent�te si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#endif

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par d�faut.

	// cdbg.setTee(&clog);  // D�commenter cette ligne pour que cdbg affiche sur la console en plus de la "Sortie" du d�bogueur.

	bibliotheque_cours::executerGoogleTest(argc, argv); // Attention de ne rien afficher avant cette ligne, sinon l'Explorateur de tests va tenter de lire votre affichage comme un r�sultat de test.
#endif
	//NOTE: C'est normal que la couverture de code dans l'Explorateur de tests de Visual Studio ne couvre pas la fin de cette fonction ni la fin du main apr�s l'appel � cette fonction puisqu'il ex�cute uniquement les tests Google Test dans l'appel ci-dessus.
}


int main(int argc, char *argv[])
{
    bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
    QApplication app(argc, argv);
    initialiserBibliothequeCours(argc, argv);
    ProjetJeuxEchecs w;
    w.show();
    return app.exec();
}