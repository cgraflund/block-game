#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    // Init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    // Init Game Engine
    Game game;

    // run the program as long as the window is open
    while (game.running())
    {
 
        // Update
        game.update();

        // Render
        game.render();
    }

    // End of application
    return 0;
}