#include "../include/tilemap.h"
#include "../include/tuile.h"
#include "../include/base.h"
#include "../include/joueur.h"
#include "../include/unite.h"
#include "../include/villageois.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <unordered_map>
#include <random>
#include <queue>
#include <set>


/**
 * @brief Création de la carte.
 * @param lignes Nombre de lignes de la carte.
 * @param colonnes Nombre de colonnes de la carte.
 */ 
TileMap::TileMap(int lignes, int colonnes) : lignes(lignes), colones(colonnes) {
    map.reserve(lignes);
    for (int i = 0; i < lignes; ++i) {
        std::vector<Tuile> tuilesLigne;
        tuilesLigne.reserve(colonnes);
        for (int j = 0; j < colonnes; ++j) {
            tuilesLigne.emplace_back(i, j);
        }
        map.push_back(std::move(tuilesLigne));
    }
}

/**
 * @brief Renvoie une tuile de la map à un endroit donné.
 * @param ligne La ligne spécifiée.
 * @param colonne La colonne spécifiée.
 * @returns Une réf vers une Tuile.
 */
Tuile& TileMap::get(int ligne, int colonne) {
    return map[ligne][colonne];
}

/**
 * @brief Renvoie une tuile de la map à un endroit donné. (version const)
 * @param ligne La ligne spécifiée.
 * @param colonne La colonne spécifiée.
 * @returns Une réf vers une Tuile constante.
 */
const Tuile& TileMap::get(int ligne, int colonne) const {
    return map[ligne][colonne];
}

/**
 * @brief Détermine si les coordonnées spécifiées sont du terrain libre ou non.
 * @param ligne La ligne spécifiée.
 * @param colonne La colonne spécifiée.
 * @returns true si la tuile est vide, false sinon.
 */
bool TileMap::isTerrain(int ligne, int colonne) const {
    if (map[ligne][colonne].terrain == TerrainType::None) {
        return true;
    } 
    else {
        return false;
    }
}

/**
 * @brief Renvoie le nombre de lignes de la map.
 * @returns Un entier.
 */
int TileMap::getLignes() const {
    return lignes;
}

/**
 * @brief Renvoie le nombre de colonnes de la map.
 * @returns Un entier.
 */
int TileMap::getColonnes() const {
    return colones;
}

/**
 * @brief Renvoie les tuiles voisines d'une tuile spécifique.
 * @param ligne La ligne spécifiée.
 * @param colonne La colonne spécifiée.
 * @returns Un vecteur de coordonnées x y.
 */
std::vector<std::pair<int, int>> TileMap::getVoisins(int ligne, int colonne) const {

    std::vector<std::pair<int, int>> decal;

    if (ligne % 2 == 0) {
        //Pair
        decal = {
            {-1,  0}, {-1, -1},
            { 0, -1}, { 0,  1},
            { 1,  0}, { 1, -1}
        };
    } else {
        //Impair
        decal = {
            {-1,  0}, {-1,  1},
            { 0, -1}, { 0,  1},
            { 1,  0}, { 1,  1}
        };
    }

    std::vector<std::pair<int, int>> voisins;

    for (std::size_t i = 0; i < decal.size(); ++i) {

        int decal_ligne = decal[i].first;
        int decal_colone = decal[i].second;


        int voisin_ligne = ligne + decal_ligne;
        int voisin_colone = colonne + decal_colone;

        if (voisin_ligne >= 0 && voisin_ligne< lignes && voisin_colone >= 0 && voisin_colone < colones) {
            voisins.push_back(std::make_pair(voisin_ligne, voisin_colone));
        }
    }
    

    return voisins;
}

/**
 * @brief Vérifie si deux tuiles sont connectées.
 * @param ligne1 La ligne de la première tuile.
 * @param colonne1 La colonne de la première tuile.
 * @param ligne2 La ligne de la deuxième tuile.
 * @param colonne2 La colonne de la deuxième tuile.
 * @param possesseurId L'id du possésseur.
 * @returns true si c'est le cas, false sinon.
 */
bool TileMap::sontConnectees(int ligne1, int colonne1, int ligne2, int colonne2, int possesseurId) const {
    if (ligne1 == ligne2 && colonne1 == colonne2) return true;

    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visite;

    q.push({ligne1, colonne1});
    visite.insert({ligne1, colonne1});

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int ligne = current.first;
        int colonne = current.second;

        for (const auto& voisin : getVoisins(ligne, colonne)) {
            int voisinLigne = voisin.first;
            int voisinColonne = voisin.second;

            if (voisinLigne == ligne2 && voisinColonne == colonne2) return true;

            const Tuile& t = map[voisinLigne][voisinColonne];
            if (visite.count({voisinLigne, voisinColonne}) == 0 && t.possesseurId == possesseurId && t.terrain == TerrainType::None) {
                visite.insert({voisinLigne, voisinColonne});
                q.push({voisinLigne, voisinColonne});
            }
        }
    }

    return false;
}

