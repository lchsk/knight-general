#include "tile.hpp"

namespace ld {

Tile::Tile(const sf::Texture &texture, const unsigned row, const unsigned col)
    : sprite(texture) {
    sprite.setPosition(row * 64, col * 64);
}
}
