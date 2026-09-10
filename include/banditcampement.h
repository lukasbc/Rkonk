#pragma once
#include "entite.h"
#include <memory>

class Match;  

/**
 * @class BanditCampement
 * @brief Classe du campement de bandits. 
 */
class BanditCampement : public Entite {
    int stockPieces = 0;                              ///< Le stock de pièces d'or du campement.
public:
    BanditCampement(int ligne, int colonne)       ///< Constructeur par défaut du campement.
      : Entite(ligne, colonne,{}) {}

    void ajouterButin(int montant);               ///< Ajoute des pièces d'or au campement.
    
    bool essayerReproduction(Match& match);       ///< Génère des bandits selon le stock de pièces d'or.
    int getStock() const { return stockPieces; }      ///< Renvoie le stock de pièces d'or du campement.
};
