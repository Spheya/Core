#pragma once

#include <vector>

#include "bounding_box.hpp"
#include "intersection.hpp"

class WindowPhysics {
public:
	void update();

	bool overlaps(const BoundingBox& box) const;
	Intersection rayCast(glm::vec2 origin, glm::vec2 direction, float maxDistance = 1e32f) const;
	Intersection boxCast(BoundingBox origin, glm::vec2 direction, float maxDistance = 1e32f) const;

private:
	std::vector<BoundingBox> m_hitboxes;
};
