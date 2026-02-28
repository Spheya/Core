#pragma once

#include "../math.hpp"
#include "sprite.hpp"

struct SpriteDrawable {
	Sprite sprite;
	glm::mat4 matrix;
};
