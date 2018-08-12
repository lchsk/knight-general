#ifndef UNIT_H
#define UNIT_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "resources.hpp"

namespace ld {

enum class UnitType {
    Armored,
};

class Unit {
  public:
    Unit(const sf::Texture &texture, ld::Faction unit_faction,
         ld::UnitType unit_type);

    void render(sf::RenderWindow &window) const;

    void set_strength(int strength);
    void update_strength(int strength_diff);
    int get_strength() const;

    ld::UnitType get_type() const;
    ld::Faction get_faction() const;

    bool can_fight(const std::shared_ptr<ld::Unit> &other_unit) const;

    void fight(std::shared_ptr<ld::Unit> &unit);

    sf::Sprite sprite;
    bool selected_;

    static std::shared_ptr<ld::Unit> build_unit(const ld::Resources &resources,
                                                ld::Faction faction,
                                                ld::UnitType unit_type);

  private:
    const ld::Faction unit_faction_;
    const ld::UnitType unit_type_;

    int strength_;
};
}

#endif /* UNIT_H */
