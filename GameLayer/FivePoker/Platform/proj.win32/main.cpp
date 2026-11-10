#include "main.h"
#include "../../src/Game/Game.h"
#include <iostream>

using GameLayer::Game::Game;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    Game *game = new Game();

    game->initialize();

    game->exec();

    game->destroyGame();

    delete game;

	return 0;
}
