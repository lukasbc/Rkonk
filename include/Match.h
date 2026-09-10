#pragma once

#include <vector>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "tilemap.h"
#include "joueur.h"
#include "entite.h"
#include "unite.h"
#include "banditcampement.h"

/**
 * @class Match
 * @brief Classe d'une partie (ou d'un Match).
 */
class Match {
private:
    int id;                                                     ///< L'identifiant du Match.
    bool partieTerminee = false;                                ///< Si le Match (ou la partie) est terminé.
    std::vector<std::shared_ptr<Joueur>> joueurs;               ///< Vecteur contenant tous les joueurs de la partie.
    std::shared_ptr<Joueur> joueurActif;                        ///< Le joueur qui joue pendant le tour courant.
    TileMap tileMap;                                            ///< La carte sur laquelle se joue la partie.
    std::vector<std::shared_ptr<Entite>> entites;               ///< Vecteur contenant toutes les entités présentes sur la carte.
    std::shared_ptr<Unite> unitSelectionnee;                    ///< Pointeur vers l'entité sélectionnée par le joueur.
    float _tempsAnim = 0.f;                                     ///< Delta temporel t permettant de faire des animations.

    std::string achatSouhaite;                                  ///< Nom de l'unité que le joueur souhaite acheter. (input)
    int prixUnite = 0;                                          ///< Prix de l'unité.
    int tour = 0;                                               ///< Numéro du tour courant.

    std::string _typeAchat;                                     ///< Nom de l'unité en cours d'achat. Teste l'égalité avec les noms des différentes unités.
    int _prixAchat = 0;                                         ///< Coût de l'achat en cours.

    std::vector<std::shared_ptr<BanditCampement>> campsBandit;  ///< Vecteur de campements de bandits.
    void detecterCampements();                                  ///< Détecte les campements de bandits.
    void banditCreer();                                         ///< Crée un bandit.
public:
    explicit Match(int id_);                                    ///< Constructeur par défaut du Match.
    int getId() const;                                          ///< Retourne l'identifiant du Match.
    
    void demarrerPartie();                                      ///< Démarre le Match.
    void boucleDeJeu();                                         ///< Boucle de jeu de la partie ou Match.
    void tourSuivant();                                         ///< Passe au tour suivant.
    void afficherCarteDebug() const;                            ///< Affiche la carte (debug).
    void paimentSalaire(std::shared_ptr<Joueur> joueur);        ///< Fait payer au Joueur le salaire de ses unités.
    void recommencer();                                         ///< Recommence un Match.
    

    void gestionInput(const sf::Event& evt);                    ///< Gère les entrées clavier/souris.
    void update(sf::Time dt);                                   ///< Mise à jour de l'affichage.
    void render(sf::RenderWindow& window);                      ///< Rendu de l'affichage.

    void phaseActionConsole(const std::shared_ptr<Joueur>& joueur); ///< Gère l'entrée clavier du Joueur (debug).
    void resetActionJoueur(const std::shared_ptr<Joueur>& joueur);  ///< Mise à zéro des actions du Joueur.

    bool estTerminee() const;                                       ///< Fin d'une partie ou Match.
    void terminerPartie();                                          ///< Termine le Match.

    void selectionnerUnite(int ligne, int colonne);                 ///< Sélectionne une unité.
    

    void setJoueurs(std::vector<std::shared_ptr<Joueur>>&& players);    ///< Initialise les joueurs de la partie.
    const std::vector<std::shared_ptr<Joueur>>& getJoueurs() const;     ///< Retourne les joueurs de la partie.

    std::shared_ptr<Joueur> getJoueurActif() const;                     ///< Retourne le Joueur qui joue son tour.
    void setJoueurActif(std::shared_ptr<Joueur> joueur);                ///< Désigne un Joueur qui doit jouer son tour.

    void ajouterEntite(std::shared_ptr<Entite> entity);                 ///< Ajoute une Entite à la partie.
    void supprimerEntite(const std::shared_ptr<Entite>& entity);        ///< Supprime une Entite à la partie.
    const std::vector<std::shared_ptr<Entite>>& getEntites() const;     ///< Renvoie les entités de la partie.

    TileMap& getTileMap();                                              ///< Renvoie la map de tuiles.

    bool chargerCustomMap(const std::string& nomFichier);               ///< Charge une map de tuiles à partir d'un fichier.

    void setUnitSelectionnee(const std::shared_ptr<Unite>& unit);       ///< Désigne une Unite comme étant sélectionnée par le Joueur actif.
    std::shared_ptr<Unite> getUniteSelectionnee() const;                ///< Renvoie l'unité sélectionnée par le Joueur actif.
    void viderSelection();                                              ///< Vide la sélection du Joueur actif.
    bool selectionEnCours() const;                                      ///< Renvoie vrai s'il y a sélection en cours.
    

    void preparerAchat(const std::string& type, int prix);              ///< Prépare la phase d'achat.
    bool achatEnCours() const;                                          ///< Renvoie vrai s'il y a achat en cours.
    bool placerAchat(int ligne, int colonne);                           ///< Place l'entité achetée.
    void annulerAchatEnCours();                                         ///< Annule l'achat en cours.

    void updateBarrieres();                                             ///< Mise à jour des barrières des joueurs.
    bool tuileEstConnecteeABase(int ligne, int colonne, int playerId) const;    ///< Renvoie vrai si la Tuile est connectée à la Base.
    void piecesTuile(const std::shared_ptr<Joueur>& joueur);                    ///< Rénumère le Joueur les tuiles possédées.
    void updateBandit();                                                        ///< Mise à jour des entités en bandits si la condition pour est vraie.


    bool verifierVictoire(const std::shared_ptr<Joueur>& joueur);               ///< Vérifie s'il y a vaincqueur.
    bool joueurAUnites(const std::shared_ptr<Joueur>& joueur) const;            ///< Vérifie si le Joueur a des entités.
    void phaseBandits();                                                        ///< Phase de déplacement des bandits.
};