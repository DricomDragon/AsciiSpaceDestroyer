#include "Brexit.h"

Brexit::Brexit()
:Shot( BREXIT_DEGAT ), m_vx( -BREXIT_SPEED_X), m_vy( BREXIT_SPEED_Y )
{
    //ctor
}

Brexit::Brexit( Sint16 x, Sint16 y, bool up, bool ally )
:Shot( BREXIT_DEGAT, x, y, ally ), m_vx( BREXIT_SPEED_X ), m_vy( BREXIT_SPEED_Y )
{
    if ( ally )
        m_vx = -m_vx;

    if ( up )
        m_vy = -m_vy;
}

Brexit::~Brexit()
{
    //dtor
}

void Brexit::update()
{
    m_x += m_vx;
    m_y += m_vy;
}

void Brexit::draw( SDL_Surface* screen )
{
    if ( !m_exist )
        return;

    stringRGBA(screen, m_x - 4, m_y - 4, "X", 0, 255, 0, 255);
}

bool Brexit::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Déterminer l'ordonnée d'impact discret
    Uint16 yImpct( ( m_y - hitbox.y ) / 8 );
    Uint16 xImpct( ( m_x - hitbox.x ) / 8 );

    Sint16 yAdd(1);
    if ( m_vy < 0 ) yAdd = -1;

    Sint16 xAdd(1);
    if ( m_ally ) xAdd = -1;

    // Détermination du bord de pénétration
    while ( yImpct != 0 && xImpct != 0 && yImpct != dimH - 1 && xImpct != dimW - 1 )
    {
        xImpct -= xAdd;
        yImpct -= yAdd;
    }

    // Détermination de l'abscisse d'impact
    while ( solid[yImpct][xImpct] == 0 )
    {
        xImpct += xAdd;
        yImpct += yAdd;

        if ( xImpct >= dimW || yImpct >= dimH )
            return false;
    }

    // Déstruction du centre
    damageBloc( solid, dimH, dimW, yImpct, xImpct, 150 );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + xAdd, 150 );
    damageBloc( solid, dimH, dimW, yImpct + yAdd, xImpct, 150 );

    // Déstruction du projectile
    m_exist = false;
    return true;
}
