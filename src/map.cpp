#include "map.hpp"
#include "config.hpp"

namespace ld {

Map::Map(const ld::MapDefinition &map_definition,
         const std::shared_ptr<ld::Resources> &resources)
    : player_1_(std::make_shared<ld::Player>(
          ld::PlayerType::Human, ld::Faction::Skeleton, ld::TileType::Earth)),
      player_2_(std::make_shared<ld::Player>(
          ld::PlayerType::AI, ld::Faction::Knight, ld::TileType::Grass)),
      resources(resources) {

    const sf::Texture &texture_crosshair =
        resources->get_texture("crosshair.png");
    crosshair = sf::Sprite(texture_crosshair);

    for (unsigned row = 0; row < ld::config::ROWS; row++) {
        for (unsigned col = 0; col < ld::config::COLS; col++) {
            unsigned tile_id = map_definition[row][col];
            const ld::TileConfig &tile_config = TILES[tile_id];

            try {
                const sf::Texture &texture =
                    resources->get_texture(tile_config.get_filename());
                tiles.push_back(
                    ld::Tile(texture, tile_config.get_type(), row, col));
            } catch (const std::out_of_range &) {
                std::cout << "Filename " << tile_config.get_filename()
                          << " not found" << std::endl;
            }
        }
    }

    add_new_unit(player_1_, ld::UnitType::Armored);
    add_new_unit(player_2_, ld::UnitType::Armored);
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

void Map::handle_left_mouse_click(const sf::Vector2i &pos) {
    const int tile_col = ld::map_coords::px2tile_col(pos.x);
    const int tile_row = ld::map_coords::px2tile_row(pos.y);

    // Tile that was just clicked on
    auto &selected_tile =
        tiles[ld::map_coords::coords_to_tile_id(tile_row, tile_col)];

    if (selected_tile.unit_ == nullptr) {
        // There's no unit on selected tile => Move
        if (player_1_->selected_unit_) {
            ld::Tile *unit_tile = nullptr;

            // Check if can move this unit
            for (auto &tile : tiles) {
                if (tile.unit_ == player_1_->selected_unit_) {
                    unit_tile = &tile;
                    break;
                }
            }

            if (unit_tile == nullptr) {
                std::cout << "Could not find selected unit's tile\n";
                return;
            }

            if (is_valid_move(selected_tile, unit_tile)) {
                player_1_->selected_unit_->sprite.setPosition(
                    selected_tile.sprite.getPosition());
                selected_tile.unit_ = player_1_->selected_unit_;
                selected_tile.set_type(player_1_->tile_type_);
                const auto pos = selected_tile.sprite.getPosition();

                std::string texture_name;

                for (const auto &tile_config : TILES) {
                    if (tile_config.get_type() == player_1_->tile_type_) {
                        texture_name = tile_config.get_filename();
                        break;
                    }
                }

                selected_tile.sprite =
                    sf::Sprite(resources->get_texture(texture_name));
                selected_tile.sprite.setPosition(pos);
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

bool Map::is_valid_move(const ld::Tile &selected_tile,
                        const ld::Tile *unit_tile) const {
    const bool neighbor_tiles =
        ld::map_coords::neighbor_tiles(selected_tile, unit_tile);

    return neighbor_tiles and selected_tile.get_type() != ld::TileType::Water;
}

void Map::add_new_unit(std::shared_ptr<ld::Player> &player,
                       ld::UnitType unit_type) {
    // Check there's free space to add a unit
    bool no_space = true;

    for (const auto &tile : tiles) {
        if (tile.unit_ == nullptr) {
            no_space = false;
        }
    }

    // Select a random tile
    while (true) {
        assert(tiles.size() > 0);

        const int id = ld::randint(tiles.size() - 1);

        auto &tile = tiles[id];

        // Add a unit on the random tile
        if (tile.get_type() == player->tile_type_ and tile.unit_ == nullptr) {
            auto unit =
                ld::Unit::build_unit(*resources, player->faction_, unit_type);
            units.push_back(unit);
            tile.unit_ = unit;
            unit->sprite.setPosition(tile.sprite.getPosition());

            break;
        }
    }
}
}
