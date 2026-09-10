#pragma once

#include "unite.h"
#include <memory>

/**
 * @class Tour
 * @brief Classe de la tour.
 */
class Tour : public Unite {
public:
    Tour(int ligne, int colonne, std::shared_ptr<Joueur> possesseur);                   ///< Constructeur par défaut de la classe Tour.

    bool peutSeDeplacerVers(int, int, const TileMap&) const override { return false; }  ///< Si la tour peut se déplacer vers un endroit.
    bool seDeplacerVers(int, int, TileMap&) override { return false; }                  ///< Déplacement vers un endroit.

    bool peutAttaquer(int, int, const TileMap&) const override { return false; }        ///< Si la Tour peut attaquer.
    bool attaque(int, int, TileMap&, Match&) override { return false; }                 ///< La Tour attaque avec cette méthode.
};
