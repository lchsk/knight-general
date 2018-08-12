#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

#include <string>

#include "config.hpp"

namespace ld {
class TileConfig {
  public:
    TileConfig(const std::string &filename, ld::TileType type);

    const std::string &get_filename() const;
    const ld::TileType get_type() const;

  private:
    const std::string filename_;
    const ld::TileType type_;
};
}

#endif /* TILE_CONFIG_H */
