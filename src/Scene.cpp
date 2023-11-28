#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

class Scene;

Scene::Scene() {}
Scene::~Scene() {}

//Entity Scene::createEntity(const std::string& name) {
//	Entity entity = { m_Registry.create(), this };
//	entity.addComponent<TransformComponent>();
//	auto& tag = entity.addComponent<TagComponent>();
//	tag.Tag = name.empty() ? "UnknownEntity" : name;
//	return entity;
//}