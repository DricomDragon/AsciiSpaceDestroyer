#ifndef CUTTER_H
#define CUTTER_H

#include "../Shot.h"

#define CUTTER_DEGAT 1280
#define CUTTER_SPEED 16

/**
Jovian Hersemeule
Description du Cutter :
    Le tir de type Cutter est assez rapide et très tactique :
    il peut trancher des vaisseaux en deux dans le sens de la hauteur !
**/

class Cutter : public Shot
{
/// Méthodes
    public:
        Cutter();
        Cutter( Sint16 x, Sint16 y, bool ally = false );
        virtual ~Cutter();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        int m_velocity;
};

#endif // CUTTER_H
