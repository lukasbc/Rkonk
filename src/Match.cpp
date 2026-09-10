#include "../include/match.h"
#include "../include/villageois.h"
#include "../include/tour.h"
#include "../include/chevalier.h"
#include "../include/heros.h"
#include "../include/bandit.h"
#include "../include/piquier.h"
#include "../include/unite.h"
#include "../include/base.h"
#include "../include/banditcampement.h"

#include <iostream>
#include <cmath>
#include <algorithm>

/**
 * @brief Constructeur par défaut d'une partie.
 * @param id_ Identifiant de la partie.
 */
Match::Match(int id_) : id(id_), tileMap(10, 10) {
    std::cout << "Match " << id << " cree" << std::endl;

    
}

/**
 * @brief Renvoie l'identifiant de la partie.
 * @returns L'id de la partie.
 */
int Match::getId() const {
    return id;
}

/**
 * @brief Initialise la partie au tour 1.
 */
void Match::demarrerPartie() {
    if (joueurs.empty()) return;
    partieTerminee = false;
    tour   = 1;
    joueurActif = joueurs[0];

    std::cout << "\n=== TOUR " << tour
              << " : " << joueurActif->getNom() << " ===\n";
    piecesTuile(joueurActif);
    paimentSalaire(joueurActif);
    resetActionJoueur(joueurActif);
    updateBarrieres();
}

/**
 * @brief Mise à jour des barrières joueurs, détection des campements et mise à jour des bandits.
 * Et mise à jour du delta t de l'animation.
 */
void Match::update(sf::Time dt) {
    updateBarrieres();
    detecterCampements();
    updateBandit();
    _tempsAnim += dt.asSeconds();
}






/**
 * @brief Fonction dessinant la partie à l'écran.
 * @param win Fenêtre de rendu SFML.
 */
