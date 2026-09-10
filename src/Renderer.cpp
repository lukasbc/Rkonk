#include "../include/renderer.h"
#include "../include/unite.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>

/**
 * @brief Convertit des coordonnées x,y en coordonnées hexagonales.
 * @param Coordonnée x.
 * @param Coordonnée y.
 * @returns Une paire de int.
 */
static std::pair<int,int> worldToHex(float x, float y,
    float tileW, float tileH)
{
    int ligne = static_cast<int>(y / (tileH * 0.75f));
    float xOffset = (ligne % 2) * (tileW / 2.f);
    int colonne = static_cast<int>((x - xOffset) / tileW);
    return { ligne, colonne };
}

/**
 * @brief Constructeur par défaut de la classe Renderer.
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param titre Titre de la fenêtre.
 */
Renderer::Renderer(unsigned int largeur, unsigned int hauteur, const std::string& titre,Jeu& jeu, PlayersPtr joueurs)
: _fenetre(sf::VideoMode(largeur, hauteur), titre), _vue(_fenetre.getDefaultView()),_jeu(jeu), _joueurs(joueurs) , _scene(Scene::Menu), _finPartieUIInitialisee(false)
{
    _fenetre.setFramerateLimit(60);
    _vue = _fenetre.getDefaultView();

    if (!_police.loadFromFile("../assets/fonts/arial.ttf"))
        throw std::runtime_error("Impossible de charger assets/fonts/arial.ttf");
    
    sf::Image icone;
    if (!icone.loadFromFile("../assets/sprites/icon.png"))
        throw std::runtime_error("Impossible de charger ../assets/sprites/icon.png");
    
    _fenetre.setIcon(icone.getSize().x, icone.getSize().y, icone.getPixelsPtr());

    initMenuPrincipal();
}

/**
 * @brief Ajoute un bouton à la fenêtre.
 * @param textureFile Chemin vers le fichier texture.
 * @param position Coordonnées x et y.
 * @param onClick Fonction à appeler lorsque le bouton est cliqué.
 */
void Renderer::ajouterBouton(const std::string& textureFile,
    sf::Vector2f position,
    std::function<void()> onClick)
{
    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(textureFile))
    throw std::runtime_error("Impossible de charger " + textureFile);

    const sf::Texture& ref = *tex;
    _textures.push_back(std::move(tex));
    _boutons.emplace_back(ref, position, std::move(onClick));

    auto& btnSprite = _boutons.back().sprite;
    auto bounds    = btnSprite.getLocalBounds();
    btnSprite.setOrigin(bounds.width  / 2.f,
    bounds.height / 2.f);
}

/**
 * @brief Ajoute du texte à la fenêtre.
 * @param str Chaîne de caractères contenant le texte à afficher.
 * @param pos Coordonnées x et y.
 */
void Renderer::ajouterTexte(const std::string& str, sf::Vector2f pos) {
    sf::Text t;
    t.setFont(_police);
    t.setString(str);
    t.setCharacterSize(24);
    auto bb = t.getLocalBounds();
    t.setOrigin(bb.left + bb.width/2.f,
                bb.top  + bb.height/2.f);
    t.setPosition(pos);
    _textes.push_back(std::move(t));
}

/**
 * @brief Réinitialise les boutons à zéro.
 */
void Renderer::effacerBoutons() { 
    _boutons.clear(); 
}

/**
 * @brief Réinitialise les sprites à zéro.
 */
void Renderer::effacerImages() { 
    _sprites.clear(); 
}

/**
 * @brief Réinitialise le texte à zéro.
 */
void Renderer::effacerTexte() { 
    _textes.clear(); 
}


void Renderer::initMenuPrincipal() {
    effacerBoutons();
    effacerImages();
    effacerTexte();

    ajouterImage("../assets/sprites/Menu_Logo.png", {400.f, 175.f});

    ajouterBouton(
        "../assets/sprites/Button_New.png",
        {400.f, 350.f},
        [this] {
            _scene = Scene::MapSelector;  
            _finPartieUIInitialisee = false;
            initMapSelector();
        }
    );

    ajouterBouton(
        "../assets/sprites/Button_Quit.png",
        {400.f, 450.f},
        [] {
            std::exit(0);
        }
    );
}


