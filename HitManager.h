#ifndef HITMANAGER_H
#define HITMANAGER_H

// Basiques
#include <iostream>
#include <cmath>
#include <deque>
#include <vector>

// SDL
#include <SDL/SDL.h>
#undef main
#include <SDL/SDL_gfxPrimitives.h>

// Combat
#include "Shot.h"

// Nombre de projectiles différents
#define SHOT_NB 9

// Identifiants
#define PART_ID 0
#define GUN_ID 1
#define CUTTER_ID 2
#define DISPER_ID 3
#define PHOTO_ID 4
#define MISSILE_ID 5
#define BREXIT_ID 6
#define HECTO_ID 7
#define SNIPER_ID 8

// Noms
#define PART_NAME "Part"
#define GUN_NAME "Gun"
#define CUTTER_NAME "Cutter"
#define DISPER_NAME "Disper"
#define PHOTO_NAME "ProtoSeeker"
#define MISSILE_NAME "Missile Launcher"
#define BREXIT_NAME "Brexit Launcher"
#define HECTO_NAME "Hecto Combustion"
#define SNIPER_NAME "Sniper Rifle"
#define ACTOR_NAME { PART_NAME, GUN_NAME, CUTTER_NAME, DISPER_NAME, PHOTO_NAME, MISSILE_NAME, BREXIT_NAME, HECTO_NAME, SNIPER_NAME }

// Délais
#define GUN_DECAY 256
#define CUTTER_DECAY 272
#define DISPER_DECAY 200
#define PHOTO_DECAY 100
#define MISSILE_DECAY 800
#define BREXIT_DECAY 106
#define HECTO_DECAY 350
#define SNIPER_DECAY 700
#define ACTOR_DECAY { 0, GUN_DECAY, CUTTER_DECAY, DISPER_DECAY, PHOTO_DECAY, MISSILE_DECAY, BREXIT_DECAY, HECTO_DECAY, SNIPER_DECAY }

// Charges CPU
#define PART_CPU 1
#define GUN_CPU 20
#define CUTTER_CPU 22
#define DISPER_CPU 24
#define PHOTO_CPU 22
#define MISSILE_CPU 70
#define BREXIT_CPU 27
#define HECTO_CPU 56
#define SNIPER_CPU 50
#define ACTOR_CPU { PART_CPU, GUN_CPU, CUTTER_CPU, DISPER_CPU, PHOTO_CPU, MISSILE_CPU, BREXIT_CPU, HECTO_CPU, SNIPER_CPU }
#define SHIELD_CPU 30

/**
Jovian Hersemeule
Description du HitManager :
    Le HitManager contient toutes les données à propos des projectiles tirés
    par les entittés du jeu. Elle affiche les tirs, les fait bouger, les créé,
    les détruit, et permet aux entités de tester la collision.
**/

class HitManager
{
/// Méthodes
    public:
        HitManager();
        virtual ~HitManager();

        virtual void draw( SDL_Surface* screen );

        virtual void addShot( Shot* tir );
        virtual void triggerWeapon( int weapId, Sint16 x, Sint16 y, bool ally = false );

        virtual Sint32 absorb( bool isAlly, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );
        virtual bool colide( bool isAlly, Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

        virtual void update();
        virtual void clean();

        unsigned int getDecay( unsigned int weapId );

        void setNearPt( SDL_Rect nearPt );
        void setNearAxe( SDL_Rect nearAxe );
        void setNearAlly( SDL_Rect nearAlly );

/// Attributs
    protected:
        SDL_Rect m_window;
        std::deque<Shot*> m_shots;

        SDL_Rect m_nearPt;
        SDL_Rect m_nearAxe;
        SDL_Rect m_nearAlly;
};

#endif // HITMANAGER_H