void Match::render(sf::RenderWindow& win) {
    static sf::Texture texHexGrass, texHexForest, texHexBarrier;
    static sf::Texture texVillageois, texVillage, texTower, texPikeman, texKnight, texHero, texBandit, texBanditCampement;
    static sf::Texture texHexJoueur1, texHexJoueur2,texHexJoueur3, texHexJoueur4,texHexJoueur5, texHexJoueur6,texHexJoueur7, texHexJoueur8,texHexJoueur9, texHexJoueur10, texHexBandit;
    static bool loaded = false;
    static sf::Texture* texHexOwners[11] = { nullptr };
    if (!loaded) {
        if (!texHexGrass   .loadFromFile("../assets/sprites/Tile_Default.png") ||
            !texHexForest  .loadFromFile("../assets/sprites/Tile_Tree.png") ||
            !texHexBarrier .loadFromFile("../assets/sprites/Tile_Barrier.png") ||
            !texHexJoueur1.loadFromFile("../assets/sprites/Tile_Player1.png") ||
            !texHexJoueur2.loadFromFile("../assets/sprites/Tile_Player2.png") ||
            !texHexJoueur3.loadFromFile("../assets/sprites/Tile_Player3.png") ||
            !texHexJoueur4.loadFromFile("../assets/sprites/Tile_Player4.png") ||
            !texHexJoueur5.loadFromFile("../assets/sprites/Tile_Player5.png") ||
            !texHexJoueur6.loadFromFile("../assets/sprites/Tile_Player6.png") ||
            !texHexJoueur7.loadFromFile("../assets/sprites/Tile_Player7.png") ||
            !texHexJoueur8.loadFromFile("../assets/sprites/Tile_Player8.png") ||
            !texHexJoueur9.loadFromFile("../assets/sprites/Tile_Player9.png") ||
            !texHexJoueur10.loadFromFile("../assets/sprites/Tile_Player10.png") ||
            !texHexBandit.loadFromFile("../assets/sprites/Tile_Bandit.png"))
        {
            throw std::runtime_error("Impossible de charger une texture hexagonale");
        }
        if (!texVillageois.loadFromFile("../assets/sprites/Villageois.png") ||
            !texPikeman.loadFromFile("../assets/sprites/Piquier.png") ||
            !texHero.loadFromFile("../assets/sprites/Heros.png") ||
            !texKnight.loadFromFile("../assets/sprites/Chevalier.png") ||
            !texTower.loadFromFile("../assets/sprites/Tour.png") ||
            !texBandit.loadFromFile("../assets/sprites/Bandit.png") ||
            !texVillage.loadFromFile("../assets/sprites/Village.png") ||
            !texBanditCampement.loadFromFile("../assets/sprites/BanditCampement.png"))
        {
            throw std::runtime_error("Impossible de charger une texture d'entité");
        }
        
        texHexOwners[1]  = &texHexJoueur1;
        texHexOwners[2]  = &texHexJoueur2;
        texHexOwners[3]  = &texHexJoueur3;
        texHexOwners[4]  = &texHexJoueur4;
        texHexOwners[5]  = &texHexJoueur5;
        texHexOwners[6]  = &texHexJoueur6;
        texHexOwners[7]  = &texHexJoueur7;
        texHexOwners[8]  = &texHexJoueur8;
        texHexOwners[9]  = &texHexJoueur9;
        texHexOwners[10]  = &texHexJoueur10;
        loaded = true;
    }

    const float tileW = static_cast<float>(texHexGrass.getSize().x);
    const float tileH = static_cast<float>(texHexGrass.getSize().y);

    constexpr float frequency = 1.f;
    const float amplitude = tileH / 6.f; 

    sf::Sprite sprite;

    int lignes = tileMap.getLignes();
    int colonnes = tileMap.getColonnes();

    for (int ligne = 0; ligne < lignes; ++ligne) {
        for (int colonne = 0; colonne < colonnes; ++colonne) {
            const Tuile& t = tileMap.get(ligne, colonne);

            if (t.possesseurId == BANDIT_ID) {
                sprite.setTexture(texHexBandit);
            }
            else if (t.possesseurId >= 0 && t.possesseurId < 11 && texHexOwners[t.possesseurId]) {
                sprite.setTexture(*texHexOwners[t.possesseurId]);
            }
            else {
                switch (t.terrain) {
                    case TerrainType::None:
                        sprite.setTexture(texHexGrass);
                        break;
                    case TerrainType::Forest:
                        sprite.setTexture(texHexForest);
                        break;
                    default:
                        sprite.setTexture(texHexGrass);
                        break;
                }
            }

            float px = colonne * tileW + (ligne % 2) * (tileW / 2.f);
            float py = ligne * (tileH * 0.75f);
            sprite.setPosition(px, py);

            if (t.possesseurId > 0 && t.possesseurId != BANDIT_ID) {
                bool connexe = tuileEstConnecteeABase(ligne, colonne, t.possesseurId);
                if (connexe) {
                    sprite.setColor(sf::Color::White);
                } else {
                    sprite.setColor(sf::Color(150, 150, 150, 255));
                }
            } else {
                sprite.setColor(sf::Color::White);
            }

            if (t.terrain != TerrainType::Water) {
                win.draw(sprite);
            }

            if (auto occ = t.occupant.lock()) {
                float cx = px + tileW / 2.f;
                float cy = py + tileH / 2.f;

                float jump = 0.f;
                if (auto u = std::dynamic_pointer_cast<Unite>(occ)) {
                    if (!std::dynamic_pointer_cast<Tour>(occ) && u->getPossesseur() == joueurActif && !u->aDejaFaitAction()) {
                        float anim = std::sin(_tempsAnim * 2.f * 3.14159265f * frequency);
                        jump = std::max(0.f, anim) * amplitude * 0.5f; 
                    }
                }

                if (auto hb = std::dynamic_pointer_cast<Base>(occ)) {
                    sf::Sprite hbSpr(texVillage);
                    auto b = hbSpr.getLocalBounds();
                    hbSpr.setOrigin(b.width/2.f, b.height/2.f);
                    hbSpr.setPosition(cx, cy - jump);
                    win.draw(hbSpr);
                }
                else if (auto v = std::dynamic_pointer_cast<Villageois>(occ)) {
                    sf::Sprite vSpr(texVillageois);
                    auto b = vSpr.getLocalBounds();
                    vSpr.setOrigin(b.width/2.f, b.height/2.f);
                    vSpr.setPosition(cx, cy - jump);
                    win.draw(vSpr);
                }
                else if (auto p = std::dynamic_pointer_cast<Piquier>(occ)) {
                    sf::Sprite pSpr(texPikeman);
                    auto b = pSpr.getLocalBounds();
                    pSpr.setOrigin(b.width/2.f, b.height/2.f);
                    pSpr.setPosition(cx, cy - jump);
                    win.draw(pSpr);
                }
                else if (auto k = std::dynamic_pointer_cast<Chevalier>(occ)) {
                    sf::Sprite kSpr(texKnight);
                    auto b = kSpr.getLocalBounds();
                    kSpr.setOrigin(b.width/2.f, b.height/2.f);
                    kSpr.setPosition(cx, cy - jump);
                    win.draw(kSpr);
                }
                else if (auto h = std::dynamic_pointer_cast<Heros>(occ)) {
                    sf::Sprite hSpr(texHero);
                    auto b = hSpr.getLocalBounds();
                    hSpr.setOrigin(b.width/2.f, b.height/2.f);
                    hSpr.setPosition(cx, cy - jump);
                    win.draw(hSpr);
                }
                else if (auto tw = std::dynamic_pointer_cast<Tour>(occ)) {
                    sf::Sprite tSpr(texTower);
                    auto b = tSpr.getLocalBounds();
                    tSpr.setOrigin(b.width/2.f, b.height/2.f);
                    tSpr.setPosition(cx, cy - jump);
                    win.draw(tSpr);
                }
                else if (auto tw = std::dynamic_pointer_cast<Bandit>(occ)) {
                    sf::Sprite tSpr(texBandit);
                    auto b = tSpr.getLocalBounds();
                    tSpr.setOrigin(b.width/2.f, b.height/2.f);
                    tSpr.setPosition(cx, cy - jump);
                    win.draw(tSpr);
                }else if (auto tw = std::dynamic_pointer_cast<BanditCampement>(occ)) {
                    sf::Sprite tSpr(texBanditCampement);
                    auto b = tSpr.getLocalBounds();
                    tSpr.setOrigin(b.width/2.f, b.height/2.f);
                    tSpr.setPosition(cx, cy - jump);
                    win.draw(tSpr);
                }
            }

            if (t.hasBarriere) {
                sprite.setTexture(texHexBarrier);
                sprite.setPosition(px, py);
                win.draw(sprite);
            }
        }
    }
}

