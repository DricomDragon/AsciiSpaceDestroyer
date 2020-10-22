#ifndef SHOT_H
#define SHOT_H

#include <iostream>
#include <cmath>

// SDL
#include <SDL/SDL.h>
#undef main

#include <SDL/SDL_gfxPrimitives.h>

/**
Jovian Hersemeule
Description du Shot :
    La classe shot contient toutes les infos à propos du tir : dégâts, positions.
    C'est une classe abstraite dont héritent les différents projectiles ( missiles, lasers, ..).
**/

class Shot
{
/// Méthodes
    public:
        Shot();
        Shot( Sint32 degat, bool ally = false );
        Shot( Sint32 degat, Sint16 x, Sint16 y, bool ally = false ); // Set les positons coin haut gauche
        virtual ~Shot();

        bool isIn( const SDL_Rect& hitbox );
        bool getIsAlly();
        bool getExist();
        void setExist( bool exist );
        virtual Sint32 takeDegat();

        virtual void update() = 0;
        virtual void draw( SDL_Surface* screen ) = 0;
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox ) = 0;

    protected:
        bool damageBloc( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const Uint16 y, const Uint16 x, const Uint8 bam);

/// Attributs
    protected:
        Sint16 m_x; // Position du centre du tir
        Sint16 m_y;

        Sint32 m_degat;

        bool m_ally;

        bool m_exist;
};

#endif // SHOT_H
