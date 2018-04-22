#ifndef ALLY_H
#define ALLY_H

#include "SpaceShip.h"

/**
Jovian Hersemeule
Description du Ally :
    Contient toutes les données relatives au vaisseau spatial du joueur.
    C'est ce que contrôle l'utilisateur.
**/

class Ally : public SpaceShip
{
/// Méthodes
    public:
        Ally();
        virtual ~Ally();

        void scroll( int rel );

        virtual void loadShape( std::string path ); // Appelle la fonction mère
        virtual void update(); // Appelle la fonction mère
        void enableThruster( bool state );

/// Attributs
    protected:
        Sint16 m_xFront;
        bool m_thruster;
};

#endif // ALLY_H
