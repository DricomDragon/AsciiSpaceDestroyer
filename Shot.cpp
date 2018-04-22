#include "Shot.h"

Shot::Shot()
:m_x(12), m_y(12), m_degat( 12 ), m_ally( true ), m_exist( true )
{
    //ctor
}

Shot::Shot( Sint32 degat, bool ally )
:m_x(12), m_y(12), m_degat( degat ), m_ally( ally ), m_exist( true )
{
    //ctor
}

Shot::Shot( Sint32 degat, Sint16 x, Sint16 y, bool ally )
:m_x(x), m_y(y), m_degat( degat ), m_ally( ally ), m_exist( true )
{
    //ctor
}

Shot::~Shot()
{
    //dtor
}


bool Shot::isIn( const SDL_Rect& hitbox )
{
    return m_exist &&
            m_x > hitbox.x &&
            m_x < hitbox.x + hitbox.w &&
            m_y > hitbox.y &&
            m_y < hitbox.y + hitbox.h ;
}

bool Shot::getIsAlly()
{
    return m_ally;
}

bool Shot::getExist()
{
    return m_exist;
}

void Shot::setExist( bool exist )
{
    m_exist = exist;
}

Sint32 Shot::takeDegat()
{
    if ( m_exist )
    {
        m_exist = false;
        return m_degat;
    }

    return 0;
}

bool Shot::damageBloc( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const Uint16 y, const Uint16 x, const Uint8 bam)
{
    // Sortie de tableau
    if ( x >= dimW || y >= dimH )
            return false;

    // Pas de dégats, renvoie false
    if ( solid[y][x] == 0 )
        return false;

    // On fait des dégats !!!
    if ( solid[y][x] < bam )
        solid[y][x] = 0;
    else
        solid[y][x] -= bam;

    return true;
}
