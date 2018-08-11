#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

#include <string>

namespace ld {
class TileConfig {
  public:
    TileConfig(const std::string &filename);

    const std::string &get_filename() const;

  private:
    const std::string filename_;
};
}

#endif /* TILE_CONFIG_H */
