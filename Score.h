#ifndef SCORE_H
#define SCORE_H

#include "HitManager.h"
#include <fstream>

/**
Jovian Hersemeule
Description du Ally :
    Contient toutes les données relatives au sauvegardes du joueur.
**/

class Score
{
/// Méthodes
    public:
        Score();
        virtual ~Score();

        void load( std::string name );
        void save();

        std::string getName();
        Uint32 getCpuLoad();
        Uint32 getWinNumber();

        bool hasShield();
        bool hasWeapon( int weapId );

        void addVictory();
        void giveLoot( std::string planet );

/// Attributs
    protected:
        std::string m_name;

        Uint32 m_cpuLoad;
        Uint32 m_winNumber;

        bool m_hasShield;
        bool m_hasWeapon[SHOT_NB];
};

#endif // SCORE_H
