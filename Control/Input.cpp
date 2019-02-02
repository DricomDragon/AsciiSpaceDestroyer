#include "Input.h"


// Constructeur et Destructeur
Input::Input()
: m_x(0), m_y(0), m_xRel(0), m_yRel(0),
m_terminer(false), m_relativeMouse(false), m_windowHalfHeight(0), m_windowHalfWidth(0)
{
    // Initialisation du tableau m_touchesHeld[]
    for(int i(0); i < SDLK_LAST; i++)
        m_touchesHeld[i] = false;
    std::cout << "Il y a "<<SDLK_LAST<<" touches sur le clavier."<<std::endl;

    // Initialisation du tableau m_boutonsSourisHeld[]
    for(int i(0); i < 8; i++)
        m_boutonsSourisHeld[i] = false;
}


Input::~Input()
{}


// Méthodes
void Input::updateEvenements()
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
            // Cas d'une touche enfoncée
            case SDL_KEYDOWN:
                m_touchesHeld[m_evenements.key.keysym.sym] = true;
                m_touchesInstant.push_back( m_evenements.key.keysym.sym );
            break;

            // Cas d'une touche relâchée
            case SDL_KEYUP:
                m_touchesHeld[m_evenements.key.keysym.sym] = false;
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

            // Les autres ne nous interessent pas : on évite de faire râler g++
            default:
            break;
        }
    }

    // Pour éviter que la souris se barre en mode relative, on la "warp"
    if (m_relativeMouse)
         SDL_WarpMouse(m_windowHalfWidth, m_windowHalfHeight);
}


bool Input::terminer() const
{
    return m_terminer;
}


void Input::afficherPointeur(bool reponse) const
{
    if(reponse)
        SDL_ShowCursor(SDL_ENABLE);

    else
        SDL_ShowCursor(SDL_DISABLE);
}


void Input::capturerPointeur(bool reponse)
{
    m_relativeMouse = reponse;
}



// Getters

bool Input::getToucheHeld(const int touche) const
{
    return m_touchesHeld[touche];
}


bool Input::getBoutonSourisHeld(const Uint8 bouton) const
{
    return m_boutonsSourisHeld[bouton];
}

bool Input::getToucheInstant(const int touche) const
{
    for ( unsigned int i(0); i < m_touchesInstant.size(); i++ ){
        if ( m_touchesInstant[i] == touche )
            return true;
    }

    return false;
}


bool Input::getBoutonSourisInstant(const Uint8 bouton) const
{
    for ( unsigned int i(0); i < m_boutonsSourisInstant.size(); i++ ){
        if ( m_boutonsSourisInstant[i] == bouton )
            return true;
    }

    return false;
}


bool Input::mouvementSouris() const
{
    if(m_xRel == 0 && m_yRel == 0)
        return false;

    else
        return true;
}


// Getters concernant la position du curseur

int Input::getX() const
{
    return m_x;
}

int Input::getY() const
{
    return m_y;
}

int Input::getXRel() const
{
    return m_xRel;
}

int Input::getYRel() const
{
    return m_yRel;
}

void Input::placerPtr(SDL_Surface* activWindow)
{
    // Détermination de l'endroit de capture du pointeur
    m_windowHalfWidth = activWindow->w / 2;
    m_windowHalfHeight = activWindow->h / 2;
}

int Input::getWinHalfH()
{
    return m_windowHalfHeight;
}

int Input::getWinHalfW()
{
    return m_windowHalfWidth;
}

