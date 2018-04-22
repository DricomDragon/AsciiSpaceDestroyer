#include "SpaceShip.h"

struct Actor
{
    Uint16 idY;
    Uint16 idX;
};

struct Laser
{
    Uint16 idY;
    Uint16 idX;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

SpaceShip::SpaceShip()
:m_heartX(0), m_heartY(0), m_ally( false ),
 m_pos( {1100, 300, 42, 42} ),
 m_shH(0), m_shW(0), m_shape(0x0), m_solid(0x0),
 m_hiter(0x0),
 m_shieldLeft(0), m_shieldMax(0), m_shieldLast(0), m_shieldDelay(SHIELD_MAX_DELAY)
{
    for ( int i(0); i < SHOT_NB; i++ )
        m_loading[i] = 0;
}

SpaceShip::SpaceShip( bool ally )
:m_heartX(0), m_heartY(0), m_ally( ally ),
 m_pos( {1100, 300, 42, 42} ),
 m_shH(0), m_shW(0), m_shape(0x0), m_solid(0x0),
 m_hiter(0x0),
 m_shieldLeft(0), m_shieldMax(0), m_shieldLast(0), m_shieldDelay(SHIELD_MAX_DELAY)
{
    //ctor ally setter
}

SpaceShip::~SpaceShip()
{
    destroyTabs();
}

void SpaceShip::giveHitManager( HitManager* theHitManager )
{
    m_hiter = theHitManager;
}

void SpaceShip::setPos( Sint16 x, Sint16 y )
{
    m_pos.x = x;
    m_pos.y = y;
}

SDL_Rect* SpaceShip::getPos()
{
    return &m_pos;
}

SDL_Rect SpaceShip::getHeartPos()
{
    SDL_Rect rep;

    rep.x = m_pos.x + 8 * m_heartX + 4;
    rep.y = m_pos.y + 8 * m_heartY + 4;

    return rep;
}

bool SpaceShip::getIsAlive()
{
    return m_solid[m_heartY][m_heartX] > 0;
}

SDL_Rect* SpaceShip::getHitBox()
{
    return &m_pos;
}

void SpaceShip::loadShape( std::string path )
{
    /// Reinitialisation
    // Déstruction structure et modèle
    destroyTabs();

    // Initialisation des dimensions
    m_shH = 0;
    m_shW = 0;
    m_heartY = 0;
    m_heartX = 0;

    // Initialisation du bouclier
    m_shieldLeft = 0;
    m_shieldMax = 0;
    m_shieldLast = 0;
    m_shieldDelay = SHIELD_MAX_DELAY;

    /// Lecture du fichier
    // Création du flux
    std::ifstream flux( path.c_str() );

    // Lecture du modèle
    std::queue<std::string> tempo ;
    std::string line ;

    while ( std::getline(flux, line) )
    {
        tempo.push( line );
        if ( line.length() > m_shW )
            m_shW = line.length();
    }

    /// Ecriture du shape ET du solid
    // Dimensions du vaisseau
    m_shH = tempo.size();
    m_pos.h = 8 * m_shH ;
    m_pos.w = 8 * m_shW ;

    // Création des tableaux statiques
    m_shape = new char*[ m_shH ];
    for ( int i(0); i < m_shH; i ++ )
        m_shape[i] = new char[ m_shW ];

    m_solid = new Uint8*[ m_shH ];
    for ( int i(0); i < m_shH; i ++ )
        m_solid[i] = new Uint8[ m_shW ];

    // Lecture des caractères
    for ( int i(0); i < m_shH; i++ )
    {
        line = tempo.front();

        for ( unsigned int j(0); j < m_shW; j++ )
        {
            if ( j < line.length() && line[j] != ' ' ) {
                // Présence d'une pièce
                m_shape[i][j] = line.c_str()[j];
                m_solid[i][j] = 255;

                // Détection du cœur
                if ( m_shape[i][j] == 'm' ){
                    m_heartY = i;
                    m_heartX = j;
                }
            }
            else {
                // Pas de bloc à cet endroit
                m_shape[i][j] = ' ';
                m_solid[i][j] = 0;
            }
        }

        tempo.pop();
    }

    /// Détection des actionneurs
    for ( int i(0); i < m_shH; i++ )
    {
        for ( unsigned int j(0); j < m_shW - 1; j++ )
        {
            // Détection d'une arme
            if ( m_shape[i][j] == '[' )
            {
                switch ( m_shape[i][j + 1] )
                {
                case '-':
                    m_act[GUN_ID].push_back( { i, j + 1} );
                    break;
                case '<':
                    m_act[DISPER_ID].push_back( { i, j + 1} );
                    break;
                case '|':
                    m_act[CUTTER_ID].push_back( { i, j + 1} );
                    break;
                case 'O':
                    m_act[PHOTO_ID].push_back( { i, j + 1} );
                    break;
                case '>':
                    m_act[MISSILE_ID].push_back( { i, j + 1} );
                    break;
                case '{':
                    m_act[BREXIT_ID].push_back( { i, j + 1} );
                    break;
                case '~':
                    m_act[HECTO_ID].push_back( { i, j + 1} );
                    break;
                case '=':
                    m_act[SNIPER_ID].push_back( { i, j + 1} );
                    break;
                default :
                    break;
                }
            }
            // Détection d'un bouclier
            else if ( m_shape[i][j] == '(' && m_shape[i][j+1] == ')' )
            {
                m_shieldMax += SHIELD_1_EARN_HP;
                m_shieldLeft += SHIELD_1_EARN_HP;
                m_shieldDelay -= SHIELD_1_EARN_DELAY;
                m_shields.push_back( { i, j } );

                // Blocage du délai minimal
                if ( m_shieldDelay < SHIELD_MIN_DELAY )
                    m_shieldDelay = SHIELD_MIN_DELAY;
            }
            // Détection d'un pointeur
            else if ( j < m_shW - 2 && m_shape[i][j] == '{' && m_shape[i][j+2] == '}' )
            {
                // Couleur
                Uint8 r(0), g(0), b(0);
                if ( m_shape[i][j+1] == 'y' )
                {
                    b = 255;
                    g = 255;
                }
                else if ( m_shape[i][j+1] == 'r' )
                {
                    r = 255;
                }
                else if ( m_shape[i][j+1] == 'g' )
                {
                    g = 255;
                }
                else if ( m_shape[i][j+1] == 'b' )
                {
                    b = 255;
                }
                else
                {
                    r = g = b = 255;
                }

                // Ajout du pointeur
                m_lasers.push_back( { i, j + 1, r, g, b, 128 } );
            }
        }
    }

    /// Inversion du vaisseau si il est allié
    if ( m_ally ){
        char sw_c;
        Uint8 sw_s;
        // Inversion des composants
        for ( unsigned int i(0); i < m_shH; i++ ) {
            for ( unsigned int j(0); j < m_shW / 2; j++ ) {
                sw_c = m_shape[i][j];
                m_shape[i][j] = m_shape[i][m_shW - j - 1];
                m_shape[i][m_shW - j - 1] = sw_c;

                sw_s = m_solid[i][j];
                m_solid[i][j] = m_solid[i][m_shW - j - 1];
                m_solid[i][m_shW - j - 1] = sw_s;
            }
        }

        // Inversion du cockpit
        m_heartX = m_shW - m_heartX - 1;

        // Inversion des armes
        for ( unsigned int i(0); i < SHOT_NB; i++) {
            for ( unsigned int k(0); k < m_act[i].size(); k++ ){
                m_act[i][k].idX = m_shW - 1 - m_act[i][k].idX;
            }
        }

        // Inversion des boucliers
        for ( unsigned int k(0); k < m_shields.size(); k++ ){
            m_shields[k].idX = m_shW - 1 - m_shields[k].idX;
        }

        // Inversion des boucliers
        for ( unsigned int k(0); k < m_lasers.size(); k++ ){
            m_lasers[k].idX = m_shW - 1 - m_lasers[k].idX;
        }

        // Traitement des caractères orientés
        for ( unsigned int i(0); i < m_shH; i++ ) {
            for ( unsigned int j(0); j < m_shW; j++ ) {
                switch ( m_shape[i][j] ){
                case '<':
                    m_shape[i][j] = '>';
                    break;
                case '>':
                    m_shape[i][j] = '<';
                    break;
                case ']':
                    m_shape[i][j] = '[';
                    break;
                case '[':
                    m_shape[i][j] = ']';
                    break;
                case ')':
                    m_shape[i][j] = '(';
                    break;
                case '(':
                    m_shape[i][j] = ')';
                    break;
                case '}':
                    m_shape[i][j] = '{';
                    break;
                case '{':
                    m_shape[i][j] = '}';
                    break;
                case '/':
                    m_shape[i][j] = '\\';
                    break;
                case '\\':
                    m_shape[i][j] = '/';
                    break;
                default :
                    break;
                }
            }
        }
    }
}

Uint32 SpaceShip::countCPU()
{
    Uint32 c( 0 );

    // CPU des blocs
    for ( unsigned int i(0); i < m_shH; i++ )
    {
        for ( unsigned int j(0); j < m_shW; j++ )
        {
            if ( m_shape[i][j] != ' ' )
                c += PART_CPU;
        }
    }

    // CPU des armes
    Uint32 weapCpu[SHOT_NB] = ACTOR_CPU;

    for ( unsigned int i(1); i < SHOT_NB; i++ )
    {
        c += m_act[i].size() * weapCpu[i];
    }

    // CPU des boucliers
    c += SHIELD_CPU * m_shields.size();

    // Fin
    return c;
}

bool SpaceShip::hasShield()
{
    return m_shields.size() > 0;
}

bool SpaceShip::hasWeapon( int weapId )
{
    return m_act[ weapId ].size() > 0;
}

void SpaceShip::draw( SDL_Surface* screen )
{
    // N'affiche rien si déjà mort
    if ( !getIsAlive() )
        return ;

    // Enveloppe du bouclier
    Uint8 shieldLvl(128);
    if ( m_shieldLeft > 0 )
    {
        shieldLvl = m_shieldLeft * 255 / m_shieldMax ;

        for ( int i(-2); i < m_shH + 2; i++ )
        {
            stringRGBA( screen, m_pos.x - 16, m_pos.y + 8 * i, "+", 0, shieldLvl, shieldLvl, 255 );
            stringRGBA( screen, m_pos.x + m_pos.w + 8, m_pos.y + 8 * i, "+", 0, shieldLvl, shieldLvl, 255 );
        }

        for ( int j(-1); j <= m_shW; j++ )
        {
            stringRGBA( screen, m_pos.x + 8 * j, m_pos.y - 16, "+", 0, shieldLvl, shieldLvl, 255 );
            stringRGBA( screen, m_pos.x + 8 * j, m_pos.y + m_pos.h + 8, "+", 0, shieldLvl, shieldLvl, 255 );
        }
    }

    // Balayae du carénage
    for ( int i(0); i < m_shH; i++ )
    {
        for ( int j(0); j < m_shW; j++ )
        {
            // Existence d'une pièce
            if ( m_solid[i][j] > 0 )
            {
                char s[2];
                s[0] = m_shape[i][j];
                s[1] = 0;

                // Energie de bouclier disponible
                if ( m_shieldLeft > 0 )
                {
                    stringRGBA( screen, m_pos.x + 8 * j, m_pos.y + 8 * i, s, 255 - shieldLvl, m_solid[i][j], shieldLvl, 255 );

                }
                // Affichage basique
                else
                    stringRGBA( screen, m_pos.x + 8 * j, m_pos.y + 8 * i, s, 255, m_solid[i][j], m_solid[i][j], 255 );
            }
        }
    }

    // Affichage des pointeurs
    Sint16 u, v, c;
    for ( unsigned int i(0); i < m_lasers.size(); i ++ )
    {
        u = m_lasers[i].idX * 8 + m_pos.x - 9;
        v = m_lasers[i].idY * 8 + m_pos.y;

        while ( u > -8 )
        {
            stringRGBA( screen, u, v, "-", m_lasers[i].r, m_lasers[i].g, m_lasers[i].b, m_lasers[i].a );
            u -= 36;
        }
    }
}

void SpaceShip::fire( int weapId )
{
    if ( ( SDL_GetTicks() - m_loading[weapId] ) > m_hiter->getDecay( weapId ) )
    {
        for ( unsigned int k(0); k < m_act[weapId].size(); k++ )
        {
            if ( m_solid[ m_act[weapId][k].idY ][ m_act[weapId][k].idX ] > 0 )
            {
                m_hiter->triggerWeapon( weapId, m_pos.x + 8*m_act[weapId][k].idX + 4, m_pos.y + 8*m_act[weapId][k].idY + 4, m_ally );
            }
        }
        m_loading[weapId] = SDL_GetTicks();
    }
}

void SpaceShip::update()
{
    // Si le vaisseau n'est plus existant, alors rien n'existe
    if ( !getIsAlive() )
        return ;

    // S'il reste du bouclier, il peut absorber les dégats !
    if ( m_shieldLeft > 0 )
    {
        Sint32 deg( m_hiter->absorb( m_ally, m_shH, m_shW, m_pos ) );

        if ( deg > 0 )
        {
            // La recharge du bouclier est retardée
            m_shieldLast = SDL_GetTicks();

            // Le bouclier en prend un coup
            if ( deg > m_shieldLeft )
                m_shieldLeft = 0;
            else
                m_shieldLeft -= deg;
        }
    }

    // S'il reste un carénage, il peut subir des dégâts !
    else if ( m_hiter->colide( m_ally, m_solid, m_shH, m_shW, m_pos ) )
    {
        // La recharge du bouclier est retardée
        m_shieldLast = SDL_GetTicks();

        // Peut être que certains blocs ne sont plus attachés au vaisseau !
        integrity();

        // Certains boucliers sont peut être hors service !
        if ( m_shieldMax > 0 )
            resetShields();
    }

    // Recharge éventuelle du bouclier
    if ( m_shieldLeft < m_shieldMax && ( SDL_GetTicks() - m_shieldLast > m_shieldDelay ) )
    {
        m_shieldLeft += SHIELD_REGEN ;

        if ( m_shieldLeft > m_shieldMax ) m_shieldLeft = m_shieldMax ;
    }
}

void SpaceShip::destroyTabs()
{
    // Destruction du modèle graphic
    if ( m_shape != 0x0 )
    {
        for ( int i(0); i < m_shH; i++ ) {
            delete[] m_shape[i];
            m_shape[i] = 0x0;
        }

        delete[] m_shape;
        m_shape = 0x0;
    }

    // Destruction de la carte du blindage
    if ( m_solid != 0x0 )
    {
        for ( int i(0); i < m_shH; i++ ) {
            delete[] m_solid[i];
            m_solid[i] = 0x0;
        }

        delete[] m_solid;
        m_solid = 0x0;
    }

    // Destruction des armes
    for ( unsigned int i(0); i < SHOT_NB; i++ )
    {
        m_act[i].clear();
    }

    // Destruction du bouclier
    m_shields.clear();

    // Destruction des pointeurs
    m_lasers.clear();
}

void SpaceShip::integrity()
{
    // Création du tableau des liens
    Sint8 network[ m_shH ][ m_shW ];
    for ( int i(0); i < m_shH; i++ )
    {
        for ( int j(0); j < m_shW; j++ )
        {
            network[i][j] = -1;
        }
    }

    // Création du réseau
    if ( getIsAlive() )
    {
        network[ m_heartY ][ m_heartX ] = 1;
        std::queue<Actor> q; // Couples à tester
        Uint16 y, x;

        if ( m_heartY > 0 ) q.push( { m_heartY - 1, m_heartX } );
        if ( m_heartX > 0 ) q.push( { m_heartY, m_heartX - 1 } );
        if ( m_heartY + 1 < m_shH ) q.push( { m_heartY + 1, m_heartX } );
        if ( m_heartX + 1 < m_shW ) q.push( { m_heartY, m_heartX + 1} );

        while ( !q.empty() )
        {
            y = q.front().idY;
            x = q.front().idX;

            if ( m_solid[ y ][ x ] == 0 )
            {
                // Bloc détruit
                network[ y ][ x ] = 0;
            }
            else if ( ( y > 0 && network[ y - 1 ][ x ] == 1 ) // Haut
                    || ( y < m_shH - 1 && network[ y + 1 ][ x ] == 1 ) // Bas
                    || ( x > 0 && network[ y ][ x - 1 ] == 1 ) // Gauche
                    || ( x < m_shW - 1 && network[ y ][ x + 1 ] == 1 ) ) // Droite
            {
                // Bloc présent
                network[ y ][ x ] = 1;

                // Connexion des blocs adjacents
                if ( y > 0 && network[ y - 1 ][ x ] == -1 )// Haut
                    q.push( { y - 1, x } );


                if ( y < m_shH - 1 && network[ y + 1 ][ x ] == -1 )// Bas
                    q.push( { y + 1, x } );


                if ( x > 0 && network[ y ][ x - 1 ] == -1 )// Gauche
                    q.push( { y, x - 1 } );


                if ( x < m_shW - 1 && network[ y ][ x + 1 ] == -1 )// Doite
                    q.push( { y, x + 1 } );
            }
            else
            {
                // Bloc non connecté
                network[ y ][ x ] = 0;
            }

            q.pop();
        }
    }

    // Destruction des pièces non connectées
    for ( int i(0); i < m_shH; i++ )
    {
        for ( int j(0); j < m_shW; j++ )
        {
            if ( m_solid[i][j] > 0 && network[i][j] == -1 ) {
                Sint16 vx( j - m_heartX );
                Sint16 vy( i - m_heartY );

                m_hiter->addShot( new Part( m_pos.x + j*8, m_pos.y + i*8, vx, vy, m_solid[i][j], m_shape[i][j], m_ally ) );
                m_solid[i][j] = 0;
            }
        }
    }
}

void SpaceShip::resetShields()
{
    m_shieldDelay = SHIELD_MAX_DELAY;
    m_shieldMax = 0;

    for ( unsigned int k(0); k < m_shields.size(); k++ )
    {
        if ( m_solid[ m_shields[k].idY ][ m_shields[k].idX ] > 0 )
        {
            m_shieldDelay -= SHIELD_1_EARN_DELAY;
            m_shieldMax += SHIELD_1_EARN_HP;
        }
    }

    if ( m_shieldLeft > m_shieldMax )
        m_shieldLeft = m_shieldMax;

    if ( m_shieldDelay < SHIELD_MIN_DELAY )
        m_shieldDelay = SHIELD_MIN_DELAY;
}
