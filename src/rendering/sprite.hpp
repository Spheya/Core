#pragma once

#include "math.hpp"

class Sprite {
public:
	// clang-format off
	Sprite(glm::ivec2 position, glm::ivec2 size, glm::ivec2 atlasSize) :
	    m_dimensions(size),
	    m_scaleOffset(
	        float(size.x) / float(atlasSize.x),
	        float(size.y) / float(atlasSize.y),
	        float(position.x) / float(atlasSize.x),
	        float(position.y) / float(atlasSize.y)
	    ) {}
	// clang-format on

	[[nodiscard]] glm::ivec2 getDimensions() const { return m_dimensions; }
	[[nodiscard]] int getWidth() const { return m_dimensions.x; }
	[[nodiscard]] int getHeight() const { return m_dimensions.y; }
	[[nodiscard]] glm::vec4 getScaleOffset() const { return m_scaleOffset; }

private:
	glm::ivec2 m_dimensions;
	glm::vec4 m_scaleOffset;
};
