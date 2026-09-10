#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>
#include <filesystem>
#include <string>
#include "match.h"
#include "jeu.h"
#include "joueur.h"

namespace fs = std::filesystem;
using Players = std::vector<std::shared_ptr<Joueur>>;
using PlayersPtr = std::shared_ptr<Players>;

/**
 * @class Renderer
 * @brief Classe du gestionnaire de rendu. C'est lui qui gère l'affichage fenêtré et les évènements clavier/souris.
 */
class Renderer final{
public:
    /**
     * @brief Constructeur par défaut de renderer.
     * @param largeur Largeur de la fenêtre.
     * @param hauteur Hauteur de la fenêtre.
     * @param titre Titre de la fenêtre.
     */
    Renderer(unsigned int largeur, unsigned int hauteur, const std::string& titre, Jeu& jeu, PlayersPtr joueurs);
    ~Renderer() = default;

    void ajouterBouton(const std::string& textureFile,
                   sf::Vector2f position,
                   std::function<void()> onClick);
    void ajouterImage(const std::string& textureFile,
                  sf::Vector2f position);

    void ajouterTexte(const std::string& str, sf::Vector2f pos);
    void effacerBoutons();
    void effacerImages();
    void effacerTexte();

    void initMenuPrincipal();
    void initMapSelector();

    void jouerMatch(std::shared_ptr<Match> m);

    void run();

private:
    /**
     * @enum Scene
     * @brief Enum définissant les différentes scènes à l'écran.
     */
    enum class Scene { Menu, MapSelector, Jeu };
    Scene _scene = Scene::Menu;
    std::shared_ptr<Match> _match = nullptr;
    bool _finPartieUIInitialisee = false;

    /**
     * @struct Bouton
     * @brief Structure définissant un boutton cliquable ainsi que son action associée.
     */
    struct Bouton {
        sf::Sprite sprite;                                  ///< Sprite du bouton.
        std::function<void()> onClick;                      ///< Fonction à appeler lors du clic.
        bool survole = false;                               ///< Booléen indiquant si le bouton est survolé.
        int prix = 0;                                       ///< Prix de l'unité.

        /**
         * @brief Constructeur par défaut du bouton.
         */
        Bouton(const sf::Texture& tex,
               sf::Vector2f pos,
               std::function<void()> cb)
          : sprite(tex)
          , onClick(std::move(cb))
        {
            sprite.setPosition(pos);
        }
        
        /**
         * @struct BoutonUnite
         * @brief Bouton avec pour icône une Unite.
         */
        struct BoutonUnite {
            sf::Sprite    icone;                            ///< Icône du bouton.
            sf::Text      textePrix;                        ///< Texte indiquant le prix.
            std::function<void()> onClick;                  ///< Fonction à appeler lors du clic.
        };
        std::vector<BoutonUnite> _unitBoutons;              ///< Vecteur de boutons d'achat d'unités.
        
        std::string _typeAchatBouton;                       ///< Type de l'unité achetée.
        int         _prixAchatBouton = 0;                   ///< Prix de l'unité.
    };

    sf::RenderWindow _fenetre;                              ///< Fenêtre de rendu.
    std::vector<std::unique_ptr<sf::Texture>> _textures;    ///< Vecteur de textures à afficher.
    std::vector<Bouton> _boutons;                           ///< Vecteur de boutons à afficher.
    std::vector<sf::Sprite> _sprites;                       ///< Vecteur de sprites.

    void gestionEvenements();
    void update(sf::Time dt);
    void render();

    std::vector<sf::Text> _textes;                          ///< Vecteur de textes à afficher.
    sf::Font _police;                                       ///< Police de caractère globale.

    Jeu& _jeu;                                              ///< Pointeur vers le Jeu.
    PlayersPtr _joueurs;                                    ///< Pointeur vers les joueurs.

    sf::View _vue;                                          ///< Vue par défaut.
    float _niveauZoom = 1.f;                                ///< Niveau de zoom par défaut.

    bool _panning   = false;                                ///< S'il y a action de "panning".
    sf::Vector2i _dragAnchor;                               ///< Le début du glisser/déposer.

    bool               _glisserDeposer    = false;          ///< S'il y a action de glisser/déposer.
    std::pair<int,int> _glisserDebut   = {-1,-1};           ///< Les coordonnées du début du glisser/déposer.
    std::pair<int,int> _glisserCourant = {-1,-1};           ///< Les coordonnées du glisser/déposer à l'instant t.

    bool _glisserAchat = false;                             ///< Si le glisser/déposer est un achat.
    sf::Sprite _glisserFantome;                             ///< "Fantôme" de l'entitée glissée.
};
