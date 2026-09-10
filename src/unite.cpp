#include "../include/unite.h"
#include "../include/joueur.h"
#include "../include/match.h"
#include "../include/piquier.h"
#include "../include/chevalier.h"
#include "../include/heros.h"
#include "../include/banditcampement.h"
#include <iostream>

/**
 * @brief Constructeur par défaut de Unite.
 * @param ligne La ligne où va être positionné l'unité.
 * @param colonne La colonne où va être positionné l'unité.
 * @param puissance Les points d'attaque de l'unité.
 * @param salaire Le salaire que demande l'unité. Peut faire la grève (ou une mutinerie) s'il n'est pas assez payé.
 * @param prix Le prix de l'unité à l'achat.
 * @param possesseur Pointeur (avec compteurs de réfs) vers le Joueur auquel il appartient.
 * @param nom Nom de l'unité. Référence vers une chaîne de caractères constante.
 */
Unite::Unite(int ligne, int colonne, int puissance, int salaire, int prix, std::shared_ptr<Joueur> possesseur, const std::string& nom)
    : Entite(ligne, colonne, possesseur),
      mPuissance(puissance),
      mSalaire(salaire),
      mPrix(prix),
      nomUnit(nom)
{}

/**
 * @brief Renvoie les points d'attaque.
 */
int Unite::getPuissance() const {
    return mPuissance;
}

/**
 * @brief Attribue les points d'attaque de l'unité.
 * @param p Nouveaux points d'attaque de l'unité.
 */
void Unite::setPuissance(int p) {
    mPuissance = p;
}

/**
 * @brief Renvoie le salaire par tour.
 */
int Unite::getSalaire() const {
    return mSalaire;
}

/**
 * @brief Attribue le salaire de l'unité. 
 * @param s Nouveau salaire de l'unité.
 */
void Unite::setSalaire(int s) {
    mSalaire = s;
}


/**
 * @brief Renvoie le prix d'achat de l'unité.
 * Ne change pas au cours d'une partie. Chaque classe a un prix fixé.
 */
int Unite::getPrix() const {
    return mPrix;
}

/**
 * @brief Renvoie le joueur auquel appartient l'unité.
 * @returns Un pointeur (avec compteur de réfs) vers un Joueur.
 */
std::shared_ptr<Joueur> Unite::getPossesseur() const {
    return mPossesseur.lock();
}

/**
 * @brief Renvoie true si l'unité a déjà effectué une action pendant le tour courant.
 * @returns Un booléen.
 */
bool Unite::aDejaFaitAction() const {
    return action;
}

/**
 * @brief Dit à l'unité qu'elle a effectué une action pendant le tour courant.
 * Met l'attribut "action" de l'unité à true.
 */
void Unite::faitAction() {
    action = true;
}

/**
 * @brief Dit à l'unité qu'elle a réinitialisé ses actions pendant le tour courant.
 * Met l'attribut "action" de l'unité à false.
 */
void Unite::reinitialiserTour() {
    action = false;
}

/**
 * @brief Renvoie true si l'unité peut se déplacer à un endroit précis dans la map.
 * @param newLigne La ligne spécifiée.
 * @param newColonne La colonne spécifiée.
 * @param map La map dans laquelle l'unit veut se déplacer. Référence vers une TileMap constante.
 * @returns Un booléen, indiquant si le déplacement est possible.
 */
