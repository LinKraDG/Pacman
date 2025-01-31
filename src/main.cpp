
#include <cstdio>
#include <Core/Game.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>


int main()
{
    Game::GameCreateInfo gameCI;
    gameCI.gameTitle = GAME_TITLE;
    gameCI.screenWidth = SPRITES_SIZE * MAP_WIDTH * SCREEN_RESIZE;
    gameCI.screenHeight = (FONT_HEIGHT + SPRITES_SIZE * MAP_HEIGHT) * SCREEN_RESIZE;
    gameCI.frameRateLimit = 60u;

    Game game;
    const bool gameInitialized = game.init(gameCI);

    if (gameInitialized == false)
    {
        printf("Game could not be initialized\n");
        return 0;
    }

    sf::Clock clock;

    while (game.isRunning())
    {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}
