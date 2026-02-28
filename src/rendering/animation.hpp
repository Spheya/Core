#pragma once

#include <chrono>
#include <span>
#include <vector>

#include "sprite.hpp"

class Animation {
public:
	Animation(std::span<Sprite> sprites, unsigned frameRate, unsigned animateOn, unsigned frameAnimationOffset = 0);

private:
	std::vector<Sprite> m_sprites;
};
