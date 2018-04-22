#include "Disper.h"

#define DISPER_LIFE 25

Disper::Disper()
:Shot ( DISPER_DEGAT ), m_vx( -DISPER_SPEED ), m_vy( 0 ), m_life( DISPER_LIFE )
{
    //ctor
}

Disper::Disper( Sint16 x, Sint16 y, Sint16 vy, bool ally )
:Shot( DISPER_DEGAT, x, y, ally ), m_vx( DISPER_SPEED ), m_vy( vy ), m_life( DISPER_LIFE )
{
    m_vx -= abs( vy );

    if ( ally )
        m_vx = -m_vx;
}

Disper::~Disper()
{
    //dtor
}

void Disper::update()
{
    m_x += m_vx;
    m_y += m_vy;

    m_life -= 1;
    if ( m_life == 0 )
        m_exist = false;
}

void Disper::draw( SDL_Surface* screen )
{
    if ( !m_exist )
        return;

    stringRGBA(screen, m_x - 4, m_y - 4, "*", 255, m_life * 255 / DISPER_LIFE, m_life * 128 / DISPER_LIFE, 255);
}

bool Disper::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
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
    damageBloc( solid, dimH, dimW, yImpct, xImpct, 255 );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct, 255 );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct, 255 );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 1, 255 );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 1, 255 );

    // Dégats sur l'immédiat
    Uint16 nearBam( m_life * 50 + 5 );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct - 1, nearBam );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct - 1, nearBam );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct + 1, nearBam );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct + 1, nearBam );
    damageBloc( solid, dimH, dimW, yImpct - 2, xImpct, nearBam );
    damageBloc( solid, dimH, dimW, yImpct + 2, xImpct, nearBam );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 2, nearBam );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 2, nearBam );

    // Déstruction du projectile
    m_exist = false;
    return true;
}
