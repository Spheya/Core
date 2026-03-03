#pragma once

#include "bounding_box.hpp"
#include "math.hpp"

struct Intersection {
	float distance;
	glm::vec2 normal;

	bool doesHit() const { return distance > 0.0f; }
};

inline bool overlaps(const BoundingBox& a, const BoundingBox& b) {
	return a.min.x < b.max.x && a.max.x > b.min.x && a.min.y < b.max.y && a.max.y > b.min.y;
}

Intersection rayCast(glm::vec2 origin, glm::vec2 direction, const BoundingBox& box, float maxDistance = 1e32f);
Intersection boxCast(BoundingBox& origin, glm::vec2 direction, const BoundingBox& box, float maxDistance = 1e32f);
