#ifndef HECTO_H
#define HECTO_H

#include "../Shot.h"

#define HECTO_DEGAT 2095
#define HECTO_SPEED 7
#define HECTO_DURABILTY 6

/**
Jovian Hersemeule
Description du Hecto :
    Le tir de type Hecto est rapide et rectiligne. Il inflige de petits dégâts
    qui se dispersent en croix.
**/

class Hecto : public Shot
{
/// Méthodes
    public:
        Hecto();
        Hecto( Sint16 x, Sint16 y, bool ally = false );
        virtual ~Hecto();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        int m_velocity;
        Uint16 m_life;
};

#endif // HECTO_H
