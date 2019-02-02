#ifndef FOE_H
#define FOE_H

#include "SpaceShip.h"


/**
Jovian Hersemeule
Description du Foe :
    La classe Foe hérité de SpaceShip. Un Foe est un vaisseau ennemi que l'allié doit détruire.
**/

#define FOE_MIN_GUN 1000
#define FOE_ADD_GUN 2142

#define FOE_MIN_CUTTER 2000
#define FOE_ADD_CUTTER 1042

#define FOE_MIN_DISPER 16
#define FOE_ADD_DISPER 32

#define FOE_MIN_PHOTO 1600
#define FOE_ADD_PHOTO 1

#define FOE_MIN_MISSILE 3000
#define FOE_ADD_MISSILE 2000

#define FOE_MIN_BREXIT 1500
#define FOE_ADD_BREXIT 1500

#define FOE_MIN_HECTO 500
#define FOE_ADD_HECTO 2200

#define FOE_MIN_SNIPER 2200
#define FOE_ADD_SNIPER 400

#define FOE_MIN {0, FOE_MIN_GUN, FOE_MIN_CUTTER, FOE_MIN_DISPER, FOE_MIN_PHOTO, FOE_MIN_MISSILE, FOE_MIN_BREXIT, FOE_MIN_HECTO, FOE_MIN_SNIPER}
#define FOE_ADD {0, FOE_ADD_GUN, FOE_ADD_CUTTER, FOE_ADD_DISPER, FOE_ADD_PHOTO, FOE_ADD_MISSILE, FOE_ADD_BREXIT, FOE_ADD_HECTO, FOE_ADD_SNIPER}

class Foe : public SpaceShip
{
/// Méthodes
    public:
        Foe();
        Foe(Uint16 speed);
        virtual ~Foe();

        virtual void update(); // Colisions et mouvements de l'IA

/// Attributs
    protected:
        Uint16 m_speed;
        Uint32 m_loading[SHOT_NB]; // date du dernier tir
        Uint32 m_delay[SHOT_NB]; // temps de recharge
};

#endif // FOE_H
