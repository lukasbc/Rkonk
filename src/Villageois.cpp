#include "../include/villageois.h"
/**
 * @brief Constructeur par défaut du Villageois.
 * 
 * Hérite de Unity.
 * 
 * @param ligne La ligne où il va être positionné.
 * @param colonne La colonne où il va être positionné.
 * @param possesseur Pointeur (avec compteur) vers le Joueur auquel il va appartenir.  
 */
Villageois::Villageois(int ligne, int colonne, std::shared_ptr<Joueur> possesseur)
    : Unite(ligne, colonne, /**<puissance*/1, /**<salaire*/2, /**<cout*/10, std::move(possesseur),"Villageois")
{}