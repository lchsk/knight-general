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
