#include "Photo.h"

Photo::Photo()
:Shot( PHOTO_DEGAT ), m_vx( -PHOTO_SPEED ), m_vy( 0 )
{
    //ctor 1
}

Photo::Photo( Sint16 xMe, Sint16 yMe, bool ally )
:Shot( PHOTO_DEGAT, xMe, yMe, ally ), m_vx( PHOTO_SPEED ), m_vy( 0 )
{
    if ( ally )
        m_vx = -m_vx;
}

Photo::Photo( Sint16 xMe, Sint16 yMe, Sint16 xHim, Sint16 yHim, bool ally )
:Shot( PHOTO_DEGAT, xMe, yMe, ally )
{
    m_vx = xHim - xMe ;
    m_vy = yHim - yMe ;
    Sint16 norme( std::sqrt( m_vx * m_vx + m_vy * m_vy ) ) ;

    xHim += norme / PHOTO_SPEED ; // Anticipe le mouvement de l'ennemi !

    m_vx = xHim - xMe ;
    m_vy = yHim - yMe ;
    norme = std::sqrt(m_vx * m_vx + m_vy * m_vy ) ;

    m_vx = m_vx * PHOTO_SPEED / norme ;
    m_vy = m_vy * PHOTO_SPEED / norme ;
}

Photo::~Photo()
{
    //dtor
}

void Photo::update()
{
    m_x += m_vx;
    m_y += m_vy;
}

void Photo::draw( SDL_Surface* screen )
{
    if ( m_exist )
        stringRGBA(screen, m_x - 4, m_y - 4, "o", 0, 100, 255, 255);
}

bool Photo::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Coordonnées impact
    Uint16 yImpct, xImpct;

    if ( m_vy == 0 )
    {
        // Pénétration axe horizontal
        yImpct = ( m_y - hitbox.y ) / 8;

        if ( m_ally )
            xImpct = dimW - 1;
        else
            xImpct = 0;

        while ( solid[yImpct][xImpct] == 0 )
        {
            if ( m_ally )
                xImpct --;
            else
                xImpct ++;

            if ( xImpct >= dimW )
                return false;
        }
    }
    else
    {
        // Pénétration verticale
        xImpct = ( m_x - hitbox.x ) / 8;

        Sint8 adder( 0 );

        if ( m_vy < 0 ) {
            yImpct = dimH - 1;
            adder = -1;
        }
        else {
            yImpct = 0;
            adder = 1;
        }

        while ( solid[yImpct][xImpct] == 0 )
        {
            yImpct += adder;

            if ( yImpct >= dimH )
                return false;
        }
    }

    // Déstruction du centre
    damageBloc( solid, dimH, dimW, yImpct, xImpct, 123 );

    // Déstruction du projectile
    m_exist = false;
    return true;
}
