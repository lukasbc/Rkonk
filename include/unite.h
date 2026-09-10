#pragma once

#include "entite.h"
#include "tilemap.h"
#include "joueur.h"
#include <memory>
#include <string>

class Match;

/**
 * @class Unite
 * @brief Définit une classe plus concrète que Entite avec méthodes lui permettant
 * de se déplacer, d'attaquer, de fusionner avec une autre unité, etc...
 */

class Unite : public Entite , public std::enable_shared_from_this<Unite>{
public:
    Unite(int ligne, int colonne, int puissance, int salaire, int prix, std::shared_ptr<Joueur> possesseur , const std::string& nom);


    int getPuissance() const;           ///< Renvoie les points d'attaque.
    void setPuissance(int p);           ///< Attribue les points d'attaque de l'unité.

 
    int getSalaire() const;             ///< Renvoie le salaire par tour.
    void setSalaire(int s);             ///< Attribue le salaire de l'unité. 

    int getPrix() const;                ///< Renvoie le prix d'achat de l'unité.

    bool aDejaFaitAction() const;       ///< Renvoie true si l'unité a déjà effectué une action pendant le tour courant.
    void faitAction();                  ///< Dit à l'unité qu'elle a effectué une action pendant le tour courant.
    void reinitialiserTour();           ///< Dit à l'unité qu'elle a réinitialisé ses actions pendant le tour courant.

    virtual bool peutSeDeplacerVers(int newLigne, int newColonne, const TileMap& map) const;           ///< Fonction renvoyant true si le déplacement est autorisé à certaines coordonnées de la map.
    virtual bool seDeplacerVers(int newLigne, int newColonne, TileMap& map);                          ///< Permet de se déplacer dans la map.

    virtual bool peutFusionner(const Unite& other) const;                                ///< Fonction renvoyant true si la fusion est possible avec une autre entitée.
    virtual bool fusionner(Unite& other, Match& match);                                  ///< Permet de se déplacer dans la map.

    virtual bool peutAttaquer(int targetLigne, int targetColonne, const TileMap& map) const;  ///< Fonction renvoyant true si l'on peut attaquer à des coordonnées de la map.
    virtual bool attaque(int targetLigne, int targetColonne, TileMap& map, Match& match);     ///< Permet d'attaquer dans la map.

    virtual void deplacement(TileMap& map);                                             ///< Méthode de déplacement. Surchargeable.


    std::shared_ptr<Joueur> getPossesseur() const;                                      ///< Renvoie le Joueur auquel il appartient.
    const std::string& getType() const;                                                 ///< Renvoie le nom, ou classe de l'unité. (ex: "Piquier").

    

private:
    int mPuissance;         ///< Points de puissance.
    int mSalaire;           ///< Salaire que demande l'unité.
    int mPrix;              ///< Prix de l'unité.
    bool action = false;    ///< Si action a eu lieu.

protected:
    std::string nomUnit; ///< Nom de l'unité (surchargeable, permet de renvoyer sa classe).
};

