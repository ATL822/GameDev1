#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr; 
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, 800, 640};

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::portalOnCooldown = false;
bool Game::debugModeOn = false;
bool Game::bluePortalDown = false;
bool Game::greenPortalDown = false;
bool Game::resetPortals = false;
bool Game::noCollision = false;
bool Game::blueProjOut = false;
bool Game::greenProjOut = false;

auto& portal_blue(manager.addEntity());
auto& portal_green(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	assets->AddTexture("tiles", "assets/tiles_ss.png");
	assets->AddTexture("player", "assets/player_ss.png");
	assets->AddTexture("portal_blue", "assets/portal_blue.png");
	assets->AddTexture("portal_green", "assets/portal_green.png");
	assets->AddTexture("proj_blue", "assets/proj_blue.png");
	assets->AddTexture("proj_green", "assets/proj_green.png");

	map = new Map("tiles", 3, 32);

	//ECS implementation
	map->LoadMap("assets/maps/map2.map", 25, 20);

	assets->CreatePlayer(Vector2D(500, 400), 2, "player");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& wallColliders(manager.getGroup(Game::groupWallColliders));
auto& projectileWallColliders(manager.getGroup(Game::groupProjectileWallColliders));
auto& portals(manager.getGroup(Game::groupPortals));

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	auto& player = players.at(0);
	SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player->getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	if (!noCollision) {
		for (auto& c : wallColliders)
		{
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol))
			{
				player->getComponent<TransformComponent>().position = playerPos;
			}
		}
	}

	for (auto& p : projectiles)
	{
		for (auto& wc : projectileWallColliders) {
			if (Collision::AABB(wc->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				// create portal
				if (portals.size() < 2) {
					if (p->getComponent<ColliderComponent>().tag == "proj_blue" && !bluePortalDown)
					{
						assets->CreatePortal(p->getComponent<TransformComponent>().position, "portal_blue");
						bluePortalDown = true;
						blueProjOut = false;
					}
					else if (p->getComponent<ColliderComponent>().tag == "proj_green" && !greenPortalDown)
					{
						assets->CreatePortal(p->getComponent<TransformComponent>().position, "portal_green");
						greenPortalDown = true;
						greenProjOut = false;
					}
				}

				// delete projectile
				p->destroy();
			}
		}
	}

	if (portals.size() > 1) {
		if (Collision::AABB(player->getComponent<ColliderComponent>().collider, portals[0]->getComponent<ColliderComponent>().collider) && !portalOnCooldown)
		{
			Game::portalOnCooldown = true;
			player->getComponent<TransformComponent>().position = portals[1]->getComponent<TransformComponent>().position;
		}
		else if (Collision::AABB(player->getComponent<ColliderComponent>().collider, portals[1]->getComponent<ColliderComponent>().collider) && !portalOnCooldown)
		{
			Game::portalOnCooldown = true;
			player->getComponent<TransformComponent>().position = portals[0]->getComponent<TransformComponent>().position;
		}
	}

	camera.x = player->getComponent<TransformComponent>().position.x - 400;
	camera.y = player->getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	if (resetPortals)
	{
		for (auto& p : portals)
		{
			p->destroy();
		}
		bluePortalDown = false;
		greenPortalDown = false;
		resetPortals = false;
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	if (debugModeOn) {
		for (auto& wc : wallColliders)
		{
			wc->draw();
		}
		for (auto& pwc : projectileWallColliders)
		{
			pwc->draw();
		}
	}
	for (auto& e : portals)
	{
		e->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& pro : projectiles)
	{
		pro->draw();
	}
	

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}