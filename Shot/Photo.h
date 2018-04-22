#ifndef PHOTO_H
#define PHOTO_H

#include "../Shot.h"

#define PHOTO_DEGAT 42
#define PHOTO_SPEED 22

/**
Jovian Hersemeule
Description du Photo :
    Le tir de type Photo est un projectile très rapide
    qui fonce vers la cible la plus proche mais qui
    inflige de faibles dégâts.
**/

class Photo : public Shot
{
/// Méthodes
    public:
        Photo();
        Photo( Sint16 xMe, Sint16 yMe, bool ally = false );
        Photo( Sint16 xMe, Sint16 yMe, Sint16 xHim, Sint16 yHim, bool ally = false );
        virtual ~Photo();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        Sint16 m_vx;
        Sint16 m_vy;
};

#endif // PHOTO_H
