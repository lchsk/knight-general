#include "map.hpp"
#include "config.hpp"

namespace ld {

Map::Map(const ld::MapDefinition &map_definition,
         const std::shared_ptr<ld::Resources> &resources)
    : gui_(resources),
      player_1_(std::make_shared<ld::Player>(
          ld::PlayerType::Human, ld::Faction::Knight, ld::TileType::Grass)),
      player_2_(std::make_shared<ld::Player>(
          ld::PlayerType::AI, ld::Faction::Skeleton, ld::TileType::Earth)),
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

    add_new_unit(player_1_, ld::UnitType::Warrior);
    add_new_unit(player_2_, ld::UnitType::Warrior);

    add_new_unit(player_1_, ld::UnitType::Armored);
    add_new_unit(player_2_, ld::UnitType::Armored);

    add_new_unit(player_1_, ld::UnitType::Special);
    add_new_unit(player_2_, ld::UnitType::Special);

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

void Map::reset_human_selection() {
    for (auto &unit : units) {
        unit->selected_ = false;
    }

    player_1_->selected_unit_ = nullptr;
}

void Map::switch_players() {
    // Reset
    for (auto &unit : units) {
        unit->already_moved_ = false;
        unit->selected_ = false;
    }
    gui_.panel_unit_name.visible_ = false;
    gui_.panel_unit_faction.visible_ = false;
    gui_.panel_unit_strength.visible_ = false;

    active_player_ = is_human_active() ? player_2_ : player_1_;

    land_payout();

    const int random = ld::randint(3);

    if (random == 0)
        add_game_resource();

    update_gui();

    if (player_1_->tiles_ == 0) {
        show_message("You lost!");
    }

    if (player_2_->tiles_ == 0) {
        show_message("You won!");
    }
}

void Map::update_gui() {
    gui_.update(player_1_, player_2_, active_player_->is_human());
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

    int tries = 0;

    while (true) {
        assert(tiles.size() > 0);

        const int id = ld::randint(tiles.size() - 1);

        auto &tile = tiles[id];

        if (tile.unit_)
            continue;

        if (tile.get_type() != ld::TileType::Water and !tile.game_resource_) {
            auto game_resource = ld::GameResource::build(
                *resources, game_resource_type, filename);
            tile.game_resource_ = game_resource;
            game_resource->sprite_.setPosition(tile.sprite.getPosition());

            break;
        }

        tries++;

        if (tries >= 100)
            break;
    }
}

void Map::update_active_player_tiles() {
    int tiles_owned = 0;
    int all_tiles = 0;

    for (const auto &tile : tiles) {
        if (tile.get_type() == active_player_->tile_type_) {
            tiles_owned++;
        }

        if (tile.get_type() != ld::TileType::Water) {
            all_tiles++;
        }
    }

    active_player_->tiles_ = tiles_owned;
    active_player_->all_tiles_ = all_tiles;
}

void Map::land_payout() {

    update_active_player_tiles();
    const int new_coins = std::ceil(active_player_->tiles_ / 4);
    active_player_->coins_ += new_coins;

    if (active_player_->is_human()) {
        show_message("Received " + std::to_string(new_coins) +
                     " coins for land");
    }
}

void Map::play_ai() {

    for (auto &unit : units) {
        if (unit->get_faction() == player_1_->faction_) {
            // Skip Human units
            continue;
        }

        if (ld::randint(3) == 0) {
            // Skip unit's turn to make it easier for human player
            continue;
        }

        ld::Tile *unit_tile = find_unit_tile(unit);

        if (!unit_tile) {
            continue;
        }

        std::string texture_name;

        for (const auto &tile_config : TILES) {
            if (tile_config.get_type() == player_2_->tile_type_) {
                texture_name = tile_config.get_filename();
                break;
            }
        }

        bool moved = false;

        // Collect a resource or attack

        for (auto &tile : tiles) {
            if (is_valid_move(tile, unit_tile)) {
                if (tile.game_resource_) {
                    const int payout =
                        tile.game_resource_->get_resource_payout();
                    player_2_->coins_ += payout;
                    move_enemy_unit(unit, tile, unit_tile, texture_name);
                    moved = true;
                    break;
                } else if (unit->can_fight(tile.unit_)) {
                    show_message("Your unit is fighting");
                    unit->fight(tile.unit_);

                    moved = true;
                    break;
                }
            }
        }

        if (moved) {
            continue;
        }

        // Find optimal move

        std::vector<ld::Tile *> target_tiles;

        for (auto &tile : tiles) {
            if (tile.game_resource_ or
                (tile.unit_ and
                 tile.unit_->get_faction() != player_2_->faction_) or
                (tile.get_type() != ld::TileType::Water and
                 tile.get_type() != player_2_->tile_type_)) {
                target_tiles.push_back(&tile);
            }
        }

        ld::Tile *closest_tile = nullptr;
        int shortest_dist = 999999;

        for (const auto tile : target_tiles) {
            int dist = ld::map_coords::calc_distance(*tile, unit_tile);

            if (dist < shortest_dist) {
                shortest_dist = dist;
                closest_tile = tile;
            }
        }

        if (closest_tile) {
            for (auto &tile : tiles) {
                if (is_valid_move(tile, unit_tile)) {
                    int dist =
                        ld::map_coords::calc_distance(tile, closest_tile);

                    if (dist < shortest_dist) {
                        move_enemy_unit(unit, tile, unit_tile, texture_name);
                        break;
                    }
                }
            }
        }
    }

    clean_up_units();

    const int random = ld::randint(3);

    add_new_unit(player_2_, ld::UnitType::Special);

    if (random == 0) {
        add_new_unit(player_2_, ld::UnitType::Armored);
    } else if (random == 1) {
        add_new_unit(player_2_, ld::UnitType::Warrior);
    }
}

bool Map::is_human_active() const { return active_player_ == player_1_; }

void Map::show_message(const std::string &text) {
    gui_.panel_info.set_text(text);
    gui_.panel_info.visible_ = true;
    gui_timer_ = sf::seconds(0);
}

void Map::move_enemy_unit(const std::shared_ptr<ld::Unit> &unit, ld::Tile &tile,
                          ld::Tile *unit_tile,
                          const std::string &texture_name) {
    const auto pos = tile.sprite.getPosition();
    tile.game_resource_ = nullptr;
    tile.unit_ = unit;
    unit_tile->unit_ = nullptr;
    tile.sprite = sf::Sprite(resources->get_texture(texture_name));
    tile.sprite.setPosition(pos);
    tile.unit_->sprite.setPosition(pos);
    tile.set_type(player_2_->tile_type_);
}

void Map::end_human_turn() {
    if (is_human_active()) {
        switch_players();
        play_ai();
    }
}

void Map::handle_left_mouse_click(const sf::Vector2i &pos) {

    if (!is_human_active()) {
        return;
    }

    ld::GuiAction action = gui_.handle_button_click(pos);

    switch (action) {
    case ld::GuiAction::EndTurn:
        end_human_turn();
        return;

    case ld::GuiAction::AddWarriorUnit:
        add_new_unit(player_1_, ld::UnitType::Warrior);
        return;

    case ld::GuiAction::AddArmoredUnit:
        add_new_unit(player_1_, ld::UnitType::Armored);
        return;

    case ld::GuiAction::AddSpecialUnit:
        add_new_unit(player_1_, ld::UnitType::Special);
        return;

    case ld::GuiAction::NoAction:
        break;
    }

    const int tile_col = ld::map_coords::px2tile_col(pos.x);
    const int tile_row = ld::map_coords::px2tile_row(pos.y);

    // Tile that was just clicked on
    const int tile_id = ld::map_coords::coords_to_tile_id(tile_row, tile_col);

    if (tile_id >= ld::config::ROWS * ld::config::COLS) {
        // Mouse click outside of game window
        return;
    }

    auto &selected_tile = tiles[tile_id];

    if (selected_tile.get_type() == ld::TileType::Water) {
        reset_human_selection();
        gui_.panel_unit_name.visible_ = false;
        gui_.panel_unit_faction.visible_ = false;
        gui_.panel_unit_strength.visible_ = false;
        return;
    }

    if (player_1_->selected_unit_ and
        player_1_->selected_unit_->can_fight(selected_tile.unit_)) {
        // Fight
        ld::Tile *unit_tile = find_unit_tile(player_1_->selected_unit_);

        if (is_valid_move(selected_tile, unit_tile)) {
            show_message("Your unit is fighting");
            player_1_->selected_unit_->fight(selected_tile.unit_);
            player_1_->selected_unit_->already_moved_ = true;
            clean_up_units();
        }
    } else if (!selected_tile.unit_) {
        // There's no unit on selected tile => Move
        if (player_1_->selected_unit_) {
            // Move to a new tile
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

                // Check if there's a resource on this tile
                if (selected_tile.game_resource_) {
                    const std::string name =
                        selected_tile.game_resource_->get_string();
                    const int coins =
                        selected_tile.game_resource_->get_resource_payout();
                    show_message("Collected " + name + " for " +
                                 std::to_string(coins) + " coins");
                    player_1_->coins_ += coins;

                    selected_tile.game_resource_ = nullptr;
                }

                selected_tile.sprite =
                    sf::Sprite(resources->get_texture(texture_name));
                selected_tile.sprite.setPosition(pos);
                player_1_->selected_unit_->selected_ = false;
                player_1_->selected_unit_->already_moved_ = true;
                player_1_->selected_unit_ = nullptr;
                unit_tile->unit_ = nullptr;
                update_active_player_tiles();
                update_gui();
            } else {
                show_message("Invalid move");
            }
        }
    } else {
        // Show unit info box
        if (selected_tile.unit_) {
            gui_.panel_unit_name.visible_ = true;
            gui_.panel_unit_name.set_text(selected_tile.unit_->get_unit_name());
            gui_.panel_unit_faction.visible_ = true;
            gui_.panel_unit_faction.set_text(
                selected_tile.unit_->get_unit_faction());
            gui_.panel_unit_strength.visible_ = true;
            gui_.panel_unit_strength.set_text(
                "Strength: " +
                std::to_string(selected_tile.unit_->get_strength()));
        }

        // Select a unit
        if (!selected_tile.unit_->already_moved_ and
            selected_tile.unit_->get_faction() == player_1_->faction_) {
            if (player_1_->selected_unit_) {
                player_1_->selected_unit_->selected_ = false;
            }
            crosshair.setPosition(selected_tile.unit_->sprite.getPosition());
            selected_tile.unit_->selected_ = true;
            player_1_->selected_unit_ = selected_tile.unit_;
        }
    }
}

