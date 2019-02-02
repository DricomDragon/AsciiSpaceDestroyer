#include "Cutter.h"

Cutter::Cutter()
:Shot( CUTTER_DEGAT ), m_velocity( -CUTTER_SPEED )
{
    //ctor
}

Cutter::Cutter( Sint16 x, Sint16 y, bool ally )
:Shot( CUTTER_DEGAT, x, y, ally ), m_velocity( CUTTER_SPEED )
{
    if ( ally )
        m_velocity = -m_velocity;
}

Cutter::~Cutter()
{
    //dtor
}

void Cutter::update()
{
    m_x += m_velocity;
}

void Cutter::draw( SDL_Surface* screen )
{
    if ( m_exist )
        stringRGBA(screen, m_x - 4, m_y - 4, "|", 255, 255, 0, 255);
}

bool Cutter::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
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

    // Déstruction de la ligne
    for ( int i(0); i < dimH; i++ ) {
        damageBloc( solid, dimH, dimW, i, xImpct, 128 );
    }


    // Le projectile a atteint sa cible
    m_exist = false;
    return true;
}
