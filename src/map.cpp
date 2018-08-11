#include "map.hpp"

namespace ld {

Map::Map(const ld::MapDefinition &map_definition,
         const ld::Resources &resources) {
    for (unsigned row = 0; row < 10; row++) {
        for (unsigned col = 0; col < 13; col++) {
            unsigned tile_id = map_definition[row][col];
            const ld::TileConfig &tc = TILES[tile_id];

            try {
                const sf::Texture &texture =
                    resources.get_texture(tc.get_filename());
                tiles.push_back(ld::Tile(texture, col, row));
            } catch (const std::out_of_range &) {
                std::cout << "Filename " << tc.get_filename() << " not found"
                          << std::endl;
            }
        }
    }
};

void Map::render(sf::RenderWindow &window) const {
    for (const auto &tile : tiles) {
        window.draw(tile.sprite);
    }
}
}
