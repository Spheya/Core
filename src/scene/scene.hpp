#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <vector>

#include "physics/bounding_box.hpp"
#include "physics/intersection.hpp"
#include "rendering/sprite_drawable.hpp"
#include "time.hpp"

class Entity;

class Scene {
public:
	Scene() = default;
	Scene(Scene&) = delete;
	Scene& operator=(Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;
	~Scene() = default;

	Entity* addEntity(std::unique_ptr<Entity> entity);

	void update(const Time& time);

	bool overlaps(const BoundingBox& box, uint32_t flags = ~0u) const;
	Intersection rayCast(glm::vec2 origin, glm::vec2 direction, float maxDistance = 1e32f, bool excludeExit = false, uint32_t flags = ~0u) const;
	Intersection boxCast(
	    const BoundingBox& origin, glm::vec2 direction, float maxDistance = 1e32f, bool excludeExit = false, uint32_t flags = ~0u
	) const;

	std::span<const SpriteDrawable> buildSprites();

private:
	std::vector<std::unique_ptr<Entity>> m_entities;

	std::vector<SpriteDrawable> m_sprites;
	std::vector<BoundingBox> m_clickRegions;
};
