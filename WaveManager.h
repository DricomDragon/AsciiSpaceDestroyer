#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include "HitManager.h"
#include "Foe.h"
#include <vector>

/**
Jovian Hersemeule
Description du WaveManager :
    Le WaveManager contient toutes les données à propos des ennemis, le numéro de la vague,
    leur quantité, taille, puissance. Il les affiche aussi, évidemment.
**/

class WaveManager
{
/// Méthodes
    public:
        WaveManager();
        virtual ~WaveManager();

        void giveHitManager( HitManager* theHitManager );

        virtual void draw( SDL_Surface* screen );

        virtual void update(); // Actionne ennemis, collisions et vague suivante
        virtual void clean();
        void reset(); // Réinitialise le niveau
        virtual bool allDead(); // Vrai si tout le monde est mort
        virtual bool hasWon(); // Vrai si la vague est terminée
        virtual bool hasLost(); // Vrai si un vaisseau ennemi arrive à sortir de l'écran

        void setPlanet( std::string planet ); // Charge un niveau
        std::vector< Foe* >* getFoes(); // Retourne un pointeur sur le tableau d'ennemis

        SDL_Rect nearestFromPoint( SDL_Rect pos_ref );
        SDL_Rect nearestFromAxe( SDL_Rect pos_ref );

    protected:
        virtual void nextWave();

/// Attributs
    private:
        Uint16 m_lvl;
        Uint32 m_chrono;
        bool m_allDead;
        bool m_complete;
        bool m_gameLost;
        std::string m_planet;

        HitManager* m_hiter;
        std::vector<Foe*> m_escadron;
};

#endif // WAVEMANAGER_H
