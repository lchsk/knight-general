#include "unit.hpp"

namespace ld {

Unit::Unit(const sf::Texture &texture, ld::Faction unit_faction,
           ld::UnitType unit_type)
    : sprite(texture), unit_type_(unit_type), unit_faction_(unit_faction),
      selected_(false) {}

void Unit::render(sf::RenderWindow &window) const { window.draw(sprite); }

void Unit::set_strength(int strength) { strength_ = strength; }

int Unit::get_strength(int strength) const { return strength_; }

// -------------------------------------------
// Factory functions for building unit types
// -------------------------------------------

std::shared_ptr<ld::Unit> Unit::build_unit(const ld::Resources &resources,
                                           ld::Faction faction,
                                           ld::UnitType unit_type) {
    std::unordered_map<ld::Faction, std::string> faction_names = {
        {ld::Faction::Skeleton, "skeleton"}, {ld::Faction::Knight, "knight"},
    };

    std::unordered_map<ld::UnitType, std::string> faction_units = {
        {ld::UnitType::Armored, "armored"},
    };

    const std::string filename =
        faction_names[faction] + "_" + faction_units[unit_type] + "_right.png";

    std::shared_ptr<ld::Unit> unit = std::make_shared<ld::Unit>(
        resources.get_texture(filename), faction, unit_type);

    unit->set_strength(10);

    return unit;
}
}