/**
 * @brief Appel au tour suivant.
 *
 * 1. Vérifie si un joueur a remporté la partie
 * 2. Fait se déplacer les bandits dans la map
 * 3. Donne la main au joueur suivant
 * 4. Rénumère le joueur actif selon les tuiles possédées
 * 5. Mise à jour des barrières.
 */
void Match::tourSuivant() {
    _tempsAnim = 0.f;
    if (partieTerminee) return;

    int actifs = 0;
    std::shared_ptr<Joueur> dernier;
    for (auto& j : joueurs) {
        if (joueurAUnites(j)) {
            ++actifs;
            dernier = j;
        }
    }
    if (actifs <= 1) {
        partieTerminee = true;
        if (dernier) 
            std::cout << dernier->getNom() << " a gagné!\n";
        return;
    }

    auto it = std::find(joueurs.begin(), joueurs.end(), joueurActif);
    if (it == joueurs.end()) it = joueurs.begin();


    do {
        if (++it == joueurs.end()) {
            phaseBandits();
            banditCreer();
            std::cout << "Phase Bandit!\n";
            ++tour;
            it = joueurs.begin();
        }
    } while (!joueurAUnites(*it));

    joueurActif = *it;
    std::cout << "\n=== TOUR " << tour
              << " : " << joueurActif->getNom() << " ===\n";

    piecesTuile(joueurActif);
    paimentSalaire(joueurActif);
    resetActionJoueur(joueurActif);
    updateBarrieres();
}






/**
 * @brief Gère la mutinerie.
 * @param joueur Pointeur (avec compteur) vers un Joueur. On vérifiera par la suite s'il a bien payé ses Unite.
 */
