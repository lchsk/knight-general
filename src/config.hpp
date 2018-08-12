#ifndef CONFIG_H
#define CONFIG_H

namespace ld {
namespace config {
const int TILE_SIZE = 64;
const int ROWS = 10;
const int COLS = 13;

constexpr int get_screen_width() noexcept { return TILE_SIZE * COLS; }

constexpr int get_screen_height() noexcept { return TILE_SIZE * ROWS; }
}

// TODO: move this out
enum class Faction {
    Skeleton,
};

enum class TileType {
    Grass,
    Earth,
    Water,
};
}

#endif /* CONFIG_H */
