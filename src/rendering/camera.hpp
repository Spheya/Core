#pragma once

#include "math.hpp"
#include "surface.hpp"

struct Camera {
	glm::mat4 view;
	glm::mat4 proj;
	const Surface* target;
};
