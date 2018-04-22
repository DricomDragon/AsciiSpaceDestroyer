#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "HitManager.h"
#include <string>
#include <fstream>
#include <queue>

#include "Shot/Part.h"

#define SHIELD_MAX_DELAY 10000
#define SHIELD_MIN_DELAY 1000
#define SHIELD_REGEN 10
#define SHIELD_1_EARN_DELAY 350
#define SHIELD_1_EARN_HP 3000
#define SHIELD_2_EARN_DELAY 500
#define SHIELD_2_EARN_HP 30000

/**
Jovian Hersemeule
Description du SpaceShip :
    Contient toutes les données relatives au vaisseau spatial,
    sa structure, sa vie, ses armes, ses modules ...
**/

struct Actor;

struct Laser;

class SpaceShip
{
/// Méthodes
    public:
        SpaceShip();
        SpaceShip( bool ally );
        virtual ~SpaceShip();

        void giveHitManager( HitManager* theHitManager );
        void setPos( Sint16 x, Sint16 y );
        SDL_Rect* getPos();
        SDL_Rect getHeartPos();
        bool getIsAlive();
        SDL_Rect* getHitBox();

        virtual void loadShape( std::string path );
        virtual Uint32 countCPU();
        bool hasShield();
        bool hasWeapon( int weapId );

        virtual void draw( SDL_Surface* screen );
        virtual void fire( int weapId );
        virtual void update(); // Met à jour la structure du vaisseau ( collisions )

    protected:
        void destroyTabs();
        void integrity();
        void resetShields();

/// Attributs
    protected:
        Uint16 m_heartX;
        Uint16 m_heartY;
        const bool m_ally;

        SDL_Rect m_pos; // Hitbox du vaisseau

        Uint16 m_shH;
        Uint16 m_shW;
        char** m_shape; // Tableau
        Uint8** m_solid; // Tableau

        HitManager* m_hiter;
        std::vector<Actor> m_act[SHOT_NB]; // Les différentes armes du vaisseau
        Uint32 m_loading[SHOT_NB]; // Les recharges des armes

        Uint16 m_shieldLeft;
        Uint16 m_shieldMax;
        Uint32 m_shieldLast; // Date de dernier contact
        Uint32 m_shieldDelay;
        std::vector<Actor> m_shields;

        std::vector<Laser> m_lasers;
};

#endif // SPACESHIP_H
