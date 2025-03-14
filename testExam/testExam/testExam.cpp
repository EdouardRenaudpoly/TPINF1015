#include <iostream>
#include <vector>
#include <memory>
using namespace std;
class Priorite {}; 
class Tache 
{
public: 
Tache(Priorite& priorite, const string& nom, const string& echeance = "sans echeance");
Tache(const Tache& tache);
void ajouterSousTache(const Tache& tache);
private:
string nom_; 
Priorite& priorite_;
string echeance_; 
vector<unique_ptr<Tache>> sousTaches_; 
}; 
class Projet 
{ 
public: 
	Projet(string nom); 
	void deplacerTache(const Tache& tacheParente, const Tache& sousTache);
	void ajouterTache(const Tache& tache);
	int trouverIndexTache(const Tache& tache); 
private: 
string nom_; 
vector<unique_ptr<Tache>> taches_; 
}; 
class Utilisateur 
{ 
public: 
//void ajouterProjet(const shared_ptr<Projet>& projet); 
private:
	vector<shared_ptr<Projet>> projets_; 
};


int main()
{
	shared_ptr<Projet>[3] projets;
	Priorite priorite1;
	Tache tache1(priorite1, "Revision Intra", "1 janvier 1970");
	Tache tache2(priorite1, "Composition & agregation", "1 janvier 1970");
	Projet projet("INF1015");
	projet.ajouterTache(tache1);
	projet.ajouterTache(tache2);
	projet.deplacerTache(tache1, tache2);
	cout << projet.getNbTaches() << endl; cout << projet[0].getNom() << endl;
	cout << projet[0].getNbSousTaches() << endl;
	cout << projet[0][0].getNom() << endl;
}