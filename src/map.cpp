#include "map.hpp"
#include "config.hpp"

namespace ld {

Map::Map(const ld::MapDefinition &map_definition,
         const ld::Resources &resources)
    : player_1_(std::make_shared<ld::Player>()){

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
                tiles.push_back(ld::Tile(texture, row, col));
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

        // Tile that was just clicked on
        auto& selected_tile = tiles[tile_row * ld::config::COLS + tile_col];

        if (selected_tile.unit_ == nullptr) {
            // There's no unit on selected tile => Move
            if (player_1_->selected_unit_) {
                bool valid_move = false;

                ld::Tile* unit_tile = nullptr;

                // Check if can move this unit
                for (auto& tile : tiles) {
                    if (tile.unit_ == player_1_->selected_unit_) {
                        unit_tile = &tile;
                        break;
                    }
                }

                if (unit_tile == nullptr) {
                    std::cout << "Could not find selected unit's tile\n";
                    return;
                }

                const int row_diff = std::abs(selected_tile.row_ - unit_tile->row_);
                const int col_diff = std::abs(selected_tile.col_ - unit_tile->col_);

                if ((row_diff == 1 and col_diff == 0)
                    or (row_diff == 0 and col_diff == 1)) {
                    valid_move = true;
                }

                if (valid_move) {
                    player_1_->selected_unit_->sprite.setPosition(selected_tile.sprite.getPosition());
                    selected_tile.unit_ = player_1_->selected_unit_;
                    player_1_->selected_unit_->selected_ = false;
                    player_1_->selected_unit_ = nullptr;
                    unit_tile->unit_ = nullptr;
                } else {
                    std::cout << "Invalid move\n";
                }
            }
        } else {
            // Select a unit
			crosshair.setPosition(selected_tile.unit_->sprite.getPosition());
            selected_tile.unit_->selected_ = true;
            player_1_->selected_unit_ = selected_tile.unit_;
        }

    }
}