void Match::paimentSalaire(std::shared_ptr<Joueur> joueur) {
    int total = 0;

    for (auto& e : entites) {
        auto u = std::dynamic_pointer_cast<Unite>(e);
        if (u && u->getPossesseur() == joueur) {
            int salaire = u->getSalaire();
            if (joueur->getPieces() >= salaire) {
                joueur->setPieces(joueur->getPieces() - salaire);
                total += salaire;
            } else {
                std::cout << "Unité pas payée en ("<< u->getLigne() << "," << u->getColonne() << ")\n";
                auto ligne = u->getLigne(), colonne = u->getColonne();
                supprimerEntite(u);
                auto bandit = std::make_shared<Bandit>(ligne, colonne);
                auto& tuile = tileMap.get(ligne, colonne);
                tuile.occupant = bandit;
                if (!tuile.hasBarriere) {
                    tuile.possesseurId = BANDIT_ID;
                }
                ajouterEntite(bandit);

                if (verifierVictoire(joueur)) {
                    partieTerminee = true;
                    return;
                }
            }
        }
    }

    if (total > 0) {
        std::cout << "Joueur " << joueur->getNom()
                  << " paye " << total << " en salaires\n";
    }
}

/**
 * @brief Renvoie si la partie est terminée ou non.
 * @returns Un booléen (partieTerminne).
 */
bool Match::estTerminee() const {
    return partieTerminee; 
}

/**
 * @brief Termine la partie.
 */
void Match::terminerPartie() { 
    partieTerminee = true; 
}

/**
 * @brief Permet de sélectionner une unité (Unite) à un endroit précis.
 * @param ligne La ligne indiquée.
 * @param colonne La colonne indiquée.
 */
void Match::selectionnerUnite(int ligne, int colonne) {
    // 1) bornes
    if (ligne < 0 || ligne >= tileMap.getLignes() ||
        colonne < 0 || colonne >= tileMap.getColonnes())
    {
        viderSelection();
        return;
    }

    // 2) récupère l’occupant
    auto occ = tileMap.get(ligne, colonne).occupant.lock();
    auto u   = std::dynamic_pointer_cast<Unite>(occ);

    // 3) teste : est-ce une unité du joueur actif et libre d’action ?
    if (u && u->getPossesseur() == joueurActif && !u->aDejaFaitAction()) {
        setUnitSelectionnee(u);
        std::cout << "Sélection de l’unité " << u->getType()
                  << " en (" << ligne << "," << colonne << ")\n";
    } else {
        viderSelection();
    }
}


/**
 * @brief Initialise les joueurs de la partie.
 * @param players Vecteur de pointeurs (avec refs) de joueurs passé en R-value référence.
 */
void Match::setJoueurs(std::vector<std::shared_ptr<Joueur>>&& players) {
    joueurs = std::move(players);
    if (!joueurs.empty()) {
        joueurActif = joueurs[0];
    }
}

/**
 * @brief Renvoie les joueurs de la partie.
 * @returns Une référence vers un vecteur constant de pointeurs (avec refs) de joueurs. 
 */
const std::vector<std::shared_ptr<Joueur>>& Match::getJoueurs() const {
    return joueurs;
}

/**
 * @brief Renvoie le joueur actif.
 * @returns Un pointeur avec compteur de réfs vers un Joueur.
 */
std::shared_ptr<Joueur> Match::getJoueurActif() const {
    return joueurActif;
}

/**
 * @brief Met le Joueur voulu en joueur actif (celui qui joue au tour courant).
 * @param joueur Pointeur avec compteur de réfs vers un Joueur.
 */
void Match::setJoueurActif(std::shared_ptr<Joueur> joueur) {
    joueurActif = joueur;
}

/**
 * @brief Ajoute une entitée voulue à la partie.
 * @param entity Pointeur avec compteur de réfs vers une Entite.
 */
void Match::ajouterEntite(std::shared_ptr<Entite> entity) {
    entites.push_back(entity);
    tileMap.get(entity->getLigne(), entity->getColonne()).occupant = entity;
}

/**
 * @brief Enlève une entitée de la partie.
 * @param e Référence vers un pointeur (avec compteur de réfs) vers une Entite constante (?).
 */
void Match::supprimerEntite(const std::shared_ptr<Entite>& e) {
    Tuile& t = tileMap.get(e->getLigne(), e->getColonne());

    if (!t.occupant.expired() && t.occupant.lock() == e) {
        t.occupant.reset();
    }

    entites.erase(
        std::remove(entites.begin(), entites.end(), e),
        entites.end()
    );
}

