#include "Game.h"

using namespace std;

Game::Game()
:m_screen( 0x0 ), m_lvlSelect(0), m_shipSelect(0), m_pSelect(0)
{
    //ctor
}

Game::~Game()
{
    if ( m_screen ) SDL_FreeSurface( m_screen );
    m_screen = 0x0;
}

/// Public
bool Game::init()
{
    /// [1] Démarrage
    // [1.1] Démarrages SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        cout << "Impossible d'initialiser la SDL: " << SDL_GetError() << endl;
        return false;
    }

    // [1.2] Préparation de fermeture
    atexit(SDL_Quit);

    // [1.3] Para-fenêtre
    SDL_WM_SetCaption("ASCII Space Destroyer", 0);

    // [1.4] Seed pour nombres aléatoires
    srand( time(0) );

    /// [2] Préparation des composants
    // [2.1] Préparation de la fenêtre
    m_screen = SDL_SetVideoMode(1280, 720, 32, SDL_HWSURFACE|SDL_DOUBLEBUF );
    if ( !m_screen )
    {
        cout << "Bug à l'initialisation: " << SDL_GetError() << endl;
        return false;
    }

    // [2.2] Préparation du WaveManager
    m_waver.giveHitManager( &m_hiter );

    // [2.3] Préparation du joueur
    m_player.giveHitManager( &m_hiter );

    // [2.4] Préparation du HitManager
    //...

    /// [3] Finalisation de la procédure
    return true;
}

void Game::run()
{
    string msg;

    while ( true )
    {
        msg = menu();

        if ( msg == "Quit" )
            return;

        msg.erase(0, 2);

        if ( rush( msg ) && ( msg != "Tutoriel" ) )
        {
            m_score.addVictory();
            m_score.giveLoot( msg );
        }

        m_score.save();
    }
}

