#include <cassert>
#include <iostream>

#include "unit.hpp"

namespace ld {

Unit::Unit(const sf::Texture &texture, ld::Faction unit_faction,
           ld::UnitType unit_type)
    : sprite(texture), selected_(false), already_moved_(false),
      unit_faction_(unit_faction), unit_type_(unit_type) {}

void Unit::render(sf::RenderWindow &window) const { window.draw(sprite); }

void Unit::set_strength(int strength) { strength_ = strength; }

void Unit::update_strength(int strength_diff) { strength_ += strength_diff; }

int Unit::get_strength() const { return strength_; }

ld::UnitType Unit::get_type() const { return unit_type_; }

ld::Faction Unit::get_faction() const { return unit_faction_; }

bool Unit::can_fight(const std::shared_ptr<ld::Unit> &other_unit) const {
    if (already_moved_)
        return false;

    return (other_unit and this != other_unit.get() and
            get_faction() != other_unit->get_faction());
}

void Unit::fight(std::shared_ptr<ld::Unit> &unit) {
    const int random_factor = ld::randint(4, 2);
    const int lucky_player = ld::randint(1);

    if (lucky_player) {
        update_strength(random_factor);
    } else {
        unit->update_strength(random_factor);
    }

    const int my_strength = get_strength();
    const int enemy_strength = unit->get_strength();

    if (enemy_strength > my_strength) {
        unit->set_strength(random_factor - (my_strength / 2)); // winner
        update_strength(-(enemy_strength / 2));                // loser
    } else if (my_strength > enemy_strength) {
        update_strength(random_factor - (enemy_strength / 2)); // winner
        unit->update_strength(-(my_strength / 2));             // loser
    } else {
        // tie
        update_strength(get_strength() / 2);
        unit->set_strength(enemy_strength / 2);
    }

    std::cout << "Result of a fight: " << strength_ << " : "
              << unit->get_strength() << std::endl;
}

const std::string Unit::get_unit_name() const {
    switch (get_type()) {
    case ld::UnitType::Warrior:
        return "Warrior";
    case ld::UnitType::Armored:
        return "Armored warrior";
    case ld::UnitType::Special:
        if (get_faction() == ld::Faction::Skeleton) {
            return "Wizard";
        } else if (get_faction() == ld::Faction::Knight) {
            return "Knight";
        }
    }

    return "";
}

const std::string Unit::get_unit_faction() const {
    if (get_faction() == ld::Faction::Skeleton) {
        return "Skeletons";
    } else if (get_faction() == ld::Faction::Knight) {
        return "Knights";
    }

    return "";
}

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
        {ld::UnitType::Warrior, "warrior"},
        {ld::UnitType::Armored, "armored"},
        {ld::UnitType::Special, "special"},
    };

    std::unordered_map<ld::UnitType, int> unit_strengths = {
        {ld::UnitType::Warrior, 10},
        {ld::UnitType::Armored, 15},
        {ld::UnitType::Special, 25},
    };

    const std::string filename =
        faction_names[faction] + "_" + faction_units[unit_type] + "_right.png";

    std::shared_ptr<ld::Unit> unit = std::make_shared<ld::Unit>(
        resources.get_texture(filename), faction, unit_type);

    unit->set_strength(unit_strengths[unit_type]);

    return unit;
}
}
