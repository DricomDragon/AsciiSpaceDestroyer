#include "Part.h"

Part::Part()
:Shot( PART_DEGAT ), m_speedX( 5 ), m_speedY( 1 ), m_lvl( 255 )
{
    m_part[0] = '#';
    m_part[1] = 0;
}

Part::Part( Sint16 x, Sint16 y, Sint16 speedX, Sint16 speedY, Uint8 lvl, char part, bool ally )
:Shot( PART_DEGAT, x, y, ally ), m_speedX( speedX ), m_speedY( speedY ), m_lvl( lvl )
{
    m_part[0] = part;
    m_part[1] = 0;
}

Part::~Part()
{
    //dtor
}

void Part::update()
{
    if ( !m_exist )
        return;

    m_x += m_speedX;
    m_y += m_speedY;

    Uint8 deg( 1 + ( rand() % 20 ) );

    if ( m_lvl < deg)
        m_exist = false;

    m_lvl -= deg;
}

void Part::draw( SDL_Surface* screen )
{
    if ( m_exist )
        stringRGBA(screen, m_x - 4, m_y - 4, m_part, 255, m_lvl, m_lvl, 255);
}

bool Part::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Déterminer les coordonnées d'impact
    Uint16 yImpct( ( m_y - hitbox.y ) / 8 );
    Uint16 xImpct( ( m_x - hitbox.x ) / 8 );

    // Débris détruit si contact
    m_exist = !damageBloc( solid, dimH, dimW, yImpct, xImpct, 42 );
    return !m_exist;
}
