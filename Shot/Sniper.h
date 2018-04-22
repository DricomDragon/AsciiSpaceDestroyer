#ifndef SNIPER_H
#define SNIPER_H

#include "../Shot.h"

#define SNIPER_DEGAT 4000
#define SNIPER_MIN_SPEED 1
#define SNIPER_MAX_SPEED 40
#define SNIPER_ACC 1
#define SNIPER_BREAK 10

/**
Jovian Hersemeule
Description du Sniper :
    Le tir de type Sniper est horizontal et accelerant. PLus sa vitesse est
    grande, plus il fait de dégâts.
**/

class Sniper : public Shot
{
/// Méthodes
    public:
        Sniper();
        Sniper( Sint16 x, Sint16 y, bool ally = false );
        virtual ~Sniper();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        int m_velocity;
};

#endif // SNIPER_H
