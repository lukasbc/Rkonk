#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Tuile.h"
#include "joueur.h"
#include "entite.h"

/**
 * @class TileMap
 * @brief Map de tuiles. Une carte quoi.
 */
class TileMap {
    private:
        std::vector<std::vector<Tuile>> map;        ///< Vecteur contenant les tuiles (Tuile) de la map.
        int lignes;                                 ///< Nombre de lignes de la map.
        int colones;                                ///< Nombre de colonnes de la map.
    
    public:
        TileMap(int lignes, int colones);            ///< Constructeur par défaut de la map. 
        Tuile& get(int ligne, int colonne);          ///< Renvoie une tuile de la map à un endroit donné.
        const Tuile& get(int ligne, int colonne) const;     ///< Renvoie une tuile de la map à un endroit donné. (const) 
        bool isTerrain(int ligne, int colonne) const;       ///< Détermine si les coordonnées spécifiées sont du terrain libre ou non.
        int getLignes() const;                              ///< Renvoie le nombre de lignes de la map.
        int getColonnes() const;                            ///< Renvoie le nombre de colonnes de la map.
        std::vector<std::pair<int, int>> getVoisins(int ligne, int colonne) const;                              ///< Renvoie les tuiles voisines d'une tuile spécifique.
        bool sontConnectees(int ligne1, int colone1, int ligne2, int colone2, int possesseurId) const;          ///< Vérifie si deux tuiles sont connectées.

        bool chargerMapASCII(const std::string& nomFichier, std::vector<std::shared_ptr<Joueur>>& joueurs, std::vector<std::shared_ptr<Entite>>& nouvellesEntites); ///< Création de la Map à partir d'un fichier ASCII.
    }; 