#ifndef UNIT_H
#define UNIT_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "resources.hpp"
#include "config.hpp"

namespace ld {

enum class UnitType {
    Armored,
};

// enum class UnitFaction {
    // Skeleton,
// };

class Unit {
  public:
    Unit(const sf::Texture &texture, ld::Faction unit_faction,
         ld::UnitType unit_type);

    void render(sf::RenderWindow &window) const;

    void set_strength(int strength);
    int get_strength(int strength) const;

    sf::Sprite sprite;
    bool selected_;

    static std::shared_ptr<ld::Unit>
    build_armored_skeleton(const ld::Resources &resources);

  private:
    const ld::Faction unit_faction_;
    const ld::UnitType unit_type_;

    int strength_;

};
}

#endif /* UNIT_H */