/// Protected
string Game::menu()
{
    /// [1] Préparation
    // [1.2] Chargement des index
    ifstream levelStream("Resources/Levels/index.txt");
    ifstream shipStream("Resources/Ships/index.txt");
    ifstream playerStream("Resources/Saves/index.txt");

    // [1.3] Variables
    string line;
    vector< vector<string> > levelDetail;
    vector<string> shipNames;
    vector<Uint32> shipCpu;
    vector<string> playerNames;
    bool loop( true );
    bool canDrive( false );

    // [1.3] Lecture des planètes
    while ( getline( levelStream, line ) )
    {
        if ( line[0] == '>' )
        {
            // Nouvelle planète
            levelDetail.push_back( {} );
        }

        levelDetail.back().push_back( line );
    }
    levelStream.close();

    // [1.4] Lecture des vaisseaux
    while ( getline( shipStream, line ) )
    {
        shipNames.push_back( line );
        shipCpu.push_back( 0 );
    }
    shipStream.close();

    // [1.4] Lecture des joueurs
    while ( getline( playerStream, line ) )
    {
        playerNames.push_back( line );
    }
    playerStream.close();

    // [1._] Erreurs
    if ( levelDetail.empty() || shipNames.empty() )
    {
        cout << "Problème de lecture d'un index." << endl;
        return "Quit";
    }

    // [1.1] Préparation du joueur
    m_player.loadShape("Resources/Ships/" + shipNames[m_shipSelect] + ".txt");
    m_player.setPos( 50, 100 );
    shipCpu[m_shipSelect] = m_player.countCPU();
    string weapNames[ SHOT_NB ] = ACTOR_NAME;

    m_score.load( playerNames[m_pSelect] );

    // [1. ] Pointeur souris
    m_input.placerPtr( m_screen );
    m_input.afficherPointeur( true );
    m_input.capturerPointeur( false );

    /// [2] Lancement boucle principale
    while ( loop )
    {
        /// [3] Évènements
        // [2.1] Mise à jour des évènements
        m_input.updateEvenements();

        // [2.2] Gestion de la boucle
        loop = !m_input.terminer() && !m_input.getToucheHeld( SDLK_ESCAPE );

        if ( loop && m_input.getToucheHeld( SDLK_RETURN ) && canDrive )
            loop = false;

        // [2.2] Gestion évènements choix du vaisseau
        if ( m_input.getToucheInstant( SDLK_RIGHT )
                || ( m_input.getBoutonSourisInstant( 1 )
                    && m_input.getX() < m_screen->w / 2
                    && m_input.getY() < m_screen->h / 2 ) )
        {
            m_shipSelect += 1;
            if ( m_shipSelect >= shipNames.size() ) m_shipSelect = 0;

            m_player.loadShape("Resources/Ships/" + shipNames[m_shipSelect] + ".txt");
            m_player.setPos( 50, 100 );

            if ( shipCpu[m_shipSelect] == 0 ) shipCpu[m_shipSelect] = m_player.countCPU();
        }

        if ( m_input.getToucheInstant( SDLK_LEFT )
                || ( m_input.getBoutonSourisInstant( 3 )
                    && m_input.getX() < m_screen->w / 2
                    && m_input.getY() < m_screen->h / 2 ) )
        {
            m_shipSelect -= 1;
            if ( m_shipSelect >= shipNames.size() ) m_shipSelect = shipNames.size() - 1;

            m_player.loadShape("Resources/Ships/" + shipNames[m_shipSelect] + ".txt");
            m_player.setPos( 50, 100 );

            if ( shipCpu[m_shipSelect] == 0 ) shipCpu[m_shipSelect] = m_player.countCPU();
        }

        // [2.2] Gestion évènements choix du joueur
        if ( m_input.getToucheInstant( SDLK_d )
                || ( m_input.getBoutonSourisInstant( 1 )
                    && m_input.getX() < m_screen->w / 2
                    && m_input.getY() > m_screen->h / 2 ) )
        {
            m_pSelect += 1;
            if ( m_pSelect >= playerNames.size() ) m_pSelect = 0;

            m_score.load( playerNames[ m_pSelect ] );
        }

        if ( m_input.getToucheInstant( SDLK_a ) || m_input.getToucheInstant( SDLK_q )
                || ( m_input.getBoutonSourisInstant( 3 )
                    && m_input.getX() < m_screen->w / 2
                    && m_input.getY() > m_screen->h / 2 ) )
        {
            m_pSelect -= 1;
            if ( m_pSelect >= playerNames.size() ) m_pSelect = playerNames.size() - 1;

            m_score.load( playerNames[ m_pSelect ] );
        }


        // [3.3] Gestion évènements choix du niveau
        if ( m_input.getToucheInstant( SDLK_DOWN )
                || ( m_input.getBoutonSourisInstant( 1 )
                    && m_input.getX() > m_screen->w / 2 ) )
        {
            m_lvlSelect += 1;
            if ( m_lvlSelect >= levelDetail.size() ) m_lvlSelect = 0;
        }

        if ( m_input.getToucheInstant( SDLK_UP )
                || ( m_input.getBoutonSourisInstant( 3 )
                    && m_input.getX() > m_screen->w / 2 ) )
        {
            m_lvlSelect -= 1;
            if ( m_lvlSelect >= levelDetail.size() ) m_lvlSelect = levelDetail.size() - 1;
        }

        /// [4] Validité du vaisseau
        canDrive = true;
        canDrive = canDrive && shipCpu[ m_shipSelect ] <= m_score.getCpuLoad();
        canDrive = canDrive && ( m_score.hasShield() || !m_player.hasShield() );

        for ( unsigned int i(1); i < SHOT_NB; i++ )
            canDrive = canDrive && ( m_score.hasWeapon(i) || !m_player.hasWeapon(i) );

        /// [5] Dessin des composants
        // [4.1] Nettoyage fenêtre
        SDL_FillRect(m_screen, 0, 0x000000);

        // [4.2] Affichage de vaisseau
        m_player.draw( m_screen );

        stringRGBA( m_screen, 50, 50, shipNames[m_shipSelect].c_str(), 255, 255, 255, 255 );

        stringRGBA( m_screen, 50, 60, "CPU needed :", 255, 255, 255, 255 );
        for ( unsigned int i(0); i < 6; i++ )
        {
            char s[2];
            s[0] = (char)( shipCpu[m_shipSelect] % (Uint32)pow(10, 6 - i) / pow(10, 5 - i) );
            s[0] += 48;
            s[1] = 0;
            stringRGBA( m_screen, 146 + i*8, 60, s, 255, 255, 255, 255 );
        }

        stringRGBA( m_screen, 50, 70, "Can be driven :", 255, 255, 255, 255 );
        if ( canDrive ) stringRGBA( m_screen, 178, 70, "Yes", 0, 255, 42, 255 );
        else stringRGBA( m_screen, 178, 70, "No", 255, 0, 0, 255 );

        // [4.3] Affichage du niveau
        for ( unsigned int i(0); i < levelDetail[m_lvlSelect].size(); i++)
        {
            stringRGBA( m_screen, m_screen->w / 2, 50 + i * 12, levelDetail[m_lvlSelect][i].c_str(), 255, 255, 255, 255 );
        }

        // [4.4] Affichage des données du joueur
        stringRGBA( m_screen, 50, m_screen->h / 2, m_score.getName().c_str(), 255, 255, 255, 255 );

        stringRGBA( m_screen, 50, m_screen->h / 2 + 12, "Nombre de victoires :", 255, 255, 255, 255 );
        for ( unsigned int i(0); i < 6; i++ )
        {
            char vict[2];
            vict[0] = (char)( m_score.getWinNumber() % (Uint32)pow(10, 6 - i) / pow(10, 5 - i) );
            vict[0] += 48;
            vict[1] = 0;
            stringRGBA( m_screen, 224 + i*8, m_screen->h / 2 + 12, vict, 255, 255, 255, 255 );
        }

        stringRGBA( m_screen, 50, m_screen->h / 2 + 24, "CPU disponible :", 255, 255, 255, 255 );
        for ( unsigned int i(0); i < 6; i++ )
        {
            char s[2];
            s[0] = (char)( m_score.getCpuLoad() % (Uint32)pow(10, 6 - i) / pow(10, 5 - i) );
            s[0] += 48;
            s[1] = 0;
            stringRGBA( m_screen, 186 + i*8, m_screen->h / 2 + 24, s, 255, 255, 255, 255 );
        }

        stringRGBA( m_screen, 50, m_screen->h / 2 + 36, "Possede le bouclier :", 255, 255, 255, 255 );

        if ( m_score.hasShield() ) stringRGBA( m_screen, 224, m_screen->h / 2 + 36, "Oui", 0, 255, 42, 255 );
        else stringRGBA( m_screen, 224, m_screen->h / 2 + 36, "Non", 255, 0, 0, 255 );

        for ( unsigned int i(1); i < SHOT_NB; i ++ )
        {
            stringRGBA( m_screen, 50, m_screen->h / 2 + 36 + 12 * i, weapNames[i].c_str(), 255, 255, 255, 255 );

            if ( m_score.hasWeapon( i ) )
                stringRGBA( m_screen, 60 + 8 * weapNames[i].size(), m_screen->h / 2 + 36 + 12 * i, ": Disponible", 0, 255, 42, 255 );
            else
                stringRGBA( m_screen, 60 + 8 * weapNames[i].size(), m_screen->h / 2 + 36 + 12 * i, ": Inconnue", 255, 0, 0, 255 );
        }

        // [4.5] Double buff
        SDL_Flip( m_screen );

        /// [6] Temps
        SDL_Delay( 16 ); // Economie du CPU
    }

    /// [7] Fin
    if ( m_input.getToucheHeld( SDLK_RETURN ) )
        return levelDetail[m_lvlSelect][0];
    else
        return "Quit";
}

