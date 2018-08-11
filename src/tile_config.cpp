#include "tile_config.hpp"

namespace ld {
TileConfig::TileConfig(const std::string &filename) : filename_(filename) {}

const std::string &TileConfig::get_filename() const { return filename_; }
}
