#include "../include/jeu.h"
#include "../include/renderer.h"
#include "../include/joueur.h"
#include "../include/villageois.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
namespace fs = std::filesystem;

int main()
{
    constexpr unsigned int windowWidth  = 800;  ///< Largeur fenêtre. 
    constexpr unsigned int windowHeight = 600;  ///< Hauteur fenêtre.

    Jeu game;

    using Players    = std::vector<std::shared_ptr<Joueur>>;    ///< Vecteur de joueurs.
    using PlayersPtr = std::shared_ptr<Players>;                ///< Pointeur vers le vecteur précédent.
    PlayersPtr players = std::make_shared<Players>();


    Renderer ren{windowWidth, windowHeight, "rkonK", game, players};

    ren.initMenuPrincipal();
    ren.run();
    return 0;
}
