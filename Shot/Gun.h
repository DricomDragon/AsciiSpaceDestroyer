#ifndef GUN_H
#define GUN_H

#include "../Shot.h"

#define GUN_DEGAT 1095
#define GUN_SPEED 18

/**
Jovian Hersemeule
Description du Gun :
    Le tir de type Gun est rapide et rectiligne. Il inflige de petits dégâts
    qui se dispersent en croix.
**/

class Gun : public Shot
{
/// Méthodes
    public:
        Gun();
        Gun( Sint16 x, Sint16 y, bool ally = false );
        virtual ~Gun();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        int m_velocity;
};

#endif // GUN_H
