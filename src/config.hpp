#ifndef CONFIG_H
#define CONFIG_H

#include <random>

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

/* Return random int [min, max] */
inline int randint(int max, int min = 0) {
    // Move initialisation somewhere else
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<int> uni(min, max);

    return uni(rng);
}
}

#endif /* CONFIG_H */
