#pragma once

#include "../math.hpp"
#include "sprite.hpp"

struct Drawable {
	Sprite sprite;
	glm::mat4 matrix;
};
