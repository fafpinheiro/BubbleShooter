// BubbleShooter.cpp : Defines the entry point for the application.
//

#include "BubbleShooter.h"
#include "core/Game.h"


int main()
{
	Game game;
	game.Init();
	game.Run();
	game.Quit();

	return 0;
}