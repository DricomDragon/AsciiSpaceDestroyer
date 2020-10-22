#include "Hecto.h"

Hecto::Hecto()
:Shot( HECTO_DEGAT ), m_velocity( -HECTO_SPEED ), m_life( HECTO_DURABILTY )
{
    //ctor
}

Hecto::Hecto( Sint16 x, Sint16 y, bool ally )
:Shot( HECTO_DEGAT, x, y, ally ), m_velocity( HECTO_SPEED ), m_life( HECTO_DURABILTY )
{
    if ( ally )
        m_velocity = -m_velocity;
}

Hecto::~Hecto()
{
    //dtor
}

void Hecto::update()
{
    m_x += m_velocity;
}

void Hecto::draw( SDL_Surface* screen )
{
    if ( m_exist )
        stringRGBA(screen, m_x - 4, m_y - 4, "H", 128, m_life * 255 / HECTO_DURABILTY, 0, 255);
}

bool Hecto::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Déterminer les cordonnées de l'impact
    Uint16 yImpct( ( m_y - hitbox.y ) / 8 );
    Uint16 xImpct( ( m_x - hitbox.x ) / 8 );

    // Bloc présent ?
    if ( solid[yImpct][xImpct] == 0 )
        return false;

    // Déstruction du centre
    damageBloc( solid, dimH, dimW, yImpct, xImpct, 255 );

    // Forts dégats sur l'immédiat
    #define HECTO_NEAREST_BAM 200
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 1, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 1, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct - 1, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct - 1, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct + 1, HECTO_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct + 1, HECTO_NEAREST_BAM );

    // Faibles dégats sur l'adjacent
    #define HECTO_FAR_BAM 60
    damageBloc( solid, dimH, dimW, yImpct - 2, xImpct, HECTO_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 2, xImpct, HECTO_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 2, HECTO_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 2, HECTO_FAR_BAM );

    // Déstruction du projectile
    m_life-- ;
    if ( m_life == 0 )
        m_exist = false;

    return true;
}
