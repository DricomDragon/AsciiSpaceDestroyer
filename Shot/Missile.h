#ifndef MISSILE_H
#define MISSILE_H

#include "../Shot.h"

#define MISSILE_DEGAT 3055
#define MISSILE_BOOST_SPEED 28
#define MISSILE_TURN_SPEED 22

/**
Jovian Hersemeule
Description du Missile :
    Le tir de type missile est assez lent à recharger mais offre
    des capacités extraordinaires de destruction. Le missile commence
    une approche en ligne droite avant de tourner à 90° vers le coeur
    de la cible.
**/

class Missile : public Shot
{
/// Méthodes
    public:
        Missile();
        Missile( Sint16 xMe, Sint16 yMe, bool ally = false );
        Missile( Sint16 xMe, Sint16 yMe, Sint16 xHim, Sint16 yHim, bool ally = false );
        virtual ~Missile();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

    private:
        void turn();

/// Attributs
    private:
        Sint16 m_xHim;
        Sint16 m_yHim;

        bool m_hasTgt;
        Sint16 m_decay;

        Sint8 m_dir;
        #define MISSILE_UP -1
        #define MISSILE_DOWN 1
        #define MISSILE_BOOST 0
};

#endif // MISSILE_H
