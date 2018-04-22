#include "Gun.h"

Gun::Gun()
:Shot( GUN_DEGAT ), m_velocity( -GUN_SPEED )
{
    //ctor
}

Gun::Gun( Sint16 x, Sint16 y, bool ally )
:Shot( GUN_DEGAT, x, y, ally ), m_velocity( GUN_SPEED )
{
    if ( ally )
        m_velocity = -m_velocity;
}

Gun::~Gun()
{
    //dtor
}

void Gun::update()
{
    m_x += m_velocity;
}

void Gun::draw( SDL_Surface* screen )
{
    if ( m_exist )
        stringRGBA(screen, m_x - 4, m_y - 4, "+", 255, 255, 255, 255);
}

bool Gun::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Déterminer l'ordonnée d'impact
    Uint16 yImpct( ( m_y - hitbox.y ) / 8 );

    // Détermination de l'abscisse d'impact
    Sint16 xImpct(0);
    if ( m_ally )
        xImpct = dimW - 1;

    while ( solid[yImpct][xImpct] == 0 )
    {
        if ( m_ally )
            xImpct --;
        else
            xImpct ++;

        if ( xImpct == -1 || xImpct == dimW )
            return false;
    }

    // Déstruction du centre
    #define GUN_CENTER_BAM 200
    damageBloc( solid, dimH, dimW, yImpct, xImpct, GUN_CENTER_BAM );

    // Forts dégats sur l'immédiat
    #define GUN_NEAREST_BAM 130
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct, GUN_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct, GUN_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 1, GUN_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 1, GUN_NEAREST_BAM );

    // Faibles dégats sur l'adjacent
    #define GUN_FAR_BAM 60
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct - 1, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct - 1, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct + 1, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct + 1, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct - 2, xImpct, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 2, xImpct, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 2, GUN_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 2, GUN_FAR_BAM );

    // Déstruction du projectile
    m_exist = false;
    return true;
}
