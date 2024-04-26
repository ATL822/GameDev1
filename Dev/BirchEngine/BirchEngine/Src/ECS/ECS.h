#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

// used to generate a unique ID for a component
inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u; // remember what the previous ID was
	return lastID++; // retrun the next ID
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>; // allows us to compare an entity's bitset to this to which components it has
using GroupBitSet = std::bitset<maxGroups>; // similar to component bit set 

using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity; // reference to its owner
	
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	Manager& manager;
	bool active = true; // if false will be remove from game
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:
	Entity(Manager& mManager) : manager(mManager) {}

	// loop through components and update using the component's update and draw funcs.
	void update()
	{
		for (auto& c : components) c->update();
	}
	void draw() 
	{
		for (auto& c : components) c->draw();
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }

	// check if entity is in a group
	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	// check if the entity has a component
	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	// add a component to an entity
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		// adds component of a certain type to the same position of the array
		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	// allow for accessing a component within the array
	template <typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private: 
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntites;

public:
	void update()
	{
		for (auto& e : entities) e->update();
	}

	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntites[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v), 
					[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
					std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntites[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntites[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};