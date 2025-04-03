// Fichier de test pour le projet Jeux d'�checs
// Ce fichier contient les tests unitaires pour le projet Jeux d'�checs.
// Pour le moment, il y a seulement des tests d'exemple.
// Vous devez ajouter vos propres tests pour v�rifier le bon fonctionnement de votre code.
// 
// 
//
// Include ici les headers de votre partie de code � tester

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif
#include "ProjetJeuxEchecs.h"
#include <QMessageBox>
#include <QString>
#ifdef TEST

TEST(Test, exempleDeTest)
{
	try
	{
		Roi roi1(0,0,true);
		Roi roi2(0,0,false);
		Roi roi3(0,0,false); //cause une exception
		FAIL() << "L'exception n'a pas �t� lanc�e !";
	}
	catch (TropDeRoisException)
	{
		QMessageBox::critical(nullptr, "Erreur", "Trop de rois sont actuellement sur l'�chiquier");
		SUCCEED() << "Le test du roi est fonctionnel";
	}
}

#endif