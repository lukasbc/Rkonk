#pragma once

#include "unite.h"

class TileMap;

constexpr int BANDIT_ID = 99;                                           ///< Définit l'id du bandit.

/**
 * @class Bandit
 * @brief Classe du Bandit. N'obéit à personne.
 */
class Bandit : public Unite{
public:
    Bandit(int ligne, int colonne);                                                             ///< Constructeur par défaut du bandit.
    void deplacement(TileMap& map) override;                                                    ///< Méthode de déplacement du bandit.
    bool peutSeDeplacerVers(int newLigne, int newColonne, const TileMap& map) const override;   ///< Si le bandit peut se déplacer à un endroit donné.
};