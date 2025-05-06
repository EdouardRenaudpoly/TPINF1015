//Le fichier de tests automatise la validation du comportement du modèle du jeu d'échecs. Il contient une série de cas de test qui vérifient la logique de mouvement 
// de chaque pièce, la détection d'états comme l'échec ou le mat, ainsi que la gestion de l'échiquier.Ces tests garantissent que les différentes composantes fonctionnent 
// correctement et permettent d'assurer la fiabilité du projet lors de futures modifications.
// \file   TestJeuxEchecs.cpp
// \author Édouard Renaud (2384807) et Zackary Labelle (2386427)
// \date   12 avril 2025
// Créé le 6 avril 2025
//

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif
#include "Modele.h"
#include "Ui.h"
#include <QMessageBox>
#include <QString>

#ifdef TEST

TEST(ModeleTest, Roi_Exception_TropDeRois) 
{
    using namespace Modele;
    Echiquier e;
    Roi* roi1 = new Roi(Position(0, 0), true);
    Roi* roi2 = new Roi(Position(1, 1), false);
    e.ajouterPiece(roi1);
    e.ajouterPiece(roi2);

    // Le 3e roi doit lancer une exception
    EXPECT_THROW({
        Roi * roi3 = new Roi(Position(2,2), true);
        delete roi3;
        }, TropDeRoisException);

    e.enleverPieces(); // nettoyage
}

TEST(ModeleTest, Deplacement_Valide_Roi) 
{
    using namespace Modele;
    Echiquier e;
    Roi* roi = new Roi(Position(3, 3), true);
    e.ajouterPiece(roi);

    auto [ok, msg] = e.deplacerPiece(Position(3, 3), Position(4, 4));
    EXPECT_TRUE(ok);
    EXPECT_TRUE(msg.empty());

    e.enleverPieces();
}

TEST(ModeleTest, Deplacement_Invalide_Obstacle_Tour) 
{
    using namespace Modele;
    Echiquier e;
    Tour* tour = new Tour(Position(0, 0), true);
    Cavalier* obstacle = new Cavalier(Position(0, 1), true);
    e.ajouterPiece(tour);
    e.ajouterPiece(obstacle);

    auto [ok, msg] = e.deplacerPiece(Position(0, 0), Position(0, 2));
    EXPECT_FALSE(ok);
    EXPECT_EQ(msg, "cette piece ne peut pas passer au-dessus des autres");

    e.enleverPieces();
}

TEST(ModeleTest, Cavalier_DeplacementValide) {
    using namespace Modele;
    Echiquier e;
    auto c = new Cavalier(Position(4, 4), true);
    e.ajouterPiece(c);
    EXPECT_TRUE(c->estDeplacementValide(Position(6, 5)));
    EXPECT_FALSE(c->estDeplacementValide(Position(5, 5)));
    e.enleverPieces();
}

TEST(ModeleTest, Tour_DeplacementVertical_Libre) {
    using namespace Modele;
    Echiquier e;
    auto t = new Tour(Position(0, 0), true);
    e.ajouterPiece(t);
    EXPECT_TRUE(e.cheminEstLibre(t, Position(0, 5)));
    e.enleverPieces();
}

TEST(ModeleTest, Cavalier_DeplacementInvalide) {
    using namespace Modele;
    Echiquier e;
    Cavalier* cavalier = new Cavalier(Position(3, 3), true);
    e.ajouterPiece(cavalier);
    EXPECT_FALSE(cavalier->estDeplacementValide(Position(4, 4)));
    e.enleverPieces();
    
}

TEST(ModeleTest, Tour_CheminBloque) {
    using namespace Modele;
    Echiquier e;
    Tour* tour = new Tour(Position(0, 0), true);
    Cavalier* cavalier = new Cavalier(Position(0, 1), true);
    e.ajouterPiece(tour);
    e.ajouterPiece(cavalier);
    EXPECT_FALSE(e.cheminEstLibre(tour, Position(0, 2)));
    e.enleverPieces();
}

TEST(ModeleTest, CaptureLegale) {
    using namespace Modele;
    Echiquier e;
    Tour* tour = new Tour(Position(0, 0), true);
    Cavalier* ennemi = new Cavalier(Position(0, 3), false);
    e.ajouterPiece(tour);
    e.ajouterPiece(ennemi);
    auto [ok, msg] = e.deplacerPiece(Position(0, 0), Position(0, 3));
    EXPECT_TRUE(ok);
    EXPECT_TRUE(msg.empty());
    e.enleverPieces();
}

TEST(ModeleTest, MouvementTemporaire_RetablitPosition) {
    using namespace Modele;
    Echiquier e;
    Roi* roi = new Roi(Position(4, 4), true);
    e.ajouterPiece(roi);
    {
        MouvementTemporaire mvt(&e, roi, Position(5, 5));
        EXPECT_EQ(roi->getPos(), Position(5, 5));
    }
    EXPECT_EQ(roi->getPos(), Position(4, 4));
    e.enleverPieces();
}

#endif