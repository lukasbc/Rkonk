#pragma once

#include <memory>
#include "entite.h"


/**
 * @class Base
 * @brief Base du Joueur.
 * 
 * A la manière d'une forteresse.
 */
class Base : public Entite {
public:
    Base(int ligne, int colonne, std::weak_ptr<Joueur> possesseur);     ///< Constructeur par défaut de la Base.
};