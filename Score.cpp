#include "Score.h"

using namespace std;

Score::Score()
:m_name("Unloaded"), m_cpuLoad( 40 ), m_winNumber( 0 ), m_hasShield( false )
{
    m_hasWeapon[ GUN_ID ] = true;

    for ( unsigned int i(2); i < SHOT_NB; i ++ )
    {
        m_hasWeapon[i] = false;
    }
}

Score::~Score()
{
    //dtor
}

void Score::load( std::string name )
{
    // Chemin
    m_name = name;
    string chemin("Resources/Saves/" + name + ".txt");

    // Flux
    ifstream loadStream( chemin.c_str() );

    // Lecture
    loadStream >> m_cpuLoad;
    loadStream >> m_winNumber;
    loadStream >> m_hasShield;

    for ( unsigned int i(1); i < SHOT_NB; i ++ )
        loadStream >> m_hasWeapon[i];
}

void Score::save()
{
    // Chemin
    string chemin("Resources/Saves/" + m_name + ".txt");

    // Flux
    ofstream writeStream( chemin.c_str() );

    // Ecriture
    writeStream << m_cpuLoad << endl;
    writeStream << m_winNumber << endl;
    writeStream << m_hasShield << endl;

    for ( unsigned int i(1); i < SHOT_NB; i ++ )
        writeStream << m_hasWeapon[i] << endl;
}

std::string Score::getName()
{
    return m_name;
}

Uint32 Score::getCpuLoad()
{
    return m_cpuLoad;
}

Uint32 Score::getWinNumber()
{
    return m_winNumber;
}

bool Score::hasShield()
{
    return m_hasShield;
}

bool Score::hasWeapon( int weapId )
{
    if ( weapId < SHOT_NB )
        return m_hasWeapon[ weapId ];
    else
        return false;
}

void Score::addVictory()
{
    m_winNumber ++;
}

void Score::giveLoot( std::string planet )
{
    // Arme en récompense
    if ( planet == "Alderande" )
    {
        m_hasWeapon[ DISPER_ID ] = true;
        return;
    }

    if ( planet == "Faiz" )
    {
        m_hasWeapon[ CUTTER_ID ] = true;
        return;
    }

    if ( planet == "Kratarite" )
    {
        m_hasWeapon[ PHOTO_ID ] = true;
        return;
    }

    if ( planet == "DistantSite2016_UK" )
    {
        m_hasWeapon[ BREXIT_ID ] = true;
        return;
    }

    if ( planet == "Telurande" )
    {
        m_hasWeapon[ HECTO_ID ] = true;
        return;
    }

    // Bouclier de combat en recompense
    if ( planet == "Titan" )
    {
        m_hasShield = true;
        return;
    }

    // Sniper
    if ( planet == "DistantSite2018_FR" )
    {
        m_hasWeapon[ SNIPER_ID ] = true;
        return;
    }

    // Sniper
    if ( planet == "Chateaubriande" )
    {
        m_hasWeapon[ MISSILE_ID ] = true;
        return;
    }

    // Majoration du CPU
    if ( planet == "BlueMoon" && m_cpuLoad < 100 )
    {
        m_cpuLoad = 100;
        return;
    }

    if ( planet == "KeplerSite429_M75" && m_cpuLoad < 200 )
    {
        m_cpuLoad = 200;
        return;
    }

    if ( planet == "Genii" && m_cpuLoad < 300 )
    {
        m_cpuLoad = 300;
        return;
    }
}
