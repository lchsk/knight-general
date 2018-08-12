#include "tile.hpp"
#include "config.hpp"

namespace ld {

Tile::Tile(const sf::Texture &texture, ld::TileType type, int row, int col)
    : sprite(texture), unit_(nullptr), type_(type), row_(row), col_(col) {
    sprite.setPosition(col * ld::config::TILE_SIZE,
                       row * ld::config::TILE_SIZE);
}

ld::TileType Tile::get_type() const { return type_; }

void Tile::set_type(ld::TileType type) { type_ = type; }
}
