#include "Game.h"

int main ( int argc, char** argv )
{
    /// [1] Construction
    Game myGame;

    /// [2] Initialisation
    if ( !myGame.init() )
        return 1;

    /// [3] Procédure
    myGame.run();

    /// [4] Fin
    std::cout << "Programme terminé." << std::endl;
    return 0;
}