/**
 * @brief Renvoie toutes les entitées de la partie.
 * @returns Une référence vers un vecteur constant de pointeurs vers des Entite. Les pointeurs sont partagés (avec compteurs de références).
 */
const std::vector<std::shared_ptr<Entite>>& Match::getEntites() const {
    return entites;
}

/**
 * @brief Retourne la map.
 * @returns Une référence vers une TileMap.
 */
TileMap& Match::getTileMap() {
    return tileMap;
}

/**
 * @brief Charge une map depuis un fichier.
 * @param nomFichier Nom du fichier. Réf vers un string const. 
 */
bool Match::chargerCustomMap(const std::string& nomFichier) {
    
    std::vector<std::shared_ptr<Entite>> nouvellesEntites;
    if (tileMap.chargerMapASCII(nomFichier, joueurs, nouvellesEntites) == true) {
        for (auto& e : nouvellesEntites) {
            ajouterEntite(e);
        }
        return true;
    }else{
        return false;
    }
    
} 

/**
 * @brief Sélectionne une unité.
 * @param unit Réf vers un pointeur (avec compteur) vers une Unite constante.
 */
void Match::setUnitSelectionnee(const std::shared_ptr<Unite>& unit) {
    unitSelectionnee = unit;
}

/**
 * @brief Renvoie l'entité sélectionnée.
 * @returns Un pointeur (avec compteur) vers une Unite.
 */
std::shared_ptr<Unite> Match::getUniteSelectionnee() const {
    return unitSelectionnee;
}

/**
 * @brief Remise à zéro de la variable définissant l'entitée sélectionnée.
 */
void Match::viderSelection() {
    unitSelectionnee = nullptr;
}

/**
 * @brief Y'a-t-il sélection d'unité ?
 * @returns true si le pointeur vers l'unité sélectionnée est non-nulle, false sinon.
 */
bool Match::selectionEnCours() const {
    return unitSelectionnee != nullptr;
}

/**
 * @brief Prépare un achat d'unité.
 * @param type Le nom de l'unité que l'on voudrait acheter. Réf vers un string const.
 * @param prix Le prix de l'achat en cours.
 */
void Match::preparerAchat(const std::string& type, int prix) {
    _typeAchat = type;
    _prixAchat = prix;
    std::cout << "Achat prêt : " << type
              << " (coût " << prix << ")\n";
}

/**
 * @brief Y'a-t-il des achats en cours ?
 * @returns true si il y en a, false sinon. Vérifie que _typeAchat n'est pas vide. 
 */
bool Match::achatEnCours() const {
    return !_typeAchat.empty();
}

/**
 * @brief Effectue un achat d'une unité dans un endroit donné.
 * @param ligne La ligne où se situera la nouvelle unité.
 * @param colonne La colonne où se situera la nouvelle unité.
 * @returns true en cas de succès, false sinon.
 */
bool Match::placerAchat(int ligne, int colonne) {
    if (!achatEnCours()) return false;

    Tuile& tuile = tileMap.get(ligne, colonne);
    // conditions valides : case libre, terrain plat, appartient au joueur
    if (!tuile.occupant.expired() ||
        tuile.terrain != TerrainType::None ||
        tuile.possesseurId != joueurActif->getId())
    {
        std::cout << "Placement invalide en ("
                  << ligne << "," << colonne << ")\n";
        return false;
    }
    if (joueurActif->getPieces() < _prixAchat) {
        std::cout << "Or insuffisant\n";
        return false;
    }

    // création de l’unité correspondante
    std::shared_ptr<Entite> ent;
    if (_typeAchat == "Villageois") {
        ent = std::make_shared<Villageois>(ligne, colonne, joueurActif);
    }
    else if (_typeAchat == "Piquier") {
        ent = std::make_shared<Piquier>(ligne, colonne, joueurActif);
    }
    else if (_typeAchat == "Chevalier") {
        ent = std::make_shared<Chevalier>(ligne, colonne, joueurActif);
    }
    else if (_typeAchat == "Heros") {
        ent = std::make_shared<Heros>(ligne, colonne, joueurActif);
    }
    else if (_typeAchat == "Tour") {
        ent = std::make_shared<Tour>(ligne, colonne, joueurActif);
    }

    // payer et ajouter
    joueurActif->setPieces(joueurActif->getPieces() - _prixAchat);
    tuile.occupant = ent;
    ajouterEntite(ent);

    std::cout << _typeAchat << " placé en ("
              << ligne << "," << colonne << ")\n";
    return true;
}

