#ifndef GAME_ENTITY
#define GAME_ENTITY

#pragma once

#include "Scene.h"

class Entity {
public:
	Entity() = default;
	Entity(Scene* scene);
	Entity(const Entity& other) = default;

public:
	//template<typename T, typename... Args>
	//T& addComponent(Args&&... args) {
	//	return gScene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	//}

	//template<typename T>
	//T& getComponent() {
	//	return gScene->m_Registry.get<T>(m_EntityHandle);
	//}

	//template<typename T>
	//bool hasComponent() {
	//	return gScene->m_Registry.any_of<T>(m_EntityHandle);
	//}

	//template<typename T>
	//void removeComponent() {
	//	gScene->m_Registry.remove<T>(m_EntityHandle);
	//}

	//operator bool() const { return m_EntityHandle != 0; }
private:
	//entt::entity m_EntityHandle{ 0 };
	Scene* gScene = nullptr;
};

#endif // GAME_ENTITY