bool Map::is_valid_move(const ld::Tile &selected_tile,
                        const ld::Tile *unit_tile) const {

    if (selected_tile.unit_ and selected_tile.unit_->already_moved_)
        return false;

    const bool neighbor_tiles =
        ld::map_coords::neighbor_tiles(selected_tile, unit_tile);

    return neighbor_tiles and
           selected_tile.get_type() != ld::TileType::Water and
           (!selected_tile.unit_ or (selected_tile.unit_ and
                                     selected_tile.unit_->get_faction() !=
                                         unit_tile->unit_->get_faction()));
}

void Map::add_new_unit(std::shared_ptr<ld::Player> &player,
                       ld::UnitType unit_type) {
    std::unordered_map<ld::UnitType, int> unit_costs = {
        {ld::UnitType::Warrior, 80},
        {ld::UnitType::Armored, 120},
        {ld::UnitType::Special, 180},
    };

    const int cost = unit_costs[unit_type];
    const int coins = player->coins_;

    if (cost > coins) {
        if (player->is_human()) {
            show_message("Cannot afford this unit");
        }
        return;
    }

    bool free_tile = check_free_tile_available(true, false);

    if (!free_tile) {
        std::cout << "No space to add this unit!\n";
        return;
    }

    int tries = 0;

    // Select a random tile
    while (true) {
        assert(tiles.size() > 0);

        const int id = ld::randint(tiles.size() - 1);

        auto &tile = tiles[id];

        if (tile.game_resource_)
            continue;

        // Add a unit on the random tile
        if (tile.get_type() == player->tile_type_ and tile.unit_ == nullptr) {
            auto unit =
                ld::Unit::build_unit(*resources, player->faction_, unit_type);
            units.push_back(unit);
            tile.unit_ = unit;
            unit->sprite.setPosition(tile.sprite.getPosition());
            player->coins_ -= cost;
            update_active_player_tiles();
            update_gui();

            break;
        }

        tries++;

        if (tries >= 100)
            break;
    }
}

void Map::update(sf::RenderWindow &window, const sf::Time &delta) {
    gui_.update(window, delta);
    gui_timer_ += delta;

    if (gui_.panel_info.visible_ and gui_timer_.asSeconds() >= 2) {
        gui_.panel_info.visible_ = false;
        gui_timer_ = sf::seconds(0);
    }

    if (active_player_->player_type_ == ld::PlayerType::AI) {
        ai_timer_ += delta;

        if (ai_timer_ > sf::seconds(ld::randint(3, 1))) {
            ai_timer_ = sf::seconds(0);
            switch_players();
        }
    }
}
}
