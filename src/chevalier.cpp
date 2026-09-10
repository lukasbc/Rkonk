#include "../include/chevalier.h"


/**
 * @brief Constructeur par défaut du chevalier.
 * 
 * @param ligne La ligne sur laquelle il va être placé.
 * @param colonne La colonne sur laquelle il va être placé.
 * @param possesseur Pointeur vers le joueur auquel il appartiendra.
 */
Chevalier::Chevalier(int ligne, int colonne, std::shared_ptr<Joueur> possesseur)
    : Unite(ligne, colonne, /*puissance=*/3, /*salaire=*/18, /*cout=*/40, std::move(possesseur),"Chevalier")
{}