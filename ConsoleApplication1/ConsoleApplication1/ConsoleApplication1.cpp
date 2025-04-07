// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <set>

using namespace std;
class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        if (nums.empty()) return {};

        sort(nums.begin(), nums.end());  // Trie les éléments

        vector<int> dp(nums.size(), 1);  // dp[i] représente la taille du plus grand sous-ensemble divisible qui se termine par nums[i]
        vector<int> previous(nums.size(), -1);  // Pour reconstruire le sous-ensemble

        int maxSize = 1, maxIndex = 0;  // Taille et index du plus grand sous-ensemble

        for (int i = 1; i < nums.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] % nums[j] == 0 && dp[i] < dp[j] + 1) {
                    dp[i] = dp[j] + 1;
                    previous[i] = j;
                }
            }
            if (dp[i] > maxSize) {
                maxSize = dp[i];
                maxIndex = i;
            }
        }

        // Reconstruction du sous-ensemble à partir de previous
        vector<int> result;
        while (maxIndex != -1) {
            result.push_back(nums[maxIndex]);
            maxIndex = previous[maxIndex];
        }

        reverse(result.begin(), result.end());
        return result;
    }

    int main() {
        vector<int> nums = { 1, 2, 4, 8 };
        vector<int> result = largestDivisibleSubset(nums);

        for (int num : result) {
            cout << num << " ";
        }
        return 0;
    }
}; 

int main()
{
    Solution s;
    vector<int> v = { 1,2,3 };
    vector<int> reponse = s.largestDivisibleSubset(v);
    for (int i : reponse)
    {
        cout << i << " ";
    }
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
