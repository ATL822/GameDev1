#include "AssetManager.h"
#include "ECS/Components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreatePlayer(Vector2D pos, int sc, std::string id)
{
	auto& player(manager->addEntity());
	player.addComponent<TransformComponent>(pos.x, pos.y, sc);
	player.addComponent<SpriteComponent>(id, true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>(id, 15, 24);
	player.addGroup(Game::groupPlayers);
}

void AssetManager::CreateProjectile(Vector2D pos, char dir, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, 1);
	projectile.addComponent<SpriteComponent>(id, true);
	projectile.addComponent<ProjectileComponent>(range, speed, dir);
	projectile.addComponent<ColliderComponent>(id, 8, 8);
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreatePortal(Vector2D pos, std::string id)
{
	auto& portal(manager->addEntity());
	portal.addComponent<TransformComponent>(pos.x, pos.y, 2);
	portal.addComponent<SpriteComponent>(id, true);
	portal.addComponent<ColliderComponent>(id, 15, 10);
	portal.addGroup(Game::groupPortals);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}