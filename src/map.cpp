#include "map.hpp"
#include "config.hpp"

namespace ld {

Map::Map(const ld::MapDefinition &map_definition,
         const ld::Resources &resources) {

    auto unit_1 = ld::Unit::build_armored_skeleton(resources);

    units.push_back(unit_1);

    const sf::Texture& texture_crosshair = resources.get_texture("crosshair.png");
    crosshair = sf::Sprite(texture_crosshair);

    for (unsigned row = 0; row < ld::config::ROWS; row++) {
        for (unsigned col = 0; col < ld::config::COLS; col++) {
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

	tiles[0].unit_ = unit_1;
};

void Map::render(sf::RenderWindow &window) const {
    for (const auto &tile : tiles) {
        window.draw(tile.sprite);
    }

    for (const auto &unit : units) {
        window.draw(unit->sprite);

        if (unit->selected_) {
            window.draw(crosshair);
        }
    }
}

    void Map::handle_left_mouse_click(const sf::Vector2i& pos)
    {
        int tile_col = static_cast<double>(pos.x) / ld::config::get_screen_width() * ld::config::COLS;
        int tile_row = static_cast<double>(pos.y) / ld::config::get_screen_height() * ld::config::ROWS;

        const auto& tile = tiles[tile_row * ld::config::COLS + tile_col];

        if (tile.unit_ == nullptr) {
            for (const auto& unit : units) {
                unit->selected_ = false;
                crosshair.setPosition(unit->sprite.getPosition());
            }
        } else {
            tile.unit_->selected_ = true;
        }

    }
}
