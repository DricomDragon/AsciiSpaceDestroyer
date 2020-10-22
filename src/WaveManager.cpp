#include "WaveManager.h"

WaveManager::WaveManager()
:m_lvl(0), m_chrono( SDL_GetTicks() ), m_allDead( true ), m_complete( false ), m_gameLost( false ), m_planet("Sideral Space"),
 m_hiter( 0x0 )
{
    //ctor
}

WaveManager::~WaveManager()
{
    //dtor
}

void WaveManager::giveHitManager( HitManager* theHitManager )
{
    m_hiter = theHitManager;
}

void WaveManager::draw( SDL_Surface* screen )
{
    // Affiche le n° de vague
    std::string wave( "Vague numero ");
    wave += char( m_lvl + 48 );
    stringRGBA( screen, 12, 10, wave.c_str(), 255, 255, 255, 255 );

    // Affiche le nome de la planète
    stringRGBA( screen, 12, 20, m_planet.c_str(), 255, 255, 255, 255 );

    // Affiche si victoire !!!
    if ( m_complete )
    {
        stringRGBA( screen, 12, 30, "Mission accomplie !", 255, 255, 255, 255 );
        stringRGBA( screen, 12, 40, "Appuyer sur ECHAP pour rentrer a la base.", 255, 255, 255, 255 );
    }

    // Affiche vaisseau
    for ( unsigned int i(0); i < m_escadron.size(); i++ ) {
        m_escadron[i]->draw( screen );
    }
}

void WaveManager::update()
{

    // Actionne les ennemis
    for ( unsigned int i(0); i < m_escadron.size(); i++ ) {
        m_escadron[i]->update();

        // Regarde s'il sort de l'écran
        if ( m_escadron[i]->getPos()->x > 1280 )
            m_gameLost = true;
    }

    // Vague suivante
    if ( !m_allDead && !m_complete ){
        m_allDead = allDead();

        if ( m_allDead )
            m_chrono = SDL_GetTicks();
    }
    else if ( SDL_GetTicks() - m_chrono > 3000 ){
        nextWave();
    }
}

void WaveManager::clean()
{
    while ( !m_escadron.empty() ) {
        delete m_escadron.back();
        m_escadron.back() = 0x0;
        m_escadron.pop_back();
    }
}

void WaveManager::reset()
{
    clean();
    m_lvl = 0;
    m_allDead = true;
    m_complete = false;
}

bool WaveManager::allDead()
{
    for ( unsigned int i(0); i < m_escadron.size(); i++ ) {
        if ( m_escadron[i]->getIsAlive() )
            return false;
    }

    return true;
}

bool WaveManager::hasWon()
{
    return m_complete;
}

bool WaveManager::hasLost()
{
    return m_gameLost;
}

void WaveManager::setPlanet( std::string planet )
{
    m_planet = planet;
}

std::vector< Foe* >* WaveManager::getFoes()
{
    return &m_escadron;
}

void WaveManager::nextWave()
{
    // Réinitialisation de l'état de l'escadron
    m_allDead = false;
    m_gameLost = false;

    // Augmentation de la difficulté
    m_lvl ++;

    // Destruction des anciens
    clean();

    // Création des nouveaux de manière procédurale
    /*Sint16 disp( 0 );
    if ( m_lvl > 1 )
        disp = 540 / (m_lvl - 1);

    for ( unsigned int i(0); i < m_lvl; i++) {
        m_escadron.push_back( new Foe );
        m_escadron.back()->loadShape("Resources/Ships/Bloc10.txt");
        m_escadron.back()->giveHitManager( m_hiter );
        Uint16 dec( m_escadron.back()->getHitBox()->w );
        m_escadron.back()->setPos( -dec, 50 + i * disp );
    }*/

    // Par lecture
    std::string lvlPath("Resources/Levels/" + m_planet + "/w_" + (char) (m_lvl + 48) + ".txt");
    std::ifstream flux( lvlPath.c_str() );
    std::string shipName;
    Sint16 shipPos;
    Sint16 shipLate;

    while ( flux >> shipName )
    {
        // Lecture
        flux >> shipPos;
        flux >> shipLate;

        // Création
        m_escadron.push_back( new Foe );
        m_escadron.back()->loadShape("Resources/Levels/" + m_planet + "/" + shipName + ".txt");
        m_escadron.back()->giveHitManager( m_hiter );

        Uint16 dec( m_escadron.back()->getHitBox()->w );
        m_escadron.back()->setPos( -dec - shipLate, 720 * shipPos / 100 );
    }

    // Si la vague est vide, c'est que c'est fini !
    if ( m_escadron.empty() )
    {
        m_complete = true;
        m_lvl --;
    }
}

SDL_Rect WaveManager::nearestFromPoint( SDL_Rect pos_ref )
{
    SDL_Rect pos_rep( pos_ref );
    pos_rep.x = -10000; // Tire devant si pas de target

    SDL_Rect pos_i;

    Sint32 dist_min( 2147483647 ); // Nombre maximal du Sint16
    Sint32 dist_i;
    Sint32 x_i;
    Sint32 y_i;

    for ( unsigned int i(0); i < m_escadron.size(); i++ )
    {
        pos_i = m_escadron[i]->getHeartPos();

        x_i = ( pos_i.x - pos_ref.x );
        y_i = ( pos_i.y - pos_ref.y );

        dist_i = x_i * x_i + y_i * y_i;

        if ( dist_i < dist_min && m_escadron[i]->getIsAlive() )
        {
            dist_min = dist_i;
            pos_rep = pos_i;
        }
    }

    //pos_rep.x += std::sqrt( dist_min ) / 20 ;
    return pos_rep;
}

SDL_Rect WaveManager::nearestFromAxe( SDL_Rect pos_ref )
{
    SDL_Rect pos_rep( pos_ref );
    pos_rep.x = -10000; // Tire devant si pas de target

    SDL_Rect pos_i;

    Sint32 dist_min( 2147483647 ); // Nombre maximal du Sint16
    Sint32 dist_i;

    for ( unsigned int i(0); i < m_escadron.size(); i++ )
    {
        pos_i = m_escadron[i]->getHeartPos();

        dist_i = abs( pos_i.y - pos_ref.y );

        if ( dist_i < dist_min && m_escadron[i]->getIsAlive() )
        {
            dist_min = dist_i;
            pos_rep = pos_i;
        }
    }

    return pos_rep;
}
