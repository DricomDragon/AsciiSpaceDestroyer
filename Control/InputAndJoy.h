#ifndef INPUTANDJOY_H_INCLUDED
#define INPUTANDJOY_H_INCLUDED


///Jovian
///Adaptation pour InputAndJoy

// Include
#include <vector>
#include "Input.h"

//Enum
#define CLAVIER_SOURIS 1
#define MANETTE 2

// Classe
class InputAndJoy : public Input
{
    public:

    InputAndJoy();
    virtual ~InputAndJoy();

    virtual void updateEvenements();

    int getAxeValue(const Uint8 axeID) const;
    bool getBoutonPad(const Uint8 bouton) const;

    void setMainControleur(int type);
    int getMainCtrl() const;


    private:

    SDL_Joystick* m_manette;
    int m_nbAxes;
    int m_nbBoutons;
    int m_controleurType;
    int const m_seuil;

    std::vector<int> m_axeValue;
    std::vector<bool> m_boutonValue;
};

#endif // INPUTANDJOY_H_INCLUDED
