#include "Ally.h"

Ally::Ally()
:SpaceShip( true ), m_xFront( 0 ), m_thruster( false )
{
    //ctor
}

Ally::~Ally()
{
    //dtor
}

void Ally::scroll( int rel )
{
    m_pos.y += rel ;
    if ( m_pos.y < 0 )
        m_pos.y = 0;
    else if ( m_pos.y > 720 - m_pos.h )
        m_pos.y = 720 - m_pos.h;
}

void Ally::loadShape( std::string path )
{
    // Appelle la fonction mère
    SpaceShip::loadShape( path );

    // Mets le vaisseau en position d'entrée
    m_xFront = 1200 - m_pos.w;
    m_pos.x = 1280;
    m_pos.y = 300;

    // Eteint le surboost de sortie
    m_thruster = false;
}

void Ally::update()
{
    // Appelle la fonction mère
    SpaceShip::update();

    // Avance le vaisseau en postion de combat
    if ( m_pos.x > m_xFront )
        m_pos.x -= 2;

    // Fait sortir le vaisseau de l'écran
    if ( m_thruster )
        m_pos.x -= 3;
}

void Ally::enableThruster( bool state )
{
    m_thruster = state;
}
