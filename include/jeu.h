#pragma once

#include <vector>
#include <memory>

class Match;

/**
 * @class Jeu
 * @brief Classe englobante contenant des parties.
 * 
 * Permet d'en créer de nouvelles ainsi que d'en continuer d'autres.
 */
class Jeu {
private:
    std::vector<std::shared_ptr<Match>> parties;
    int idSuivant = 1;                  ///< Identifiant de la partie suivante créee par la suite.
public:
    Jeu() = default;                    ///< Constructeur par défaut du Jeu.
    std::shared_ptr<Match> nouvellePartie();    ///< Création d'une nouvelle partie (Match).
    std::shared_ptr<Match> continuerPartie(int partieId) const; ///< Continuer une partie (Match).
    void recommencer(int partieId);                             ///< Recommencer une partie (Match).
    void bouclePrincipale();                                    ///< Boucle principale du Jeu.
};
