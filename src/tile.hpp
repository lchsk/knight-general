#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "game_resource.hpp"
#include "unit.hpp"

namespace ld {
class Tile {
  public:
    Tile(const sf::Texture &texture, ld::TileType type, int, int);

    ld::TileType get_type() const;
    void set_type(ld::TileType type);

    sf::Sprite sprite;

    std::shared_ptr<ld::Unit> unit_;
    std::shared_ptr<ld::GameResource> game_resource_;

    int row_, col_;

  private:
    ld::TileType type_;
};
}

#endif /* TILE_H */