/**
 * @brief Remise à zéro des achats en cours.
 */
void Match::annulerAchatEnCours() {
    _typeAchat.clear();
    _prixAchat = 0;
}



/**
 * @brief Remise à zéro des actions du joueur.
 * @param joueur Référence vers un pointeur (avec compteur de réfs) vers un Joueur constant. 
 */
void Match::resetActionJoueur(const std::shared_ptr<Joueur>& joueur) {
    for (const auto& e : entites) {
        auto u = std::dynamic_pointer_cast<Unite>(e);
        if (u && u->getPossesseur() == joueur) {
            u->reinitialiserTour();
        }
    }
}



/**
 * @brief Mise à jour des barrières de la map.
 */
void Match::updateBarrieres() {
    for (int ligne = 0; ligne < tileMap.getLignes(); ++ligne)
        for (int colonne = 0; colonne < tileMap.getColonnes(); ++colonne)
            tileMap.get(ligne, colonne).hasBarriere = false;

    for (auto& e : entites) {
        auto tower = std::dynamic_pointer_cast<Tour>(e);
        auto home  = std::dynamic_pointer_cast<Base>(e);
        if (!tower && !home) continue;

        int possesseurId = (tower ? tower->getPossesseur()->getId()
                             : home->getPossesseur().lock()->getId());

        auto& center = tileMap.get(e->getLigne(), e->getColonne());
        if (center.possesseurId == possesseurId)
            center.hasBarriere = true;

        for (auto [voisinLigne,voisinColonne] : tileMap.getVoisins(e->getLigne(), e->getColonne())) {
            auto& t = tileMap.get(voisinLigne, voisinColonne);
            if (t.terrain == TerrainType::None
             && t.possesseurId == possesseurId)
            {
                t.hasBarriere = true;
            }
        }
    }
}


/**
 * @brief Savoir si une tuile est connectée a une base du joueur.
 */
bool Match::tuileEstConnecteeABase(int ligne, int colonne, int playerId) const {
    const Tuile& t = tileMap.get(ligne, colonne);
    if (t.possesseurId != playerId) return false;

    for (auto& e : entites) {
        auto hb = std::dynamic_pointer_cast<Base>(e);
        if (!hb) continue;
        auto possesseur = hb->getPossesseur().lock();
        if (!possesseur || possesseur->getId() != playerId) continue;

        if (tileMap.sontConnectees(ligne, colonne,
                                  hb->getLigne(),
                                  hb->getColonne(),
                                  playerId))
        {
            return true;
        }
    }
    return false;
}


/**
 * @brief Calcul des gains apres chaque tour en tenant compte des tuiles non connectés.
 * @param joueur Référence vers un pointeur (avec compteur de réfs) vers un Joueur constant. 
 */
void Match::piecesTuile(const std::shared_ptr<Joueur>& joueur) {
    const int tileValue = 1;
    int gain = 0;

    for (int ligne = 0; ligne < tileMap.getLignes(); ++ligne) {
        for (int colonne = 0; colonne < tileMap.getColonnes(); ++colonne) {
            if (tuileEstConnecteeABase(ligne, colonne, joueur->getId())) {
                gain += tileValue;
            }
        }
    }

    joueur->ajouterPieces(gain);
    if (gain > 0) {
        std::cout << "Joueur " << joueur->getNom()
                  << " gagne +" << gain
                  << " or de tuiles connectées\n";
    }
}



/**
 * @brief Transformation des unités en bandits si les conditions sont favorables.
 */
