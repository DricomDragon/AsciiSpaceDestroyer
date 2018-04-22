#ifndef BREXIT_H
#define BREXIT_H

#include "../Shot.h"

#define BREXIT_DEGAT 900
#define BREXIT_SPEED_X 18
#define BREXIT_SPEED_Y 9

/**
Jovian Hersemeule
Description du Brexit :
    L'arme Brexit lance deux tirs en diagonale, très utile
    pour flinguer les ennemis sans se faire toucher.
**/

class Brexit : public Shot
{
/// Méthodes
    public:
        Brexit();
        Brexit( Sint16 x, Sint16 y, bool up, bool ally = false );
        virtual ~Brexit();

        virtual void update();
        virtual void draw( SDL_Surface* screen );
        virtual bool damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox );

/// Attributs
    protected:
        int m_vx;
        int m_vy;
};

#endif // BREXIT_H
