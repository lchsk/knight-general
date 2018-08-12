#include "map.hpp"
#include "config.hpp"

namespace ld {

Map::Map(const ld::MapDefinition &map_definition,
         const std::shared_ptr<ld::Resources> &resources)
    : gui_(resources),
      player_1_(std::make_shared<ld::Player>(
          ld::PlayerType::Human, ld::Faction::Skeleton, ld::TileType::Earth)),
      player_2_(std::make_shared<ld::Player>(
          ld::PlayerType::AI, ld::Faction::Knight, ld::TileType::Grass)),
      active_player_(player_2_), resources(resources) {

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

    switch_players();
}

ld::Tile *Map::find_unit_tile(const std::shared_ptr<ld::Unit> &unit) {
    for (auto &tile : tiles) {
        if (tile.unit_ == unit) {
            return &tile;
        }
    }
    return nullptr;
}

void Map::render(sf::RenderWindow &window) const {
    for (const auto &tile : tiles) {
        window.draw(tile.sprite);

        if (tile.game_resource_) {
            window.draw(tile.game_resource_->sprite_);
        }
    }

    for (const auto &unit : units) {
        window.draw(unit->sprite);

        if (unit->selected_) {
            window.draw(crosshair);
        }
    }

    gui_.render(window);
}

void Map::clean_up_units() {
    for (auto &tile : tiles) {
        if (tile.unit_ and tile.unit_->get_strength() <= 0) {
            tile.unit_->selected_ = false;
            tile.unit_ = nullptr;
        }
    }

    units.erase(std::remove_if(units.begin(), units.end(),
                               [&](const auto &unit) {
                                   return unit->get_strength() <= 0;
                               }),
                units.end());
}

void Map::switch_players() {
    // Unselect
    for (auto &unit : units) {
        unit->selected_ = false;
    }

    active_player_ = is_human_active() ? player_2_ : player_1_;

    const int random = ld::randint(3);

    land_payout();

    if (random == 0)
        add_game_resource();

    gui_.update(player_1_, player_2_,
                active_player_->player_type_ == ld::PlayerType::Human);
}

bool Map::check_free_tile_available(bool check_for_units,
                                    bool check_for_resources) const {

    if (check_for_units and check_for_resources) {
        throw std::logic_error("Only one of parameters can be true");
    }

    // Check there's free space to add a unit
    bool free_tile = false;

    for (const auto &tile : tiles) {
        if (tile.get_type() != ld::TileType::Water and !tile.unit_) {
            free_tile = true;
            break;
        }
    }

    return free_tile;
}

void Map::add_game_resource() {

    bool free_tile = check_free_tile_available(false, true);

    if (!free_tile) {
        std::cout << "No space to add game resource!\n";
        return;
    }

    auto resource_filenames =
        std::unordered_map<ld::GameResourceType, std::string>{
            {ld::GameResourceType::Gold, "gold"},
            {ld::GameResourceType::Timber, "timber"},
            {ld::GameResourceType::Tree_1, "tree_1"},
            {ld::GameResourceType::Tree_2, "tree_2"},
            {ld::GameResourceType::Stone_1, "stone_1"},
            {ld::GameResourceType::Stone_2, "stone_2"},
            {ld::GameResourceType::Stone_3, "stone_3"},
            {ld::GameResourceType::Stone_4, "stone_4"},
        };

    std::vector<ld::GameResourceType> types(resource_filenames.size());

    std::transform(resource_filenames.begin(), resource_filenames.end(),
                   types.begin(), [](const auto pair) { return pair.first; });

    const int random_id = ld::randint(resource_filenames.size() - 1);

    ld::GameResourceType game_resource_type = types[random_id];
    std::string filename = resource_filenames[game_resource_type] + ".png";

    while (true) {
        assert(tiles.size() > 0);

        const int id = ld::randint(tiles.size() - 1);

        auto &tile = tiles[id];

        if (tile.get_type() != ld::TileType::Water and !tile.game_resource_) {
            auto game_resource = ld::GameResource::build(
                *resources, game_resource_type, filename);
            tile.game_resource_ = game_resource;
            game_resource->sprite_.setPosition(tile.sprite.getPosition());

            break;
        }
    }
}

void Map::land_payout() const {
    int payout = 0;

    int all_tiles = 0;

    for (const auto &tile : tiles) {
        if (tile.get_type() == active_player_->tile_type_) {
            payout++;
        }

        if (tile.get_type() != ld::TileType::Water) {
            all_tiles++;
        }
    }

    active_player_->coins_ += payout;
    active_player_->tiles_ = payout;
    active_player_->all_tiles_ = all_tiles;
}

void Map::play_ai() {}

bool Map::is_human_active() const { return active_player_ == player_1_; }

void Map::handle_left_mouse_click(const sf::Vector2i &pos) {

    if (!is_human_active()) {
        return;
    }

    ld::GuiAction action = gui_.handle_button_click(pos);

    if (action == ld::GuiAction::EndTurn) {
        switch_players();
        play_ai();
        return;
    }

    const int tile_col = ld::map_coords::px2tile_col(pos.x);
    const int tile_row = ld::map_coords::px2tile_row(pos.y);

    // Tile that was just clicked on
    auto &selected_tile =
        tiles[ld::map_coords::coords_to_tile_id(tile_row, tile_col)];

    if (player_1_->selected_unit_ and
        player_1_->selected_unit_->can_fight(selected_tile.unit_)) {
        ld::Tile *unit_tile = find_unit_tile(player_1_->selected_unit_);

        if (is_valid_move(selected_tile, unit_tile)) {
            player_1_->selected_unit_->fight(selected_tile.unit_);
            clean_up_units();
        }
    } else if (!selected_tile.unit_) {
        // There's no unit on selected tile => Move
        if (player_1_->selected_unit_) {
            ld::Tile *unit_tile = find_unit_tile(player_1_->selected_unit_);

            if (!unit_tile) {
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
        if (selected_tile.unit_->get_faction() == player_1_->faction_) {
            crosshair.setPosition(selected_tile.unit_->sprite.getPosition());
            selected_tile.unit_->selected_ = true;
            player_1_->selected_unit_ = selected_tile.unit_;
        }
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

    bool free_tile = check_free_tile_available(true, false);

    if (!free_tile) {
        std::cout << "No space to add this unit!\n";
        return;
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

void Map::update(const sf::Time &delta) {
    if (active_player_->player_type_ == ld::PlayerType::AI) {
        ai_timer_ += delta;

        if (ai_timer_ > sf::seconds(ld::randint(3, 1))) {
            ai_timer_ = sf::seconds(0);
            switch_players();
        }
    }
}
}
