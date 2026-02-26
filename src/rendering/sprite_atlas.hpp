#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

#include "sprite.hpp"

class SpriteAtlas {
public:
	static void load();
	static void destroy();

	[[nodiscard]] static SpriteAtlas& getInstance() {
		assert(s_instance);
		return *s_instance;
	}

	[[nodiscard]] const Sprite& get(const std::string& name) { return m_sprites.at(name); }

private:
	std::unordered_map<std::string, Sprite> m_sprites;

	static std::unique_ptr<SpriteAtlas> s_instance;
};
