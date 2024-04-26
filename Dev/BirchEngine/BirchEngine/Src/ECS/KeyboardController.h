#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
#include "../AssetManager.h"
#include "SDL.h"


class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	void update() override
	{
		transform->velocity.x = 0;
		transform->velocity.y = 0;

		if (keystates[SDL_SCANCODE_W])
		{
			transform->velocity.y = -1;
			sprite->Play("Walk");
			transform->lastFacing = 'N';
		}
		else if (keystates[SDL_SCANCODE_A])
		{
			transform->velocity.x = -1;
			sprite->Play("Walk");
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			transform->lastFacing = 'W';
		}
		else if (keystates[SDL_SCANCODE_S])
		{
			transform->velocity.y = 1;
			sprite->Play("Walk");
			transform->lastFacing = 'S';
		}
		else if (keystates[SDL_SCANCODE_D])
		{
			transform->velocity.x = 1;
			sprite->spriteFlip = SDL_FLIP_NONE;
			sprite->Play("Walk");
			transform->lastFacing = 'E';
		}
		else if (keystates[SDL_SCANCODE_E] && !Game::blueProjOut)
		{
			Game::blueProjOut = true;
			sprite->Play("CastBlue");
			Game::assets->CreateProjectile(transform->position, transform->lastFacing, 500, 3, "proj_blue");
		}
		else if (keystates[SDL_SCANCODE_Q] && !Game::greenProjOut)
		{
			Game::greenProjOut = true;
			sprite->Play("CastGreen");
			Game::assets->CreateProjectile(transform->position, transform->lastFacing, 500, 3, "proj_green");
		}
		else if (keystates[SDL_SCANCODE_R])
		{
			Game::portalOnCooldown = false;			
		}
		else if (keystates[SDL_SCANCODE_C])
		{
			Game::resetPortals = true;
			Game::blueProjOut = false;
			Game::greenProjOut = false;
		}
		else if (keystates[SDL_SCANCODE_F1])
		{
			Game::debugModeOn = true;
		}
		else if (keystates[SDL_SCANCODE_F2])
		{
			Game::debugModeOn = false;
		}
		else if (keystates[SDL_SCANCODE_F3])
		{
			Game::noCollision = true;
		}
		else if (keystates[SDL_SCANCODE_F4])
		{
			Game::noCollision = false;
		}
		else if (keystates[SDL_SCANCODE_ESCAPE])
		{
			Game::isRunning = false;
		}
		else
		{
			transform->velocity.x = 0;
			sprite->Play("Idle");			
		}
	}
};