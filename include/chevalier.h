#pragma once

#include "unite.h"
#include <memory>


/**
 * @class Chevalier
 * @brief Classe du Chevalier.
 */
class Chevalier : public Unite {
public:
    Chevalier(int ligne, int colonne, std::shared_ptr<Joueur> possesseur);  ///< Constructeur par défaut de la Base.
};