#ifndef PART_H
#define PART_H

#include "../Shot.h"
#define PART_DEGAT 42

/**
Jovian Hersemeule
Description de Part :
    Le tir de type Part est en fait un débris de vaisseau qui
    a explosé. Cela fait très peu de dégâts.
**/

class Part : public Shot
{
/// Méthodes
    public:
        Part();
        Part( Sint16 x, Sint16 y, Sint16 speedX, Sint16 speedY, Uint8 lvl, char part, bool ally = false );
        virtual ~Part();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        Sint16 m_speedX;
        Sint16 m_speedY;
        Uint8 m_lvl;
        char m_part[2];
};

#endif // PART_H
