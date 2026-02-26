#include "sprite_atlas.hpp"

#include <memory>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::unique_ptr<SpriteAtlas> SpriteAtlas::s_instance;

void SpriteAtlas::load() {
	assert(!s_instance);
	s_instance = std::make_unique<SpriteAtlas>();

	constexpr char jsonFile[] = {
#embed "embed/atlas.json"
	};

	json data = json::parse(jsonFile);
	glm::ivec2 atlasSize(data.at("width"), data.at("height"));

	for(const auto& sprite : data.at("regions")) {
		std::string name = sprite.at("name");
		glm::ivec2 position(sprite.at("x"), sprite.at("y"));
		glm::ivec2 size(sprite.at("width"), sprite.at("height"));
		s_instance->m_sprites.emplace(name, Sprite(position, size, atlasSize));
	}
}

void SpriteAtlas::destroy() {
	s_instance.reset();
}
