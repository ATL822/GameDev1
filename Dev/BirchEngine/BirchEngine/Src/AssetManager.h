#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	// game objects
	void CreatePlayer(Vector2D pos, int sc, std::string id);
	void CreateProjectile(Vector2D pos, char dir, int range, int speed, std::string id);
	void CreatePortal(Vector2D pos, std::string id);

	// texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;

};