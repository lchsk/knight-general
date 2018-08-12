#ifndef PLAYER_H
#define PLAYER_H

#include "unit.hpp"
#include "config.hpp"

namespace ld {

enum class PlayerType {
    Human,
    AI,
};

class Player {
  public:
    Player(ld::PlayerType player_type, ld::TileType tile_type);

    std::shared_ptr<ld::Unit> selected_unit_;

    const ld::PlayerType player_type_;
    const ld::TileType tile_type_;
};
}

#endif /* PLAYER_H */
