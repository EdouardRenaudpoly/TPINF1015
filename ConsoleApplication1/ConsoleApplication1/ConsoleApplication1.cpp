// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <numeric>
using namespace std;
bool
ostream& operator<< (ostream& sortie, const GestionnaireArbitre& g)
{
    for (auto& arbitre : g.arbitres_)
    {
        sortie << arbitre->getNom() << "\t" << arbitre->getRole() << endl;
    }
    for (auto& paire : g.filtreArbitreRole_)
    {
        sortie << paire.first;
        for (auto& arbitre : paire.second)
        {
            sortie << "\t" << arbitre->getNom();
        }
        sortie << endl;
    }
    return sortie;
}

ajouterArbitre(Arbitre* a)
{
    if (getArbitreParNom(a->getNom()) != nullptr)
    {
        arbitres_.push_back(a);
        filtreArbitreRole_[a->getRole()].push_back(a);
        return true;
    }
    else
    {
        return false;
    }
}
supprimerArbitre(string nom)
{
    Arbitre* ptrArbitre = getArbitreParNom(nom);
    if (ptrArbitre!=nullptr)
    {
        auto it = arbitres_.find(ptrArbitre);
        arbitres_.erase(it);
        auto it = filtreArbitreRole_[a->getRole()].find(ptrArbitre);;
        filtreArbitreRole_[a->getRole()].erase(it);
        return true;
    }
    else
    {
        return false;
    }
}
auto getIteratorArbitreParNom(string nom)
{
    return find_if(arbitres_.begin(), arbitres_.end(), [=](Arbitre* arbitre) {return arbitre->getNom() == nom;});
}
struct comparerPersonne
{
    comparerPersonne() {}
    bool operator()(Personne* p1, Personne* p2)
    {
        return p1->getNom() > p2->getNom();
    }
};

class MatierePrecieuse
{
public:
    MatierePrecieuse() : prixParGramme_(0.0) {}
    ~MatierePrecieuse() = default;
    double getPrixParGramme()
    {
        return prixParGramme_;
    }
    void setPrixParGramme(double nouveauPrix)
    {
        prixParGramme_=nouveauPrix;
    }
    virtual string getNom() = 0;
    virtual bool estSynthetisable() = 0;
private:
    double prixParGramme_;
};

class Or24k : public MatierePrecieuse
{
public:
    Or24k() : MatierePrecieuse() {}

    string getNom() override
    {
        return "Or";
    }
    bool estSynthetisable() override
    {
        return false;
    }
};

using NomMatiere = std::string;
class Inventaire {
public:
    Inventaire() = default;
    ~Inventaire() = default;
    void ajouterMatiere(NomMatiere nom, double grammes)
    {
        grammesDeMatieres_[nom] = grammes;
    }
    double prixTotal(const std::map<NomMatiere, MatierePrecieuse*> matieres) const;
private:
    map<NomMatiere, double> grammesDeMatieres_;
};

double trouverPrixMatiere(const map<NomMatiere, MatierePrecieuse*>& matieres, pair<NomMatiere,double> matiereEtGrammes)
{
    auto it = matieres.find(matiereEtGrammes.first);
    if (it != matieres.end())
    {
        return it->second->getPrixParGramme()*matiereEtGrammes.second;
    }
    else
    {
        return 0.0;
    }
}
double Inventaire::prixTotal(const map<NomMatiere, MatierePrecieuse*> matieres) const
{
    return transform_reduce(matieres.begin(), matieres.end(),
        0.0, plus{},
        [&](pair<NomMatiere,MatierePrecieuse*> paire) { return trouverPrixMatiere(matieres,make_pair(paire.first, grammesDeMatieres_.at(paire.first)
        )); });
}

int main() 
{
    Inventaire p;
    Or24k m; 
    m.setPrixParGramme(10);
    Or24k o;
    o.setPrixParGramme(2);
    p.ajouterMatiere(m.getNom(), 40);
    p.ajouterMatiere(o.getNom(), 10);
    map<string, MatierePrecieuse*> matieres = {
    { m.getNom(), &m }, { o.getNom(), &o } };
    cout <<  p.prixTotal(matieres) << "\n";
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
