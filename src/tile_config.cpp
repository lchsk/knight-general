#include "tile_config.hpp"

namespace ld {
TileConfig::TileConfig(const std::string &filename, ld::TileType type)
    : filename_(filename), type_(type) {}

const std::string &TileConfig::get_filename() const { return filename_; }
}
