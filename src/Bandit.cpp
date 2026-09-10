#include "../include/bandit.h"
#include "../include/tilemap.h"
#include <random>
#include <algorithm>

/**
 * @brief Construction par défaut de la classe Bandit.
 * 
 * Hérite du constructeur de Unite.
 * Le pointeur `possesseur` est initialisé à nullptr (puisque le bandit n'appartient à personne).
 * 
 * @param ligne La ligne où va être positionné le bandit.
 * @param colonne La colonne où va être positionné le bandit.
 */
Bandit::Bandit(int ligne, int colonne)
  : Unite(ligne, colonne,/*puissance=*/0,/*salaire=*/0,/*prix=*/0,/*possesseur=*/nullptr,/*name=*/"Bandit")
{}

/**
 * @brief Surcharge de la méthode bool Unite::peutSeDeplacerVers() à la classe Bandit.
 * 
 * @param ligne La ligne spécifiée.
 * @param colonne La colonne spécifiée.
 * @param map La map dans laquelle le bandit veut se déplacer. Référence vers une TileMap constante.
 * @returns Un booléen, indiquant si le déplacement est possible.
 */
bool Bandit::peutSeDeplacerVers(int ligne, int colonne, const TileMap& map) const {
    const Tuile& t = map.get(ligne, colonne);
    return (t.terrain == TerrainType::None && t.occupant.expired());
}

/**
 * @brief Permet au bandit de se déplacer d'une case au hasard dans la map si celle-ci est innocuppée.
 * @param map La map dans laquelle le déplacement va être effectué.
 */
void Bandit::deplacement(TileMap& map) {

    if (map.get(getLigne(), getColonne()).hasBarriere) {
        return;
    }

    auto voisins = map.getVoisins(getLigne(), getColonne());
    
    // Déplacement aléatoire
    static std::mt19937 rng{std::random_device{}()};
    std::shuffle(voisins.begin(), voisins.end(), rng);
    for (const auto& coord : voisins) {
        int ligne = coord.first;
        int colonne = coord.second;
        if (map.get(ligne, colonne).terrain == TerrainType::None && map.get(ligne, colonne).occupant.expired() && map.get(ligne, colonne).hasBarriere == false) {
            seDeplacerVers(ligne, colonne, map);
            map.get(ligne, colonne).possesseurId = BANDIT_ID;
            break;
        }
    }
}