bool Game::rush( string planet )
{
    /// [1] Préparation
    // [1.1] Préparation du joueur
    //m_player.loadShape("Resources/Ships/LightBeam.txt");
    m_player.setPos( m_screen->w, m_screen->h / 2 );

    // [1.2] Nettoyage
    m_hiter.clean();
    m_waver.reset();

    // [1.3] Paramètrage des vagues
    m_waver.setPlanet( planet );

    // [1.4] Variables du temps
    Uint32 lastFresh( SDL_GetTicks() );

    // [1.5] Capturer le pointeur
    m_input.placerPtr( m_screen );
    m_input.afficherPointeur( false );
    m_input.capturerPointeur( true );

    /// [2] Boucle principale
    while ( !m_input.terminer() && !m_input.getToucheHeld( SDLK_ESCAPE ) )
    {
        // [2.1] Mise à jour des évènements
        m_input.updateEvenements();

        // [2.2] Gestion évènements
        m_player.scroll( m_input.getYRel() );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getBoutonSourisHeld( 1 ) ) // Clic gauche
            m_player.fire( GUN_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getBoutonSourisHeld( 3 ) ) // Clic droit
            m_player.fire( CUTTER_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getToucheHeld( SDLK_a ) ) // Touche a
            m_player.fire( PHOTO_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getToucheHeld( SDLK_z ) ) // Touche z
            m_player.fire( MISSILE_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getToucheHeld( SDLK_e ) ) // Touche e
            m_player.fire( DISPER_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getToucheHeld( SDLK_r ) ) // Touche r
            m_player.fire( BREXIT_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getToucheHeld( SDLK_t ) ) // Touche r
            m_player.fire( HECTO_ID );

        if ( m_input.getToucheHeld( SDLK_SPACE ) || m_input.getToucheHeld( SDLK_y ) ) // Touche r
            m_player.fire( SNIPER_ID );

        // [2.3] Mise à jour des éléments
        m_player.enableThruster( m_waver.hasWon() );

        m_player.update();
        m_waver.update();

        m_hiter.setNearPt( m_waver.nearestFromPoint( m_player.getHeartPos() ) );
        m_hiter.setNearAxe( m_waver.nearestFromAxe( m_player.getHeartPos() ) );
        m_hiter.setNearAlly( m_player.getHeartPos() );

        m_hiter.update();

        // [2.4] Dessin des composants
        SDL_FillRect(m_screen, 0, 0x000000);

        m_waver.draw( m_screen );
        m_player.draw( m_screen );
        m_hiter.draw( m_screen );

        if ( !m_player.getIsAlive() || m_waver.hasLost() )
            printGameOver();

        SDL_Flip( m_screen );

        // [2.5] Temps
        while( SDL_GetTicks() - lastFresh < 16 )
        {
            if ( SDL_GetTicks() - lastFresh < 12 )
                SDL_Delay( 2 ); // Economie du CPU
        }
        lastFresh = SDL_GetTicks();
    }

    /// [3] Fin
    // [3. ] Si victoire
    bool victory( m_waver.hasWon() );

    // [3._] Nettoyage
    m_hiter.clean();
    m_waver.clean();

    // [3._] Attente de la touche echape
    while ( m_input.getToucheHeld( SDLK_ESCAPE ) ) m_input.updateEvenements();

    // [3._] Retour
    return m_player.getIsAlive() && victory;
}

/// Private
void Game::printGameOver()
{
    stringRGBA( m_screen, 12, 30, "GAME OVER", 255, 255, 255, 255 );
}
