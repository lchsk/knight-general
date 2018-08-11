#include "tile.hpp"
#include "config.hpp"

namespace ld {

Tile::Tile(const sf::Texture &texture, int row, int col)
    : sprite(texture), unit_(nullptr), row_(row), col_(col) {
    sprite.setPosition(col * ld::config::TILE_SIZE, row * ld::config::TILE_SIZE);
}
}
