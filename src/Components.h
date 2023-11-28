#ifndef GAME_COMPONENTS
#define GAME_COMPONENTS
#pragma once

#include <olcPixelGameEngine.h>
#include <string>

struct TagComponent {
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag) : Tag(tag) {}
};

struct TransformComponent {
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
};

struct SpriteRendererComponent {
	SpriteRendererComponent() = default;
	SpriteRendererComponent(const SpriteRendererComponent&) = default;
};

#endif // GAME_COMPONENTS
