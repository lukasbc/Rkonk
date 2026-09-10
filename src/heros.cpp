#include "../include/heros.h"
#include <utility>

/**
 * @brief Constructeur par défaut du héros.
 * 
 * Hérite du constructeur par défaut de Unite.
 * 
 * @param ligne La ligne où va être positionné le héros.
 * @param colonne La colonne où va être positionné le héros.
 * @param possesseur Pointeur (avec compteurs de réfs) vers le Joueur auquel il appartient.
 */
Heros::Heros(int ligne, int colonne, std::shared_ptr<Joueur> possesseur)
    : Unite(ligne, colonne, /**<puissance*/4, /**<salaire=*/54, /**<cout*/80, std::move(possesseur),"Heros")
{}