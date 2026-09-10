#pragma once

#include <memory>

class Joueur;

/**
 * @class Entite
 * @brief Une classe abstraite définissant une entitée quelconque sur la map.
 * 
 * Elle vient englober un tas de comportements communs à toutes ses sous-classes
 * telles que Piquier, Chevalier et Heros.
 */
class Entite {
private:

public:
/**
 * @brief Le constructeur par défaut de Entite.
 */
Entite(int ligne, int colonne, std::weak_ptr<Joueur> possesseur)
: mLigne(ligne), mColonne(colonne), mPossesseur(possesseur) {}

    virtual ~Entite() = default;        ///< Destructeur par défaut de Entite.

    /** 
     * @brief Retourne la ligne dans laquelle se situe l'entitée.
     * @returns Un entier (int).
     */
    int getLigne() const { return mLigne; } 
    
    /** 
     * @brief Retourne la colonne dans laquelle se situe l'entitée.
     * @returns Un entier (int).
     */
    int getColonne() const { return mColonne; } 

    /**
     * @brief Renvoie le joueur auquel il appartient.
     * @returns Un weak_ptr vers le Joueur.
     */
    std::weak_ptr<Joueur> getPossesseur() const { return mPossesseur; }

protected:
    int mLigne,                             ///< La colonne où se situe l'entité.
        mColonne;                           ///< La colonne où se situe l'entité.
    std::weak_ptr<Joueur> mPossesseur;      ///< Le weak_ptr du Joueur auquel appartient l'entité.
};