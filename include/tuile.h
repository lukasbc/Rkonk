#pragma once

#include <memory>
#include "entite.h"

/**
 * @enum TerrainType
 * @brief Définit des types de terrains pour la map.
 */
enum class TerrainType {
    None,
    Forest,
    Water
};

/**
 * @class Tuile
 * @brief Une Tuile de la map. En forme hexagonale.
 */
class Tuile {
public:
    Tuile(int ligne, int colonne);

    int ligne;                                      ///< La ligne sur laquelle se situe la Tuile.
    int colonne;                                    ///< La ligne sur laquelle se situe la Tuile.

    int possesseurId = -1;                          ///< L'id du joueur qui possède la tuile, initialisé à -1 (personne).

    TerrainType terrain = TerrainType::None;        ///< Type du terrain de la Tuile.

    std::weak_ptr<Entite> occupant;                 ///< l'entité qui occupe la Tuile.

    bool hasBarriere = false;                       ///< Si la Tuile possède des barrières (en bois).
};
