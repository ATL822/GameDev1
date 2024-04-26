#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, char dir) : range(rng), speed(sp), direction(dir)
	{}
	~ProjectileComponent()
	{};

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		switch (direction) 
		{
		case 'N':
			velocity = Vector2D(0, -2);
			break;
		case 'S':
			velocity = Vector2D(0, 2);
			break;
		case 'E':
			velocity = Vector2D(2, 0);
			break;
		case 'W':
			velocity = Vector2D(-2, 0);
			break;
		default:
			velocity = Vector2D(2, 0);
			break;
		}
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
			std::cout << "Out of range" << std::endl;
			entity->destroy();
		}
	}

private:

	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	char direction = 'N';
	Vector2D velocity;
};