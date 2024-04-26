#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100; // delay between frames in ms

public:

	int animIndex = 0;
	std::map<const char*, Animation> animations; // hold the animations

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTexture(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;
		if (isAnimated) {
			if (id == "player") {
				Animation cast_blue = Animation(0, 3, 100);
				Animation cast_green = Animation(1, 3, 100);
				Animation idle = Animation(2, 8, 200);
				Animation walk = Animation(3, 4, 100);

				animations.emplace("CastBlue", cast_blue);
				animations.emplace("CastGreen", cast_green);
				animations.emplace("Idle", idle);
				animations.emplace("Walk", walk);

				Play("Idle");
			}
			else if (id == "portal_blue") {
				Animation portal_blue = Animation(0, 3, 100);

				animations.emplace("portal_blue", portal_blue);

				Play("portal_blue");
			}
			else if (id == "portal_green") {
				Animation portal_green = Animation(0, 3, 100);

				animations.emplace("portal_green", portal_green);

				Play("portal_green");
			}
			else if (id == "proj_blue")
			{
				Animation proj_blue = Animation(0, 4, 100);

				animations.emplace("proj_blue", proj_blue);

				Play("proj_blue");
			}
			else if (id == "proj_green")
			{
				Animation proj_green = Animation(0, 4, 100);

				animations.emplace("proj_green", proj_green);

				Play("proj_green");
			}
		}
		setTexture(id);
	}

	~SpriteComponent()
	{	}

	void setTexture(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};