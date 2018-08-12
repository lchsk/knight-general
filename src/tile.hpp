#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "unit.hpp"

namespace ld {
class Tile {
  public:
    Tile(const sf::Texture &texture, ld::TileType type, int, int);

    sf::Sprite sprite;

    std::shared_ptr<ld::Unit> unit_;

    int row_, col_;

    const ld::TileType type_;

  private:
};
}

#endif /* TILE_H */
