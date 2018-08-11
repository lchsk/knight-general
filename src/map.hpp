#ifndef MAP_H
#define MAP_H

#include <iostream>

#include "resources.hpp"
#include "tile.hpp"
#include "tile_config.hpp"

namespace ld {
using MapDefinition = std::array<std::array<unsigned, 13>, 10>;

const MapDefinition map_1 = {{
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},

}};

class Map {
  public:
    Map(const ld::MapDefinition &map_definition,
        const ld::Resources &resources);

    void render(sf::RenderWindow &window) const;

  private:
    std::vector<ld::Tile> tiles;
};
}

#endif /* MAP_H */
