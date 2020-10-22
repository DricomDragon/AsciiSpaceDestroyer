#include "Sniper.h"

Sniper::Sniper()
:Shot( SNIPER_DEGAT ), m_velocity( -SNIPER_MIN_SPEED )
{
    //ctor
}

Sniper::Sniper( Sint16 x, Sint16 y, bool ally )
:Shot( SNIPER_DEGAT, x, y, ally ), m_velocity( SNIPER_MIN_SPEED )
{
    if ( ally )
        m_velocity = -m_velocity;
}

Sniper::~Sniper()
{
    //dtor
}

void Sniper::update()
{
    m_x += m_velocity;

    if ( m_ally )
    {
        m_velocity -= SNIPER_ACC;

        if ( m_velocity < -SNIPER_MAX_SPEED )
            m_velocity = -SNIPER_MAX_SPEED;
    }
    else
    {
        m_velocity += SNIPER_ACC;

        if ( m_velocity > SNIPER_MAX_SPEED )
            m_velocity = SNIPER_MAX_SPEED;
    }




}

void Sniper::draw( SDL_Surface* screen )
{
    if ( m_exist )
        stringRGBA(screen, m_x - 4, m_y - 4, "=",
                255 - (abs( m_velocity ) - SNIPER_MIN_SPEED) * 255 / (SNIPER_MAX_SPEED - SNIPER_MIN_SPEED),
                255,
                (abs( m_velocity ) - SNIPER_MIN_SPEED) * 255 / (SNIPER_MAX_SPEED - SNIPER_MIN_SPEED),
                255);
}

bool Sniper::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Déterminer l'ordonnée d'impact
    Uint16 yImpct( ( m_y - hitbox.y ) / 8 );

    // Détermination de l'abscisse d'impact
    Uint16 xImpct(0);
    if ( m_ally )
        xImpct = dimW - 1;

    while ( solid[yImpct][xImpct] == 0 )
    {
        if ( m_ally )
            xImpct --;
        else
            xImpct ++;

        if ( xImpct >= dimW )
            return false;
    }

    // Déstruction
    Uint8 power( abs( m_velocity ) * 255 / SNIPER_MAX_SPEED );

    while ( xImpct < dimW && power >= SNIPER_BREAK )
    {
        // Dommage central
        if ( damageBloc( solid, dimH, dimW, yImpct, xImpct, power ) )
        {
            // Baisse de puissance
            power -= SNIPER_BREAK;
        }

        // Dégâts latéraux
        damageBloc( solid, dimH, dimW, yImpct + 1, xImpct, power / 2 );
        damageBloc( solid, dimH, dimW, yImpct - 1, xImpct, power / 2 );

        // Mouvement
        if ( m_ally )
            xImpct --;
        else
            xImpct ++;
    }

    if ( xImpct < dimW && power > 0 )
        damageBloc( solid, dimH, dimW, yImpct, xImpct, power );

    // Déstruction du projectile
    m_exist = false;
    return true;
}
