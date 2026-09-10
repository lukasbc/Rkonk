#include "../include/tuile.h"
#include <iostream>

/**
 * @brief Constructeur par défaut de la classe Tuile.
 * Une tuile dans une map.
 * @param ligne Ligne où se situe la tuile.
 * @param colonne Colonne où se situe la tuile.
 */
Tuile::Tuile(int ligne, int colonne)
    : ligne(ligne), colonne(colonne) {}