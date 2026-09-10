#include "../include/banditcampement.h"
#include "../include/match.h"
#include "../include/bandit.h"

/**
 * @brief Ajoute de l'or au butin du campement.
 * @param montant Quantité d'or à ajouter.
 */
void BanditCampement::ajouterButin(int montant) {
    stockPieces += montant;
}

/**
 * @brief Tente d'ajouter un nouveau bandit au campement.
 * @param match Le match auquel le bandit sera ajouté.
 * @returns true en cas de succès (assez d'or), false sinon.
 */
bool BanditCampement::essayerReproduction(Match& match) {
    if (stockPieces < 3) return false;
    stockPieces -= 3;

    auto b = std::make_shared<Bandit>(getLigne(), getColonne());
    match.ajouterEntite(b);

    match.getTileMap().get(getLigne(), getColonne()).possesseurId = BANDIT_ID;
    return true;
}
