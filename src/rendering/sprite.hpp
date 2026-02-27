#pragma once

#include "math.hpp"

class Sprite {
public:
	// clang-format off
	Sprite(glm::uvec2 position, glm::uvec2 size, glm::uvec2 atlasSize) :
	    m_dimensions(size),
	    m_scaleOffset(
	        float(size.x) / float(atlasSize.x),
	        float(size.y) / float(atlasSize.y),
	        float(position.x) / float(atlasSize.x),
	        float(position.y) / float(atlasSize.y)
	    ) {}
	// clang-format on

	[[nodiscard]] glm::uvec2 getDimensions() const { return m_dimensions; }
	[[nodiscard]] unsigned getWidth() const { return m_dimensions.x; }
	[[nodiscard]] unsigned getHeight() const { return m_dimensions.y; }
	[[nodiscard]] glm::vec4 getScaleOffset() const { return m_scaleOffset; }

private:
	glm::uvec2 m_dimensions;
	glm::vec4 m_scaleOffset;
};
