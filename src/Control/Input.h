#ifndef DEF_INPUT
#define DEF_INPUT

///Jovian
///Adaptation pour InputAndJoy

// Include
#include <iostream>
#include <SDL/SDL.h>
#include <vector>


// Classe
class Input
{
    public:

    Input();
    virtual ~Input();

    virtual void updateEvenements();

    bool terminer() const;
    void afficherPointeur(bool reponse) const;
    void capturerPointeur(bool reponse);

    bool getToucheHeld(const int touche) const;
    bool getBoutonSourisHeld(const Uint8 bouton) const;
    bool getToucheInstant(const int touche) const;
    bool getBoutonSourisInstant(const Uint8 bouton) const;
    bool mouvementSouris() const;

    int getX() const;
    int getY() const;

    int getXRel() const;
    int getYRel() const;

    void placerPtr(SDL_Surface* activWindow);

    int getWinHalfH();
    int getWinHalfW();


    protected:

    SDL_Event m_evenements;
    bool m_touchesHeld[SDLK_LAST];
    std::vector<int> m_touchesInstant;
    bool m_boutonsSourisHeld[8];
    std::vector<int> m_boutonsSourisInstant;

    int m_x;
    int m_y;
    int m_xRel;
    int m_yRel;

    bool m_terminer;
    bool m_relativeMouse;

    int m_windowHalfHeight;
    int m_windowHalfWidth;
};

#endif