bool Unite::peutSeDeplacerVers(int newLigne, int newColonne, const TileMap& map) const
{
    const Tuile& tileCible = map.get(newLigne, newColonne);
    auto ownerPtr = getPossesseur();
    if (!ownerPtr) return false;
    int possesseurId = ownerPtr->getId();

    if (tileCible.possesseurId != possesseurId && tileCible.occupant.expired()) {
        for (auto [voisinLigne, voisinColonne] : map.getVoisins(newLigne, newColonne)) {
            const Tuile& voisin = map.get(voisinLigne, voisinColonne);
            if (auto def = std::dynamic_pointer_cast<Unite>(voisin.occupant.lock())) {
                if (def->getPossesseur()
                 && def->getPossesseur()->getId() == tileCible.possesseurId
                 && def->getPuissance() >= this->getPuissance())
                {
                    return false;
                }
            }
        }
    }

    if (tileCible.hasBarriere && getPuissance() <= 2) {
        if (tileCible.possesseurId != possesseurId) return false;
    }
    if (tileCible.terrain != TerrainType::None) {
        return false;
    }

    if (!ownerPtr) return false;


    if (tileCible.possesseurId == possesseurId && tileCible.occupant.expired()) {
        return true;
    }

    if (tileCible.possesseurId != possesseurId && tileCible.occupant.expired()) {
        for (auto [voisinLigne, voisinColonne] : map.getVoisins(newLigne, newColonne)) {
            if (map.get(voisinLigne, voisinColonne).possesseurId == possesseurId) {
                return true;
            }
        }
    }

    return false;
}



/**
 * @brief Déplacement de l'unité vers des coordonnées spécifiques dans une map.
 * @param newLigne La ligne spécifiée.
 * @param newColonne La colonne spécifiée.
 * @param map La map dans laquelle l'unit veut se déplacer. Référence vers une TileMap.
 */
bool Unite::seDeplacerVers(int newLigne, int newColonne, TileMap& map)
{
    if (!peutSeDeplacerVers(newLigne, newColonne, map)) {
        return false;
    }

    Tuile& oldTile = map.get(getLigne(), getColonne());
    Tuile& newTile = map.get(newLigne, newColonne);

    auto ownerPtr = getPossesseur();
    bool sameOwnerBefore = ownerPtr && (newTile.possesseurId == ownerPtr->getId());

    oldTile.occupant.reset();

    mLigne = newLigne; 
    mColonne = newColonne;

    if (ownerPtr && newTile.occupant.expired() && newTile.possesseurId != ownerPtr->getId()) {
        newTile.possesseurId = ownerPtr->getId();
        std::cout << "Tuile capturée en (" << newLigne << "," << newColonne << ")\n";
    }

    newTile.occupant = shared_from_this();

    if (!sameOwnerBefore) {
        faitAction();
    }

    return true;
}

/**
 * @brief Renvoie le type de l'unité (son nom, ex: "Piquier").
 * @returns Une réf vers un string const.
 */
const std::string& Unite::getType() const {
    return nomUnit;
}

/**
 * @brief Indique si les deux unités appartiennent au même joueur.
 * @param other Une réf vers une Unite const.
 * @returns Renvoie true si colonne'est le cas, false sinon.
 */
