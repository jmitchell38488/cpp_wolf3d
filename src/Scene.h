#ifndef GAME_SCENE
#define GAME_SCENE
#pragma once

#include <vector>
#include <string>

class Entity;

class Registry {
public:
	Registry() = default;
	Registry(const Registry& other) = default;
};

class Scene {
public:
	Scene();
	~Scene();

public:
	Entity createEntity(const std::string& name = std::string());

private:
	std::vector<Entity*> entities;

	friend class Entity;
};

#endif // GAME_SCENE
