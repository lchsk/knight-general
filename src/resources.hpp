#ifndef RESOURCES_H
#define RESOURCES_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "tile_config.hpp"

namespace ld {
const std::vector<ld::TileConfig> TILES = {
    TileConfig("grass_1.png", ld::TileType::Grass), // 0
    TileConfig("grass_2.png", ld::TileType::Grass), // 1

    TileConfig("earth_1.png", ld::TileType::Earth), // 2
    TileConfig("earth_2.png", ld::TileType::Earth), // 3

    TileConfig("water_1.png", ld::TileType::Water), // 4
    TileConfig("water_2.png", ld::TileType::Water), // 5

    // TileConfig("light_water_1.png"), // 6
    // TileConfig("light_water_2.png"), // 7

    // TileConfig("light_earth_1.png"), TileConfig("light_earth_2.png"),

    // TileConfig("purple_1.png"),      TileConfig("purple_2.png"),
};

const std::vector<std::string> files = {
    "skeleton_armored_right.png", "crosshair.png",
};

class Resources {

  public:
    void load();
    const sf::Texture &get_texture(const std::string &filename) const;

  private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};
}

#endif /* RESOURCES_H */
