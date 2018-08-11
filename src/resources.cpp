#include "resources.hpp"

namespace ld {

void Resources::load() {
    for (const ld::TileConfig &tc : TILES) {
        std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();

        const auto &filename = tc.get_filename();

        if (!tex->loadFromFile("assets/" + filename)) {
            throw std::runtime_error("Unable to load " + filename);
        }

        textures[filename] = std::move(tex);
    }
}

const sf::Texture &Resources::get_texture(const std::string &filename) const {
    return *textures.at(filename);
}
}
