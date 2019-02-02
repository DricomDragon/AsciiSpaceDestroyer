#include "Foe.h"

Foe::Foe()
:SpaceShip( false ), m_speed( 1 )
{
    Uint32 tabMins[SHOT_NB] = FOE_MIN;
    Uint32 tabAdds[SHOT_NB] = FOE_ADD;

    for ( unsigned int i(1); i < SHOT_NB; i++ )
    {
        m_loading[i] = SDL_GetTicks();
        m_delay[i] = tabMins[i] + ( rand() % tabAdds[i] );
    }
}

Foe::Foe( Uint16 speed )
:SpaceShip( false ), m_speed( speed )
{
    Uint32 tabMins[SHOT_NB] = FOE_MIN;
    Uint32 tabAdds[SHOT_NB] = FOE_ADD;

    for ( unsigned int i(1); i < SHOT_NB; i++ )
    {
        m_loading[i] = SDL_GetTicks();
        m_delay[i] = tabMins[i] + ( rand() % tabAdds[i] );
    }
}

Foe::~Foe()
{
    //dtor
}

void Foe::update()
{
    // Collision
    SpaceShip::update();

    // Teste l'existence
    if ( !getIsAlive() )
        return ;

    // Déplacement
    m_pos.x += m_speed;

    // Feu
    Uint32 last( SDL_GetTicks() );
    Uint32 tabMins[SHOT_NB] = FOE_MIN;
    Uint32 tabAdds[SHOT_NB] = FOE_ADD;

    for ( unsigned int i(1); i < SHOT_NB; i++ )
    {
        if ( last - m_loading[i] > m_delay[i] ){
            fire( i );

            m_loading[i] = SDL_GetTicks();
            m_delay[i] = tabMins[i] + ( rand() % tabAdds[i] );
        }
    }
}
