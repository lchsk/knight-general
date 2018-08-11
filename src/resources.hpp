#ifndef RESOURCES_H
#define RESOURCES_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "tile_config.hpp"

namespace ld {
const std::vector<ld::TileConfig> TILES = {
    TileConfig("grass_1.png"), TileConfig("earth_1.png"),
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
