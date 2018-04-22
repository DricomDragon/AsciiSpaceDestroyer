#include "Missile.h"

Missile::Missile()
:Shot( MISSILE_DEGAT ), m_xHim( 0 ), m_yHim( 0 ), m_hasTgt( false ), m_decay( 0 ), m_dir( MISSILE_BOOST )
{
    //ctor 1
}

Missile::Missile( Sint16 xMe, Sint16 yMe, bool ally )
:Shot( MISSILE_DEGAT, xMe, yMe, ally ), m_xHim( 0 ), m_yHim( 0 ), m_hasTgt( false ), m_decay( 0 ), m_dir( MISSILE_BOOST )
{
    //ctor 2
}

Missile::Missile( Sint16 xMe, Sint16 yMe, Sint16 xHim, Sint16 yHim, bool ally )
:Shot( MISSILE_DEGAT, xMe, yMe, ally ), m_xHim( xHim ), m_yHim( yHim ), m_hasTgt( true ), m_decay( 0 ), m_dir( MISSILE_BOOST )
{
    //ctor 3
    if ( ally )
        m_decay = abs( yHim - yMe ) / MISSILE_TURN_SPEED;
}

Missile::~Missile()
{
    //dtor
}

void Missile::update()
{
    if ( m_dir == MISSILE_BOOST )
    {
        if ( m_ally )
        {
            m_x -= MISSILE_BOOST_SPEED;
            m_xHim += 1;

            if ( m_hasTgt && m_xHim >= m_x ) turn();
        }
        else
        {
            m_x += MISSILE_BOOST_SPEED;

            if ( m_hasTgt && m_xHim <= m_x ) turn();
        }
    }
    else
    {
        m_y += m_dir * MISSILE_TURN_SPEED;
    }
}

void Missile::draw( SDL_Surface* screen )
{
    if ( !m_exist )
        return;

    switch ( m_dir )
    {
        case MISSILE_BOOST:
            if ( m_ally ) stringRGBA(screen, m_x - 8, m_y - 4, "<-", 255, 120, 0, 255);
            else stringRGBA(screen, m_x - 8, m_y - 4, "->", 255, 120, 0, 255);
            break;
        case MISSILE_DOWN:
            stringRGBA(screen, m_x - 4, m_y - 8, "|", 255, 120, 0, 255);
            stringRGBA(screen, m_x - 4, m_y, "v", 255, 120, 0, 255);
            break;
        case MISSILE_UP:
            stringRGBA(screen, m_x - 4, m_y - 8, "^", 255, 120, 0, 255);
            stringRGBA(screen, m_x - 4, m_y, "|", 255, 120, 0, 255);
            break;
        default:
            stringRGBA(screen, m_x - 4, m_y - 4, "X", 255, 120, 0, 255);
            break;
    }

}

bool Missile::damageSolid( Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    // Coordonnées impact
    Uint16 yImpct, xImpct;

    if ( m_dir == MISSILE_BOOST )
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

        if ( m_dir == MISSILE_UP ) {
            yImpct = dimH - 1;
            adder = MISSILE_UP;
        }
        else {
            yImpct = 0;
            adder = MISSILE_DOWN;
        }

        while ( solid[yImpct][xImpct] == 0 )
        {
            yImpct += adder;

            if ( yImpct >= dimH )
                return false;
        }
    }

    // Déstruction du centre
    damageBloc( solid, dimH, dimW, yImpct, xImpct, 255 );

    // Forts dégats sur l'immédiat
    #define MISSILE_NEAREST_BAM 250
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 1, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 1, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct - 1, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct - 1, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct - 1, xImpct + 1, MISSILE_NEAREST_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 1, xImpct + 1, MISSILE_NEAREST_BAM );

    // Faibles dégats sur l'adjacent
    #define MISSILE_FAR_BAM 200
    damageBloc( solid, dimH, dimW, yImpct - 2, xImpct, MISSILE_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct + 2, xImpct, MISSILE_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct - 2, MISSILE_FAR_BAM );
    damageBloc( solid, dimH, dimW, yImpct, xImpct + 2, MISSILE_FAR_BAM );

    // Déstruction du projectile
    m_exist = false;
    return true;
}

void Missile::turn()
{
    m_x = m_xHim + m_decay ;

    if ( m_yHim > m_y )
        m_dir = MISSILE_DOWN;
    else
        m_dir = MISSILE_UP;
}

