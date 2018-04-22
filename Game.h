#ifndef GAME_H
#define GAME_H

// Basiques
#include <iostream>
#include <cstdlib>
#include <ctime>

// SDL
#include <SDL/SDL.h>
#undef main
#include <SDL/SDL_gfxPrimitives.h>

// Local
#include "HitManager.h"
#include "Ally.h"
#include "Score.h"
#include "WaveManager.h"
#include "Control/Input.h"

/**
Jovian Hersemeule
Description du Game :
    Le Game gère le jeu en cours dans sa globalité.
    La fonction run() est la fonction principale et se
    contente d'appeler alternativement menu() et rush().
**/

class Game
{
/// Méthodes
    public:
        Game();
        virtual ~Game();
        bool init();
        void run();

    protected:
        std::string menu(); // Renvoie le nom de la planète
        bool rush( std::string planet );

    private:
        void printGameOver();

/// Attributs
    private:
        SDL_Surface* m_screen;

        Ally m_player;
        Score m_score;

        HitManager m_hiter;
        WaveManager m_waver;

        Input m_input;

        Uint16 m_lvlSelect;
        Uint16 m_shipSelect;
        Uint16 m_pSelect;
};

#endif // GAME_H
