#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

namespace ld {
class Tile {
  public:
    Tile(const sf::Texture &texture, const unsigned row, const unsigned col);

    sf::Sprite sprite;

  private:
};
}

#endif /* TILE_H */
