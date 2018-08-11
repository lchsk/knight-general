#include "unit.hpp"

namespace ld {

Unit::Unit(const sf::Texture &texture, ld::UnitFaction unit_faction,
           ld::UnitType unit_type)
    : sprite(texture), unit_type_(unit_type), unit_faction_(unit_faction) {}

void Unit::render(sf::RenderWindow &window) const { window.draw(sprite); }
}
