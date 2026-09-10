#pragma once

#include "unite.h"
#include <memory>

/**
 * @class Heros
 * @brief Classe du Héros.
 * 
 * Hérite de Unite.
 */
class Heros : public Unite {
public:
    Heros(int ligne, int colonne, std::shared_ptr<Joueur> possesseur);  ///< Constructeur par défaut du Heros.
};