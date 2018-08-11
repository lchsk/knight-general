#include "tile.hpp"
#include "config.hpp"

namespace ld {

Tile::Tile(const sf::Texture &texture, const unsigned row, const unsigned col)
    : sprite(texture), unit_(nullptr) {
    sprite.setPosition(row * ld::config::TILE_SIZE, col * ld::config::TILE_SIZE);
}
}
