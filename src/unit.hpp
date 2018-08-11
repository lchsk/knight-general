#ifndef UNIT_H
#define UNIT_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "resources.hpp"

namespace ld {

enum class UnitType {
    Armored,
};

enum class UnitFaction {
    Skeleton,
};

class Unit {
  public:
    Unit(const sf::Texture &texture, ld::UnitFaction unit_faction,
         ld::UnitType unit_type);

    void render(sf::RenderWindow &window) const;

    sf::Sprite sprite;

    static std::shared_ptr<ld::Unit>
    build_armored_skeleton(const ld::Resources &resources) {
        std::shared_ptr<ld::Unit> unit = std::make_shared<ld::Unit>(
            resources.get_texture("skeleton_armored_right.png"),
            ld::UnitFaction::Skeleton, ld::UnitType::Armored);

        return unit;
    }

  private:
    const ld::UnitFaction unit_faction_;
    const ld::UnitType unit_type_;
};
}

#endif /* UNIT_H */
