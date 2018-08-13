#include <iostream>

#include "resources.hpp"

namespace ld {

	const std::string ASSETS_DIR = "knight_general_assets/";

void Resources::load() {
    for (const ld::TileConfig &tc : TILES) {
        std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();

        const auto &filename = tc.get_filename();

        if (!tex->loadFromFile(ASSETS_DIR + filename)) {
            throw std::runtime_error("Unable to load " + filename);
        }

        textures[filename] = std::move(tex);
    }

    for (const std::string &filename : files) {
        std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();

        if (!tex->loadFromFile(ASSETS_DIR + filename)) {
            throw std::runtime_error("Unable to load " + filename);
        }

        textures[filename] = std::move(tex);
    }

    if (!basic_font_.loadFromFile(ASSETS_DIR + "CaviarDreams.ttf")) {
        std::cout << "No font found\n";
    }

    if (music_.openFromFile(ASSETS_DIR + "little_town_orchestral.ogg")) {
        music_.setVolume(100);
        music_.setLoop(true);
        music_.play();
    } else {
        std::cout << "No music found\n";
    }
}

const sf::Texture &Resources::get_texture(const std::string &filename) const {
    return *textures.at(filename);
}
}
