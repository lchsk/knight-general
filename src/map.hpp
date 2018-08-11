#ifndef MAP_H
#define MAP_H

#include <iostream>

#include "resources.hpp"
#include "tile.hpp"
#include "tile_config.hpp"
#include "unit.hpp"

namespace ld {
using MapDefinition = std::array<std::array<unsigned, 13>, 10>;

const MapDefinition map_1 = {{
    {4, 4, 4, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4},
    {0, 4, 4, 0, 0, 0, 4, 4, 4, 3, 4, 4, 4},
    {0, 1, 0, 0, 0, 1, 0, 3, 2, 2, 2, 3, 4},
    {0, 1, 1, 1, 0, 1, 0, 2, 3, 3, 2, 2, 4},
    {0, 1, 1, 1, 1, 1, 3, 3, 2, 3, 2, 2, 2},
    {0, 1, 1, 0, 0, 0, 3, 2, 2, 2, 3, 3, 3},
    {0, 0, 0, 0, 1, 0, 2, 3, 2, 2, 2, 3, 4},
    {0, 0, 1, 0, 1, 1, 2, 3, 2, 3, 2, 3, 4},
    {5, 0, 1, 0, 0, 0, 4, 4, 3, 3, 2, 3, 4},
    {4, 4, 0, 1, 0, 4, 4, 4, 3, 2, 2, 4, 4},

}};

class Map {
  public:
    Map(const ld::MapDefinition &map_definition,
        const ld::Resources &resources);

    void render(sf::RenderWindow &window) const;

  private:
    std::vector<ld::Tile> tiles;
    std::vector<std::shared_ptr<ld::Unit>> units;
};
}

#endif /* MAP_H */
