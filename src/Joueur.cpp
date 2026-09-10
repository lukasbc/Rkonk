#include "../include/joueur.h"
#include "../include/tilemap.h"
#include <iostream>

/**
 * @brief Destructeur par défaut de la classe Joueur.
 */
Joueur::~Joueur() = default;

/**
 * @brief Constructeur par défaut du joueur.
 * @param id Identifiant du joueur
 * @param nom Nom/Pseudo du joueur
 * @param couleur Couleur des tuiles du joueur
 */
Joueur::Joueur(int id, const std::string& nom, int couleur)
    : id(id), nom(nom), pieces(0), couleur(couleur) {
    std::cout << "Joueur creer " << nom << " (ID: " << id << ")" << std::endl;       // Message de début indiquant la création avec succès.
}

/**
 * @brief Renvoie l'id du joueur.
 * @returns Un entier.
 */
int Joueur::getId() const {
    return id;
}

/**
 * @brief Renvoie le nom du joueur.
 * @returns Une chaîne de caractères const.
 */
const std::string& Joueur::getNom() const {
    return nom;
}

/**
 * @brief Renvoie le nombre de pièces d'or possédés par le joueur.
 * @returns Un entier.
 */
int Joueur::getPieces() const {
    return pieces;
}

/**
 * @brief Renvoie la couleur des tuiles du joueur.
 * @returns Un entier.
 */
int Joueur::getCouleur() const {
    return couleur;
}

/**
 * @brief Permet d'attribuer une couleur pour les tuiles d'un joueur.
 * @param newCouleur Couleur (int) à affecter.
 */
void Joueur::setCouleur(int newCouleur) {
    couleur = newCouleur;
}

/**
 * @brief Permet d'affecter le nombre de pieces d'or d'un joueur.
 * @param newPieces Nouveau nombre de pieces d'or du joueur.
 */
void Joueur::setPieces(int newPieces) {
    pieces = newPieces;
}

/**
 * @brief Fonction d'appel de piecesTerre.
 * Ajoute des points convertis en pieces d'or (?).
 * 
 * @param points Points/Pieces d'or à ajouter.
 */
void Joueur::ajouterPieces(int points) {
    pieces += points;
}

/**
 * @brief Ajoute des points en pieces d'or selon les Units présentes sur les tuiles à chaque tour.
 * @param map La TileMap sur laquelle le joueur va gagner des points.
 */
void Joueur::piecesTerre(const TileMap& map) {
    int revenu = 0;

    for (int ligne = 0; ligne < map.getLignes(); ++ligne) {
        for (int colonne = 0; colonne < map.getColonnes(); ++colonne) {
            const Tuile& t = map.get(ligne, colonne);
            if (t.possesseurId == id) {
                revenu++;
            }
        }
    }

    std::cout << "Joueur " << nom << " gagne +" << revenu << " or de tuiles\n";

    ajouterPieces(revenu);
}


/**
 * @brief Affiche des infos sur un joueur.
 * Printe son nom, son id et son score (nombre de pieces d'or).
 */
void Joueur::afficher() const {
    std::cout << "Joueur : " << nom << " (ID: " << id << "), Score: " << pieces << std::endl;
}
