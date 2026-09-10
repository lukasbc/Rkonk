#pragma once

#include "unite.h"
#include <memory>

/** 
 * @class Piquier
 * @brief Classe du Piquier.
 * Hérite de Unite.
*/
class Piquier : public Unite {
public:
    Piquier(int ligne, int colonne, std::shared_ptr<Joueur> possesseur);    ///< Constructeur par défaut du Piquier.
};