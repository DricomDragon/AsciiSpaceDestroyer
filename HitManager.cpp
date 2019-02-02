#include "HitManager.h"

#include "Shot/Gun.h"
#include "Shot/Part.h"
#include "Shot/Cutter.h"
#include "Shot/Disper.h"
#include "Shot/Photo.h"
#include "Shot/Missile.h"
#include "Shot/Brexit.h"
#include "Shot/Hecto.h"
#include "Shot/Sniper.h"

HitManager::HitManager()
:m_window( {0, 0, 1280, 720} )
{
    //ctor
}

HitManager::~HitManager()
{
    //dtor
}

void HitManager::draw( SDL_Surface* screen )
{
    for ( unsigned int i(0); i < m_shots.size(); i++) {
        m_shots[i]->draw( screen );
    }
}

void HitManager::addShot( Shot* tir )
{
    m_shots.push_back( tir );
}

void HitManager::triggerWeapon( int weapId, Sint16 x, Sint16 y, bool ally )
{
    switch ( weapId )
    {
    case GUN_ID :
        addShot( new Gun( x, y, ally ) );
        break;
    case CUTTER_ID :
        addShot( new Cutter( x, y, ally ) );
        break;
    case DISPER_ID :
        for ( int vy(-4); vy <= 4; vy++ )
            addShot( new Disper( x, y, vy, ally ) );
        break;
    case PHOTO_ID :
        if ( ally )
            addShot( new Photo( x, y, m_nearPt.x, m_nearPt.y, ally ) );
        else
            addShot( new Photo( x, y, m_nearAlly.x, m_nearAlly.y, ally  ) );
        break;
    case MISSILE_ID :
        if ( ally )
            addShot( new Missile( x, y, m_nearAxe.x, m_nearAxe.y, ally ) );
        else
            addShot( new Missile( x, y, m_nearAlly.x, m_nearAlly.y, ally ) );
        break;
    case BREXIT_ID :
        addShot( new Brexit(x, y, true, ally ) );
        addShot( new Brexit(x, y, false, ally ) );
        break;
    case HECTO_ID :
        addShot( new Hecto( x, y, ally ) );
        break;
    case SNIPER_ID :
        addShot( new Sniper( x, y, ally ) );
        break;
    default :
        break;
    }
}

Sint32 HitManager::absorb( bool isAlly, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox )
{
    Sint32 deg( 0 );

    for ( unsigned int i(0); i < m_shots.size(); i++) {
        if ( isAlly != m_shots[i]->getIsAlly() && m_shots[i]->isIn( hitbox ) )
            deg += m_shots[i]->takeDegat();
    }

    return deg;
}

bool HitManager::colide( bool isAlly, Uint8** solid, const Uint16 dimH, const Uint16 dimW, const SDL_Rect& hitbox  )
{
    bool rep( false );
    for ( unsigned int i(0); i < m_shots.size(); i++) {
        if ( isAlly != m_shots[i]->getIsAlly() && m_shots[i]->isIn( hitbox ) )
            rep = rep || m_shots[i]->damageSolid( solid, dimH, dimW, hitbox );
    }

    return rep;
}

void HitManager::update()
{
    // Raccourcissement de la file
    while ( !m_shots.empty() && ( !m_shots.front()->isIn(m_window) || !m_shots.front()->getExist() ) ) {
        delete m_shots.front();
        m_shots.front() = 0x0;
        m_shots.pop_front();
    }

    // Déplacement des tirs
    for ( unsigned int i(0); i < m_shots.size(); i++) {

        m_shots[i]->update() ;

        // Destruction des tirs hors champs
        if ( !m_shots[i]->isIn( m_window ) )
            m_shots[i]->setExist( false );
    }
}

void HitManager::clean()
{
    while ( !m_shots.empty() ) {
        delete m_shots.front();
        m_shots.front() = 0x0;
        m_shots.pop_front();
    }
}

unsigned int HitManager::getDecay( unsigned int weapId )
{
    if ( weapId >= SHOT_NB )
    {
        std::cout << "getDecay : Bug cher ami !" << std::endl;
        return 16;
    }

    unsigned int decTab[ SHOT_NB ] = ACTOR_DECAY;

    return decTab[ weapId ];
}

void HitManager::setNearPt( SDL_Rect nearPt )
{
    m_nearPt = nearPt;
}

void HitManager::setNearAxe( SDL_Rect nearAxe )
{
    m_nearAxe = nearAxe;
}

void HitManager::setNearAlly( SDL_Rect nearAlly )
{
    m_nearAlly = nearAlly;
}
