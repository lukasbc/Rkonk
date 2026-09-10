#pragma once

#include "unite.h"
#include <memory>

/**
 * @class Villageois
 * @brief Villageois. Il village de villages en villages.
 */
class Villageois : public Unite {
public:
    Villageois(int ligne, int colonne, std::shared_ptr<Joueur> possesseur); ///< Constructeur par défaut du Villageois.
};