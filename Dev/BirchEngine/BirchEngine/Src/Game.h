#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;
class AssetManager;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();
	
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static bool portalOnCooldown;
	static bool debugModeOn;
	static bool bluePortalDown;
	static bool greenPortalDown;
	static bool resetPortals;
	static bool noCollision;
	static bool Game::blueProjOut;
	static bool Game::greenProjOut;
	static SDL_Rect camera;
	static AssetManager* assets;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupPortals,
		groupProjectiles,
		groupWallColliders,
		groupProjectileWallColliders
	};

private:
	int count = 0;
	SDL_Window *window;
};