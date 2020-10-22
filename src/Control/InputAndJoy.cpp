#include "InputAndJoy.h"

InputAndJoy::InputAndJoy()
: m_manette(0), m_nbAxes(0), m_nbBoutons(0), m_controleurType(0), m_seuil(3600)
{
    // Démarrage du joystick
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK)<0)
    {
        std::cout << "Problème lors de l'initialisation du matériel pour les manettes : "<<SDL_GetError()<< std::endl;
    }
    else
    {
        // Ouverture du joystick
        SDL_JoystickEventState(SDL_ENABLE);
        m_manette = SDL_JoystickOpen(0);
        if (m_manette == NULL)
        {
            std::cout << "Manette non démarrée : " << SDL_GetError() << std::endl;
            setMainControleur(CLAVIER_SOURIS);
        }
        else
        {
            setMainControleur(MANETTE);
            // Détermination de la valeur des attributs
            m_nbAxes = SDL_JoystickNumAxes(m_manette);
            m_nbBoutons = SDL_JoystickNumButtons(m_manette);
            for (int i(0); i<m_nbAxes; i++)
                m_axeValue.push_back(0);
            for (int i(0); i<m_nbBoutons; i++)
                m_boutonValue.push_back(false);
        }
    }
}

InputAndJoy::~InputAndJoy()
{
    if (m_manette != 0x0) SDL_JoystickClose(m_manette);
}

void InputAndJoy::updateEvenements()
{
    // Pour éviter des mouvements fictifs de la souris, on réinitialise les coordonnées relatives
    m_xRel = 0;
    m_yRel = 0;

    // Les touches qui viennent d'être appuyées ne le sont plus
    m_touchesInstant.clear();
    m_boutonsSourisInstant.clear();

    // Boucle d'évènements
    while(SDL_PollEvent(&m_evenements))
    {
        // Switch sur le type d'évènement
        switch(m_evenements.type)
        {
        /// Evenements clavier et souris
            // Cas d'une touche enfoncée
            case SDL_KEYDOWN:
                m_touchesInstant[m_evenements.key.keysym.sym] = true;
                m_touchesInstant.push_back( m_evenements.key.keysym.sym );
            break;

            // Cas d'une touche relâchée
            case SDL_KEYUP:
                m_touchesInstant[m_evenements.key.keysym.sym] = false;
            break;

            // Cas de pression sur un bouton de la souris
            case SDL_MOUSEBUTTONDOWN:
                m_boutonsSourisHeld[m_evenements.button.button] = true;
                m_boutonsSourisInstant.push_back( m_evenements.button.button );
            break;

            // Cas du relâchement d'un bouton de la souris
            case SDL_MOUSEBUTTONUP:
                m_boutonsSourisHeld[m_evenements.button.button] = false;
            break;

            // Cas d'un mouvement de souris
            case SDL_MOUSEMOTION:
                if (m_relativeMouse)
                {
                    m_xRel = m_evenements.motion.x-m_windowHalfWidth;
                    m_yRel = m_evenements.motion.y-m_windowHalfHeight;
                }
                else
                {
                    m_x = m_evenements.motion.x;
                    m_y = m_evenements.motion.y;
                    m_xRel = m_evenements.motion.xrel;
                    m_yRel = m_evenements.motion.yrel;
                }

            break;

            // Cas de la fermeture de la fenêtre
            case SDL_QUIT:
                m_terminer = true;
            break;

        /// Evenements joystick
            case SDL_JOYAXISMOTION:
                if (m_evenements.jaxis.value>m_seuil || m_evenements.jaxis.value<-m_seuil)
                    m_axeValue[m_evenements.jaxis.axis] = m_evenements.jaxis.value;
                else
                    m_axeValue[m_evenements.jaxis.axis] = 0;
                break;
            case SDL_JOYBUTTONDOWN:
                    m_boutonValue[m_evenements.jbutton.button] = true;
                break;
            case SDL_JOYBUTTONUP:
                    m_boutonValue[m_evenements.jbutton.button] = false;
                break;

            default:
            break;
        }
    }

    // Pour éviter que la souris se barre en mode relative, on la "warp"
    if (m_relativeMouse)
         SDL_WarpMouse(m_windowHalfWidth, m_windowHalfHeight);
}

int InputAndJoy::getAxeValue(const Uint8 axeID) const
{
    if (axeID < m_nbAxes)
        return m_axeValue[axeID];
    else
        std::cout << "Axe numéro " << axeID << " non-éxistant." << std::endl;
    return -1;
}

bool InputAndJoy::getBoutonPad(const Uint8 bouton) const
{
    if (bouton<m_nbBoutons)
        return m_boutonValue[bouton];
    else
        std::cout << "Bouton numéro "<<bouton<<" non-éxistant." << std::endl;
    return false;
}

void InputAndJoy::setMainControleur(int type)
{
    if (type == CLAVIER_SOURIS || type == MANETTE)
        if (m_manette!=0 || type!=MANETTE)
            m_controleurType = type;
}

int InputAndJoy::getMainCtrl() const
{
    return m_controleurType;
}
