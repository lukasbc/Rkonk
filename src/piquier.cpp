#include "../include/piquier.h"
#include <utility>

/**
 * @brief Constructeur par défaut du Piquier (oui, c'est un mot français qui existe).
 * Hérite du constructeur de Unite.
 * 
 * @param ligne La ligne où va être positionné l'unité.
 * @param colonne La colonne où va être positionné l'unité.
 * @param possesseur Pointeur (avec compteurs de réfs) vers le Joueur auquel il appartient.
 */
Piquier::Piquier(int ligne, int colonne, std::shared_ptr<Joueur> possesseur)
    : Unite(ligne, colonne, /*puissance=*/2, /*salaire=*/6, /*cout=*/20, std::move(possesseur),"Piquier")
{}