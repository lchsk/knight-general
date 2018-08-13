#include <cassert>
#include <iostream>

#include "game_resource.hpp"

namespace ld {

GameResource::GameResource(const sf::Texture &texture,
                           ld::GameResourceType game_resource_type)
    : sprite_(texture), game_resource_type_(game_resource_type) {}

void GameResource::render(sf::RenderWindow &window) const {
    window.draw(sprite_);
}

const std::string GameResource::get_string() const {
    switch (game_resource_type_) {
    case GameResourceType::Gold:
        return "gold";
    case GameResourceType::Timber:
        return "timber";
    case GameResourceType::Tree_1:
        return "tree";
    case GameResourceType::Tree_2:
        return "tree";
    case GameResourceType::Stone_1:
        return "stone";
    case GameResourceType::Stone_2:
        return "stone";
    case GameResourceType::Stone_3:
        return "stone";
    case GameResourceType::Stone_4:
        return "stone";
    }

    return "";
}

int GameResource::get_resource_payout() const {
    switch (game_resource_type_) {
    case GameResourceType::Gold:
        return 25;
    case GameResourceType::Timber:
        return 10;
    case GameResourceType::Tree_1:
        return 12;
    case GameResourceType::Tree_2:
        return 13;
    case GameResourceType::Stone_1:
        return 5;
    case GameResourceType::Stone_2:
        return 8;
    case GameResourceType::Stone_3:
        return 12;
    case GameResourceType::Stone_4:
        return 15;
    }

    return 0;
}

// -------------------------------------------

// Factory functions for building resources
// -------------------------------------------

std::shared_ptr<ld::GameResource>
GameResource::build(const ld::Resources &resources,
                    ld::GameResourceType game_resource_type,
                    const std::string &filename) {

    std::shared_ptr<ld::GameResource> resource =
        std::make_shared<ld::GameResource>(resources.get_texture(filename),
                                           game_resource_type);

    return resource;
}
}
