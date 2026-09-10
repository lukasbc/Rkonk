#include "../include/tour.h"
/**
 * @brief Constructeur par défaut de la classe Tour.
 * @param ligne La ligne où va être positionné la tour.
 * @param colonne La colonne où va être positionné la tour.
 * @param possesseur Pointeur (avec compteurs de réfs) vers le Joueur auquel la tour appartient.
 */
Tour::Tour(int ligne, int colonne, std::shared_ptr<Joueur> possesseur)
    : Unite(ligne, colonne, /*puissance=*/2, /*salaire=*/2, /*cout=*/20, std::move(possesseur),"Tour")
{}