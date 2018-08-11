#ifndef PLAYER_H
#define PLAYER_H

#include "unit.hpp"

namespace ld {

enum class PlayerType {
    Human,
    AI,
};

class Player {
  public:
    Player();

    std::shared_ptr<ld::Unit> selected_unit_;
};
}

#endif /* PLAYER_H */
