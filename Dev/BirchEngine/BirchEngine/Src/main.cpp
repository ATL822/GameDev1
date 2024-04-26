#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{

	const int FPS = 60; // Setting FPS constant
	const int frameDelay = 1000 / FPS; // max time between frames

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("Game Demo", 800, 640, false);

	while (game->running())
	{

		frameStart = SDL_GetTicks(); // set the start time of the tick

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart; // set the length of the tick

		// ensure the frame displays for a set amount of time
		if (frameDelay > frameTime) 
		{
			SDL_Delay(frameDelay - frameTime);
		}

	}

	game->clean();
	return 0;
}