bool Unite::peutFusionner(const Unite& other) const {
    if (getPossesseur() == other.getPossesseur() && typeid(*this) == typeid(other)) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Effectue le test ainsi que la fusion avec une autre unité.
 * @param other L'unité avec laquelle on souhaite fusionner. Réf vers une Unite.
 * @param match La partie dans laquelle a lieu la fusion. Réf vers un Match.
 * @returns true si la fusion est possible et menée avec succès, false sinon.
 */
bool Unite::fusionner(Unite& other, Match& match) {
    if (!peutFusionner(other)) {
        return false;
    }

    if (!match.getTileMap().sontConnectees(getLigne(), getColonne(), other.getLigne(), other.getColonne(), getPossesseur()->getId())) {
        return false;
    }

    std::shared_ptr<Unite> fusionnee;
    if (getType() == "Villageois" && other.getType() == "Villageois") {
        fusionnee = std::make_shared<Piquier>(other.getLigne(),
                                              other.getColonne(),
                                              getPossesseur());
    }
    else if (getType() == "Piquier" && other.getType() == "Piquier") {
        fusionnee = std::make_shared<Chevalier>(other.getLigne(),
                                             other.getColonne(),
                                             getPossesseur());
    }
    else if (getType() == "Chevalier" && other.getType() == "Chevalier") {
        fusionnee = std::make_shared<Heros>(other.getLigne(),
                                           other.getColonne(),
                                           getPossesseur());
    }
    else {
        return false;
    }

    match.supprimerEntite(shared_from_this());
    match.supprimerEntite(other.shared_from_this());
    match.ajouterEntite(fusionnee);

    std::cout << "Fusion de deux " << getType()<< " en un " << fusionnee->getType() << "\n";
    return true;
}



/**
 * @brief Permet de savoir si l'unité peut attaquer les contenus d'une tuile de la map.
 * @param tr La ligne où l'unité veut attaquer.
 * @param tc La colonne où l'unité veut attaquer.
 * @param map La map où l'unité veut attaquer. Réf vers une TileMap const.
 * @returns true si l'attaque est possible, false sinon.
 */
bool Unite::peutAttaquer(int tr, int tc, const TileMap& map) const
{
    const Tuile& tileCible = map.get(tr, tc);
    if (tileCible.occupant.expired()) {
        return false;
    }

    auto ent = tileCible.occupant.lock();
    auto cibleUnite = std::dynamic_pointer_cast<Unite>(ent);
    auto cibleCamp  = std::dynamic_pointer_cast<BanditCampement>(ent);
    if (!cibleUnite && !cibleCamp) {
        return false;
    }

    auto myOwner = getPossesseur();
    if (!myOwner) {
        return false;
    }
    int myId = myOwner->getId();

    if (cibleUnite
     && cibleUnite->getPossesseur()
     && cibleUnite->getPossesseur()->getId() == myId)
    {
        return false;
    }

    bool inRange = false;
    for (auto [vl, vc] : map.getVoisins(tr, tc)) {
        if (map.get(vl, vc).possesseurId == myId) {
            inRange = true;
            break;
        }
    }
    if (!inRange) {
        return false;
    }

    if (tileCible.terrain != TerrainType::None) {
        return false;
    }

    if (tileCible.hasBarriere && getPuissance() < 2
     && tileCible.possesseurId != myId)
    {
        return false;
    }

    if (cibleUnite) {
        for (auto [vl, vc] : map.getVoisins(tr, tc)) {
            auto def = std::dynamic_pointer_cast<Unite>(map.get(vl, vc).occupant.lock());
            if (def
             && def->getPossesseur()
             && def->getPossesseur()->getId() == tileCible.possesseurId
             && def->getPuissance() >= this->getPuissance())
            {
                return false;
            }
        }
        if (getPuissance() < cibleUnite->getPuissance()) {
            return false;
        }
    }

    return true;
}



/**
 * @brief Permet à l'unité d'attaquer.
 * @param tr La ligne où l'unité veut attaquer.
 * @param tc La colonne où l'unité veut attaquer.
 * @param map La map où l'unité veut attaquer. Réf vers une TileMap const.
 * @param match Le Match (ou partie) dans laquelle va avoir lieu l'attaque.
 * @returns true si l'attaque est possible et menée avec succès, false sinon.
 */
bool Unite::attaque(int tr, int tc, TileMap& map, Match& match) {

    if (!peutAttaquer(tr, tc, map)) {
        return false;
    }

    Tuile& t = map.get(tr, tc);
    auto ent = t.occupant.lock();
    if (!ent) {
        return false;
    }

    if (auto camp = std::dynamic_pointer_cast<BanditCampement>(ent)) {

        match.supprimerEntite(camp);
        t.occupant.reset();

        t.possesseurId = getPossesseur()->getId();
    }
    else if (auto cible = std::dynamic_pointer_cast<Unite>(ent)) {

        match.supprimerEntite(cible);
        t.occupant.reset();

        t.possesseurId = getPossesseur()->getId();
    }
    else {
        return false;
    }

    faitAction();
    return true;
}


/** @brief Méthode de déplacement de l'unité. Peut être surchargé au besoin. */
void Unite::deplacement(TileMap& /*map*/) {}