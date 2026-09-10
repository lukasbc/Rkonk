#pragma once

#include <string>

class Unite;
class TileMap;

/**
 * @class Joueur
 * @brief Classe Joueur.
 * 
 * Définit les informations d'un joueur lors d'une partie.
 */
class Joueur {
private:
    int id;                     ///< Identifiant du joueur.
    std::string nom;            ///< Nom/Pseudo du joueur.
    int pieces;                 ///< Pieces d'or du joueur.
    int couleur;                ///< Couleur des tuiles du joueur.

public:
    Joueur(int id, const std::string& nom, int couleur);    ///< Constructeur par défaut du Joueur.

    ~Joueur();                                              ///< Destructeur par défaut du Joueur.

    int getId() const;                                      ///< Renvoie l'id du joueur.
    const std::string& getNom() const;                      ///< Renvoie le nom du joueur.
    int getPieces() const;                                  ///< Renvoie le nombre de pièces d'or possédés par le joueur.

    int getCouleur() const;                                 ///< Renvoie la couleur des tuiles du joueur.
    void setCouleur(int newCouleur);                        ///< Attribue une couleur pour les tuiles d'un joueur.

    void setPieces(int newPieces);                          ///< Affecte le nombre de pieces d'or d'un joueur.
    void ajouterPieces(int pieces);                         ///< Fonction d'appel de piecesTerre.

    void piecesTerre(const TileMap& map);                   ///< Ajoute de l'or selon les Units présentes.

    void afficher() const;                                  ///< Affiche des infos sur un joueur.
};