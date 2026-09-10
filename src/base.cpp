#include "../include/base.h"
#include <utility>

/**
 * @brief Constructeur par défaut de Base.
 * 
 * Hérite du constructeur de Entite.
 * 
 * @param ligne La ligne où va être positionné la base.
 * @param colonne La colonne où va être positionné la base.
 * @param possesseur Pointeur (avec compteurs de réfs) vers le Joueur auquel la base appartient.
 */
Base::Base(int ligne, int colonne, std::weak_ptr<Joueur> possesseur)
    : Entite(ligne, colonne, std::move(possesseur))
{}