void Renderer::initMapSelector() {
    effacerBoutons();
    effacerImages();
    effacerTexte();

    std::vector<std::string> mapPaths;
    for (auto& p : fs::directory_iterator("../maps")) {
        if (p.path().extension() == ".txt")
            mapPaths.push_back(p.path().string());
    }

    ajouterImage("../assets/sprites/MapImg.png",{400.f, 50.f});

    if (mapPaths.empty()) {
        ajouterTexte("Aucune carte trouvée", {400.f, 300.f});
    }
    else {
        sf::Texture btnTex; btnTex.loadFromFile("../assets/sprites/Button_Template.png");
        float btnH    = float(btnTex.getSize().y);
        float spacing = 20.f;
        float totalH  = mapPaths.size() * btnH + (mapPaths.size() - 1) * spacing;
        float startY  = (600.f - totalH) / 2.f + btnH/2.f;
        float centerX = 400.f;

        for (size_t i = 0; i < mapPaths.size(); ++i) {
            float y        = startY + i * (btnH + spacing);
            auto  fullpath = mapPaths[i];
            auto  fname    = std::filesystem::path(fullpath).filename().string();

            ajouterBouton(
                "../assets/sprites/Button_Template.png",
                {centerX, y},
                [this, fullpath] {
                    auto m = _jeu.nouvellePartie();
                    Players local = *_joueurs;
                    m->setJoueurs(std::move(local));

                    if (!m->chargerCustomMap(fullpath)) {
                        std::cerr << "Impossible de charger la carte " << fullpath << "\n";
                        return;
                    }
                    jouerMatch(m);
                    m->demarrerPartie();
                }
            );


            ajouterTexte(fname, {centerX, y});
        }
    }

    ajouterBouton(
        "../assets/sprites/Button_Back.png",
        {150.f, 550.f},
        [this] {
            _scene = Scene::Menu;
            initMenuPrincipal();
        }
    );
}




/**
 * @brief Ajoute une image à la fenêtre.
 * @param file Chemin vers l'image texture.
 * @param pos Coordonnées x et y.
 */
void Renderer::ajouterImage(const std::string& file, sf::Vector2f pos)
{
    auto tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromFile(file))
    throw std::runtime_error("Impossible de charger " + file);
    _textures.push_back(std::move(tex));
    sf::Sprite s{*_textures.back()};

    auto bounds = s.getLocalBounds();
    s.setOrigin(bounds.width  / 2.f,
    bounds.height / 2.f);

    s.setPosition(pos);
    _sprites.push_back(s);
}

/**
 * @brief Joue une Partie donnée en paramètre.
 * @param m Le Match voulu.
 */
void Renderer::jouerMatch(std::shared_ptr<Match> m) {
    _match = std::move(m);
    _scene = Scene::Jeu;

    effacerBoutons();
    effacerImages();
    effacerTexte();

    ajouterImage("../assets/sprites/Shop_Sidebar.png", {763.f, 300.f});

    ajouterBouton(
        "../assets/sprites/Button_NextTurn.png",
        {100.f, 550.f},
        [this]() { 
            if (_match) _match->tourSuivant();
        }
      );

    struct U { const char* tex, *type; int prix; };
    constexpr U units[] = {
      {"../assets/sprites/Villageois.png","Villageois",10},
      {"../assets/sprites/Piquier.png",   "Piquier",   20},
      {"../assets/sprites/Chevalier.png",    "Chevalier",    40},
      {"../assets/sprites/Heros.png",      "Heros",      80},
      {"../assets/sprites/Tour.png",     "Tour",     20}
    };

    float x0 = _fenetre.getSize().x - 35.f;
    float y0 = 120.f, dy = 80.f;
    for (int i = 0; i < 5; ++i) {
        auto& u = units[i];
    
        sf::Vector2f pos;
        if (i == 4) {
            pos = {x0, y0 + i * dy + 20.f};
        } else {
            pos = {x0, y0 + i * dy};
        }
    
        ajouterBouton(u.tex, pos, [this, u]() {
            _match->preparerAchat(u.type, u.prix);
            _glisserAchat = true;
        
            auto ghostTexture = std::make_unique<sf::Texture>();
            if (!ghostTexture->loadFromFile(u.tex)) {
                throw std::runtime_error("Impossible de charger " + std::string(u.tex));
            }
        
            _glisserFantome.setTexture(*ghostTexture);
            _glisserFantome.setTextureRect(sf::IntRect(0, 0, ghostTexture->getSize().x, ghostTexture->getSize().y));
            _glisserFantome.setColor(sf::Color(255,255,255,128));
        
            auto b = _glisserFantome.getLocalBounds();
            _glisserFantome.setOrigin(b.width / 2.f, b.height / 2.f);
        
            _textures.push_back(std::move(ghostTexture));

            auto mousePx = sf::Mouse::getPosition(_fenetre);
            sf::Vector2f worldPos = _fenetre.mapPixelToCoords(mousePx, _fenetre.getDefaultView());
            _glisserFantome.setPosition(worldPos);
        });
    
        _boutons.back().prix = u.prix;
    
        ajouterTexte(std::to_string(u.prix), {pos.x, pos.y + 40.f});
    }
}

