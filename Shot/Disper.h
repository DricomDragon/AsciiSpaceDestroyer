#ifndef DISPER_H
#define DISPER_H

#include "../Shot.h"

#define DISPER_DEGAT 3315
#define DISPER_SPEED 20

/**
Jovian Hersemeule
Description du Disper :
    Le tir de type Disper est rapide. Il inflige de gros dégâts localisés.
    La puissance diminue avec la distance.
**/

class Disper : public Shot
{
/// Méthodes
    public:
        Disper();
        Disper( Sint16 x, Sint16 y, Sint16 vy, bool ally = false );
        virtual ~Disper();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        Sint16 m_vx;
        Sint16 m_vy;
        Uint8 m_life;
};

#endif // DISPER_H