void Match::updateBandit() {

    std::vector<std::shared_ptr<Unite>> toConvert;
    for (auto& e : entites) {
        auto u = std::dynamic_pointer_cast<Unite>(e);
        if (!u) continue;

        auto possesseur = u->getPossesseur();
        if (!possesseur) continue;
        int playerId = possesseur->getId();

        int ligne = u->getLigne(), colonne = u->getColonne();
        if (!tuileEstConnecteeABase(ligne, colonne, playerId)) {
            toConvert.push_back(u);
        }
    }

    for (auto& u : toConvert) {
        auto oldOwner = u->getPossesseur();
        int ligne = u->getLigne(), colonne = u->getColonne();
        supprimerEntite(u);

        auto bandit = std::make_shared<Bandit>(ligne, colonne);
        auto& tuile = tileMap.get(ligne, colonne);
        tuile.occupant = bandit;
        if (!tuile.hasBarriere) {
            tuile.possesseurId = BANDIT_ID;
        }
        ajouterEntite(bandit);

        std::cout << "Unité isolée en ("<<ligne<<","<<colonne<<") devient bandit\n";

        if (verifierVictoire(oldOwner)) {
            partieTerminee = true;
            return;
        }
    }

    
}


/**
 * @brief Vérifie si un joueur a gagné.
 * @param joueur Référence vers un pointeur (avec compteur de réfs) vers un Joueur constant. 
 */
bool Match::verifierVictoire(const std::shared_ptr<Joueur>& joueur) {
    for (const auto& e : entites) {
        auto unit = std::dynamic_pointer_cast<Unite>(e);
        if (!unit) continue;

        auto possesseur = unit->getPossesseur();

        if (possesseur && possesseur != joueur) {
            return false;
        }
    }
    std::cout << joueur->getNom() << " a gagné!\n";
    return true;
}

/**
 * @brief Si le joueur possède des entités.
 * @returns true si c'est vrai, false sinon.
 */
bool Match::joueurAUnites(const std::shared_ptr<Joueur>& joueur) const {
    for (auto& e : entites) {
        auto u = std::dynamic_pointer_cast<Unite>(e);
        if (u && u->getPossesseur() == joueur) {
            return true;
        }
    }
    return false;
}



/**
 * @brief Effectue les déplacements des bandits.
 */
void Match::phaseBandits() {
    for (auto& e : entites) {
      if (auto b = std::dynamic_pointer_cast<Bandit>(e)) {
        b->deplacement(tileMap);
      }
    }
}

/**
 * @brief Transforme des royaumes autrefois paisibles en répugnants campements pour bandits (beurk). 
 */
void Match::detecterCampements() {
    std::vector<std::shared_ptr<Tour>> aConvertir;
    for (auto& e : entites) {
        if (auto tw = std::dynamic_pointer_cast<Tour>(e)) {
            if (!tuileEstConnecteeABase(tw->getLigne(), tw->getColonne(),
                                       tw->getPossesseur()->getId()))
            {
                aConvertir.push_back(tw);
            }
        }
    }
    for (auto& tw : aConvertir) {
        int L = tw->getLigne(), C = tw->getColonne();
        supprimerEntite(tw);
        auto camp = std::make_shared<BanditCampement>(L, C);
        tileMap.get(L, C).occupant    = camp;
        tileMap.get(L, C).possesseurId = BANDIT_ID;
        ajouterEntite(camp);
        campsBandit.push_back(camp);
    }
}

/**
 * @brief Gestion de création de bandits dans les campements.
 */
void Match::banditCreer() {
    int revenuTotal = 0;
    int L = tileMap.getLignes(), C = tileMap.getColonnes();
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < C; ++j) {
            if (tileMap.get(i, j).possesseurId == BANDIT_ID) {
                ++revenuTotal;
            }
        }
    }
    if (revenuTotal <= 0 || campsBandit.empty()) return;

    for (auto& camp : campsBandit) {
        camp->ajouterButin(revenuTotal);
    }

    for (auto& camp : campsBandit) {
        if (camp->getStock() < 3) continue;

        camp->ajouterButin(-3);

        int rl = camp->getLigne();
        int rc = camp->getColonne();
        bool spawned = false;

        for (auto [nl, nc] : tileMap.getVoisins(rl, rc)) {
            auto& t = tileMap.get(nl, nc);
            if (t.terrain == TerrainType::None && t.occupant.expired()) {
                auto b = std::make_shared<Bandit>(nl, nc);
                ajouterEntite(b);
                t.occupant     = b;
                t.possesseurId = BANDIT_ID;
                spawned = true;
                break;
            }
        }
    }
}