#include "unit.hpp"

namespace ld {

Unit::Unit(const sf::Texture &texture, ld::UnitFaction unit_faction,
           ld::UnitType unit_type)
    : sprite(texture), unit_type_(unit_type), unit_faction_(unit_faction), selected_(false) {}

void Unit::render(sf::RenderWindow &window) const { window.draw(sprite); }

void Unit::set_strength(int strength) { strength_ = strength; }

int Unit::get_strength(int strength) const { return strength_; }

// -------------------------------------------
// Factory functions for building unit types
// -------------------------------------------

std::shared_ptr<ld::Unit>
Unit::build_armored_skeleton(const ld::Resources &resources) {
    std::shared_ptr<ld::Unit> unit = std::make_shared<ld::Unit>(
        resources.get_texture("skeleton_armored_right.png"),
        ld::UnitFaction::Skeleton, ld::UnitType::Armored);

    unit->set_strength(10);

    return unit;
}
}
