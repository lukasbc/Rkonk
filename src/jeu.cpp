#include "../include/jeu.h"
#include "../include/match.h"
#include "../include/joueur.h"

/**
 * @brief Démarre une nouvelle partie.
 * @returns Un pointeur vers un Match.
 */
std::shared_ptr<Match> Jeu::nouvellePartie() {
    return std::make_shared<Match>(idSuivant++);
}

/**
 * @brief Permet de charger une partie existante selon l’id.
 * 
 * @param id L'identifiant de la partie souhaitée.
 * @returns Un pointeur vers une partie en cours.
 */
std::shared_ptr<Match> Jeu::continuerPartie(int id) const {
    return std::make_shared<Match>(id);
}