/**
 * @brief Démarre la boucle de rendu.
 */
void Renderer::run() {
    sf::Clock clock;
    while (_fenetre.isOpen()) {
        gestionEvenements();
        sf::Time dt = clock.restart();
        update(dt);
        render();
    }
}

/**
 * @brief Gère les évènements de la fenêtre.
 */
void Renderer::gestionEvenements() {
    sf::Event evt;
    constexpr float tileW = 74.f, tileH = 84.f;

    auto toWorld = [&](int x, int y) {
        return _fenetre.mapPixelToCoords({ x, y }, _vue);
    };
    auto toHex = [&](sf::Vector2f w) {
        return worldToHex(w.x, w.y, tileW, tileH);
    };

    while (_fenetre.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            _fenetre.close();
            return;
        }

        if (_glisserAchat && evt.type == sf::Event::MouseMoved) {
            sf::Vector2f p = _fenetre.mapPixelToCoords(
                {evt.mouseMove.x, evt.mouseMove.y},
                _fenetre.getDefaultView()
                );
            _glisserFantome.setPosition(p);
        }

        if (_glisserDeposer && evt.type == sf::Event::MouseMoved) {
            sf::Vector2f world = _fenetre.mapPixelToCoords(
                                  {evt.mouseMove.x, evt.mouseMove.y},
                                  _vue);
            _glisserFantome.setPosition(world);
        }

        if (_glisserAchat
            && evt.type == sf::Event::MouseButtonReleased
            && evt.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f worldPos = _fenetre.mapPixelToCoords(
                { evt.mouseButton.x, evt.mouseButton.y },
                _vue
            );

            constexpr float tileW = 74.f, tileH = 84.f;
            auto [ligne, colonne] = worldToHex(worldPos.x, worldPos.y, tileW, tileH);

            auto& tm = _match->getTileMap();
            if (ligne >= 0 && ligne < tm.getLignes()
             && colonne >= 0 && colonne < tm.getColonnes())
            {
                _match->placerAchat(ligne, colonne);
            }
            else
            {
                _match->annulerAchatEnCours();
            }

            _glisserAchat = false;
        }


        if (evt.type == sf::Event::MouseButtonPressed
         && evt.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mp = _fenetre.mapPixelToCoords(
                { evt.mouseButton.x, evt.mouseButton.y },
                _fenetre.getDefaultView()
            );
            for (auto& btn : _boutons) {
                if (btn.sprite.getGlobalBounds().contains(mp)) {
                    btn.onClick();
                    goto next_event;
                }
            }
        }

        if (_scene == Scene::Jeu && _match && _match->estTerminee()) {
            continue;
        }


        if (_scene == Scene::Jeu) {
            switch (evt.type) {
                case sf::Event::MouseWheelScrolled:
                    if (evt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        if (evt.mouseWheelScroll.delta > 0) {
                            _vue.zoom(0.9f);  _niveauZoom *= 0.9f;
                        } else {
                            _vue.zoom(1.1f);  _niveauZoom *= 1.1f;
                        }
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    if (evt.mouseButton.button == sf::Mouse::Right) {
                        _panning    = true;
                        _dragAnchor = sf::Mouse::getPosition(_fenetre);
                    } else {
                        auto world = toWorld(evt.mouseButton.x, evt.mouseButton.y);
                        auto [ligne, colonne] = toHex(world);
                        _match->selectionnerUnite(ligne, colonne);
                        if (auto unit = _match->getUniteSelectionnee()) {
                            if (unit->getType() != "Tour") {
                                _glisserDeposer    = true;
                                _glisserDebut   = { ligne, colonne };
                                _glisserCourant = { ligne, colonne };
                            
                                std::string path = std::string("../assets/sprites/") 
                                                + unit->getType() + ".png";
                                auto tex = std::make_unique<sf::Texture>();
                                if (!tex->loadFromFile(path))
                                    throw std::runtime_error("Impossible de charger " + path);
                            
                                _glisserFantome.setTexture(*tex);
                                _glisserFantome.setTextureRect(
                                sf::IntRect(0,0, tex->getSize().x, tex->getSize().y));
                                _glisserFantome.setColor({255,255,255,128});
                                auto b = _glisserFantome.getLocalBounds();
                                _glisserFantome.setOrigin(b.width/2.f, b.height/2.f);
                            
                                _textures.push_back(std::move(tex));

                                auto mousePx = sf::Mouse::getPosition(_fenetre);
                                sf::Vector2f worldPos = _fenetre.mapPixelToCoords(mousePx, _vue);
                                _glisserFantome.setPosition(worldPos);
                            }else{
                                _match->viderSelection();
                            }
                        }
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (_panning) {
                        auto cur  = sf::Mouse::getPosition(_fenetre);
                        sf::Vector2f diff(
                            float(cur.x - _dragAnchor.x),
                            float(cur.y - _dragAnchor.y)
                        );
                        _vue.move(-diff.x * _niveauZoom,
                                   -diff.y * _niveauZoom);
                        _dragAnchor = cur;
                    }
                    else if (_glisserDeposer) {
                        auto world = toWorld(evt.mouseMove.x, evt.mouseMove.y);
                        _glisserCourant = toHex(world);
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    if (evt.mouseButton.button == sf::Mouse::Right) {
                        _panning = false;
                    }
                    else if (_glisserDeposer) {
                        auto world = toWorld(evt.mouseButton.x, evt.mouseButton.y);
                        auto [ligne, colonne] = toHex(world);
                    
                        auto& tm = _match->getTileMap();
                        if (ligne < 0 || ligne >= tm.getLignes() ||
                            colonne < 0 || colonne >= tm.getColonnes())
                        {
                            _match->viderSelection();
                            _glisserDeposer = false;
                            goto next_event;
                        }

                        if (auto sel = _match->getUniteSelectionnee()) {
                            Tuile& tileDest = tm.get(ligne, colonne);
                            int playerId = _match->getJoueurActif()->getId();
                    
                            if (tileDest.possesseurId == playerId
                             && !_match->tuileEstConnecteeABase(ligne, colonne, playerId))
                            {
                                std::cout << "case isolée (non connectée à une base)\n";
                            }
                            else {

                                auto ent  = tileDest.occupant.lock();
                                auto targ = std::dynamic_pointer_cast<Unite>(ent);
                                auto camp = std::dynamic_pointer_cast<BanditCampement>(ent);
                            

                                if (targ && targ != sel && sel->peutFusionner(*targ)) {
                                    if (sel->fusionner(*targ, *_match)) {
                                        _match->viderSelection();
                                    }
                                }

                                else if (targ && sel->peutAttaquer(ligne, colonne, tm)) {
                                    if (sel->attaque(ligne, colonne, tm, *_match)) {
                                        sel->seDeplacerVers(ligne, colonne, tm);
                                        tileDest.hasBarriere = false;
                                        _match->updateBarrieres();
                                        _match->viderSelection();
                                    }
                                }

                                else if (camp && sel->peutAttaquer(ligne, colonne, tm)) {
                                    if (sel->attaque(ligne, colonne, tm, *_match)) {
                                        sel->seDeplacerVers(ligne, colonne, tm);
                                        tileDest.hasBarriere = false;
                                        _match->updateBarrieres();
                            
                                        _match->viderSelection();
                                    }
                                }

                                else if (sel->seDeplacerVers(ligne, colonne, tm)) {
                                    _match->viderSelection();
                                }
                            }
                            
                        }
                    
                        _glisserDeposer = false;
                    }                    
                    break;

                default:
                    break;
            }
        }

    next_event:;
    }
}

/**
 * @brief Mise à jour de l'affichage selon un delta temporisé.
 */
void Renderer::update(sf::Time dt) {
    if (_scene == Scene::Jeu && _match) {
        _match->update(dt);
    }
}

/**
 * @brief Gestion du rendu.
 */
void Renderer::render() {
    _fenetre.clear({50,125,160});

    static sf::Texture texVictory;
    static sf::Sprite  sprVictory;
    static bool        victoryLoaded = false;
    if (!victoryLoaded) {
        if (!texVictory.loadFromFile("../assets/sprites/Victory.png"))
            throw std::runtime_error("Impossible de charger victory.png");
        sprVictory.setTexture(texVictory);

        auto winSize = _fenetre.getDefaultView().getSize();
        auto texSize = texVictory.getSize();
        sprVictory.setOrigin(texSize.x/2.f, texSize.y/2.f);
        sprVictory.setPosition(winSize.x/2.f, winSize.y/2.f);
        victoryLoaded = true;
    }

    if (_scene == Scene::Menu) {
        _fenetre.setView(_fenetre.getDefaultView());
        for (auto& spr : _sprites) _fenetre.draw(spr);
        for (auto& btn : _boutons) _fenetre.draw(btn.sprite);
        for (auto& txt : _textes)  _fenetre.draw(txt);
    }

    else if (_scene == Scene::MapSelector) {
        _fenetre.setView(_fenetre.getDefaultView());
        for (auto& spr : _sprites) _fenetre.draw(spr);
        for (auto& btn : _boutons) _fenetre.draw(btn.sprite);
        for (auto& txt : _textes)  _fenetre.draw(txt);
    }

    else if (_scene == Scene::Jeu && _match) {
        _fenetre.setView(_vue);
        _match->render(_fenetre);

        if (_glisserDeposer) {
            _fenetre.draw(_glisserFantome);
        }

        _fenetre.setView(_fenetre.getDefaultView());

        for (auto& spr : _sprites) _fenetre.draw(spr);

        int pieces = _match->getJoueurActif()->getPieces();
        for (auto& btn : _boutons) {
            if (btn.prix > 0) {
                if (pieces < btn.prix) {
                    btn.sprite.setColor(sf::Color(128,128,128));
                } else {
                    btn.sprite.setColor(sf::Color::White);
                }
            }
            _fenetre.draw(btn.sprite);
        }

        sf::Text status;
        status.setFont(_police);
        status.setCharacterSize(20);
        status.setFillColor(sf::Color::White);
        status.setPosition(20.f, 20.f);
        auto player = _match->getJoueurActif();
        status.setString(player->getNom() + "\nOr : " + std::to_string(player->getPieces()));
        _fenetre.draw(status);

        for (auto& txt : _textes) _fenetre.draw(txt);

        if (_match->estTerminee()) {
            sf::RectangleShape overlay;
            overlay.setSize({float(_fenetre.getSize().x), float(_fenetre.getSize().y)});
            overlay.setFillColor({0,0,0,150});
            _fenetre.draw(overlay);

            sprVictory.setColor({255,255,255,200});
            _fenetre.draw(sprVictory);

            sf::Text winnerText;
            winnerText.setFont(_police);
            winnerText.setCharacterSize(36);
            winnerText.setFillColor(sf::Color::White);
            winnerText.setString(_match->getJoueurActif()->getNom() + " est vainqueur!");
            auto b = winnerText.getLocalBounds();
            winnerText.setOrigin(b.left + b.width/2.f, b.top  + b.height/2.f);
            winnerText.setPosition(
                _fenetre.getSize().x/2.f,
                _fenetre.getSize().y/2.f + texVictory.getSize().y/2.f + 40.f
            );
            _fenetre.draw(winnerText);

            if (!_finPartieUIInitialisee) {
                effacerBoutons();
                ajouterBouton(
                    "../assets/sprites/Button_Back.png",
                    { _fenetre.getSize().x/2.f,
                      _fenetre.getSize().y/2.f + texVictory.getSize().y/2.f + 150.f},
                    [this]() {
                        _match.reset();
                        _scene = Scene::Menu;
                        _finPartieUIInitialisee = false;
                        initMenuPrincipal();
                    }
                );
                _finPartieUIInitialisee = true;
            }

            for (auto& btn : _boutons) _fenetre.draw(btn.sprite);
        }
    }

    if (_glisserAchat) {
        _fenetre.draw(_glisserFantome);
    }

    _fenetre.display();
}