/** 
 * @brief Création de la Map à partir d'un fichier ASCII.
 * @param nomFichier Nom du fichier texte à charger.
 * @param joueurs Réference vers un vecteur de joueurs.
 * @param nouvellesEntites Réference vers un vecteur d'entités.
 * @returns true en cas de succès, false sinon.
*/
bool TileMap::chargerMapASCII(const std::string& nomFichier, std::vector<std::shared_ptr<Joueur>>& joueurs, std::vector<std::shared_ptr<Entite>>& nouvellesEntites) {
    std::ifstream file(nomFichier);
    if (!file) {
        map.clear();
        return false;
    }

    //Pour l'initialisation de base
    std::vector<std::tuple<int, int, int>> baseVoisinesAClaimer;

    //Verification si la base est deja mise
    std::unordered_map<int, bool> baseSet;

    std::vector<std::vector<Tuile>> newMap;
    std::string ligneTexte;
    int ligneIter = 0;
    bool erreur = false;

    while (std::getline(file, ligneTexte)) {
        std::vector<Tuile> tuilesLigne;
        for (size_t colone = 0; colone < ligneTexte.size(); ++colone) {
            char colonne = ligneTexte[colone];

            Tuile tuile(ligneIter, static_cast<int>(colone));

            if (colonne == '#') {
                tuile.terrain = TerrainType::Forest;
            } 
            else if (colonne == '.') {
                tuile.terrain = TerrainType::None;
            } 
            else if (std::isdigit(colonne)) {
                int playerId = colonne - '0';
            
                if (playerId > static_cast<int>(joueurs.size())) {
                    while (static_cast<int>(joueurs.size()) < playerId) {
                        int id = joueurs.size() + 1;
                        joueurs.emplace_back(
                            std::make_shared<Joueur>(id, "Joueur " + std::to_string(id),50));
                    }
                }
            
                if (baseSet[playerId]) {
                    std::cerr << "ERREUR: Joueur " << playerId << " a plusieurs bases!\n";
                    erreur = true;
                    break;
                }
                baseSet[playerId] = true;
            
                tuile.terrain = TerrainType::None;
                tuile.possesseurId = playerId;
            
                auto base = std::make_shared<Base>(ligneIter, static_cast<int>(colone), joueurs[playerId - 1]);
            
                tuile.occupant = base;
                nouvellesEntites.push_back(base);
            
                baseVoisinesAClaimer.emplace_back(ligneIter, static_cast<int>(colone), playerId);
            }
            else {
                tuile.terrain = TerrainType::Water;
            }

            tuilesLigne.push_back(std::move(tuile));
        }
        if (erreur) break;
        newMap.push_back(std::move(tuilesLigne));
        ligneIter++;
    }
    file.close();

    if (erreur) {
        map.clear();
        return false;
    }

    //Nouvelle map
    map = std::move(newMap);
    lignes = map.size();
    colones = (lignes > 0) ? map[0].size() : 0;

    std::mt19937 rng{std::random_device{}()};

    //Claimer les tuiles voisines aux bases
    for (const auto& info : baseVoisinesAClaimer) {
        int ligne = std::get<0>(info);
        int colonne = std::get<1>(info);
        int id = std::get<2>(info);

        std::vector<std::pair<int, int>> tuilesClaimables;
        auto voisins = getVoisins(ligne, colonne);
        for (const auto& voisin : voisins) {
            int voisinLigne = voisin.first;
            int voisinColonne = voisin.second;
    
            if (map[voisinLigne][voisinColonne].possesseurId == -1 && map[voisinLigne][voisinColonne].terrain == TerrainType::None) {
                map[voisinLigne][voisinColonne].possesseurId = id;
                tuilesClaimables.emplace_back(voisinLigne, voisinColonne);
            }
        }

        if (!tuilesClaimables.empty()) {
            std::uniform_int_distribution<std::size_t> dist(0, tuilesClaimables.size() - 1);
            std::pair<int, int> caseChoisie = tuilesClaimables[dist(rng)];
            int voisinLigne = caseChoisie.first;
            int voisinColonne = caseChoisie.second;
    
            auto villageois = std::make_shared<Villageois>(voisinLigne, voisinColonne, joueurs[id - 1]);
            map[voisinLigne][voisinColonne].occupant = villageois;
            nouvellesEntites.push_back(villageois);
    
        }
    }

    std::cout << "Carte charger!" << std::endl;
    return true;
}