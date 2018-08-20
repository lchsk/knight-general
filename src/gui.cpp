#include "gui.hpp"

#include <iostream>

#if SFML_VERSION_MINOR >= 4
#define SFML_SET_COLOR setFillColor
#else
#define SFML_SET_COLOR setColor
#endif

namespace ld {

Gui::Gui(const std::shared_ptr<ld::Resources> &resources)
    : resources_(resources), panel_turn(resources, "button_brown.png", false),
      panel_tiles(resources, "button_brown.png", false),
      panel_coins(resources, "button_brown.png", false),
      panel_info(resources, "button_brown.png", false),
      panel_unit_name(resources, "button_brown.png", false),
      panel_unit_faction(resources, "button_brown.png", false),
      panel_unit_strength(resources, "button_brown.png", false),
      button_turn(resources, "button_brown_pressed.png", true),
      button_warrior(resources, "button_brown_pressed.png", true),
      button_armored(resources, "button_brown_pressed.png", true),
      button_special(resources, "button_brown_pressed.png", true),
      cursor(resources->get_texture("cursor.png")) {
    set_positions();

    button_turn.set_text("End turn (Ret)");

    panel_info.visible_ = false;
    panel_info.label_.setCharacterSize(14);

    panel_unit_name.visible_ = false;
    panel_unit_faction.visible_ = false;
    panel_unit_strength.visible_ = false;

    button_warrior.set_text("Warrior");
    button_armored.set_text("Armored warrior");
    button_special.set_text("Special unit");
}

void Gui::render(sf::RenderWindow &window) const {

    panel_turn.render(window);
    panel_tiles.render(window);
    panel_coins.render(window);
    panel_info.render(window);
    panel_unit_name.render(window);
    panel_unit_faction.render(window);
    panel_unit_strength.render(window);
    button_turn.render(window);

    button_warrior.render(window);
    button_armored.render(window);
    button_special.render(window);

    window.draw(cursor);
}

void Gui::update(sf::RenderWindow &window, const sf::Time &delta) {
    const sf::Vector2i pos = sf::Mouse::getPosition(window);
    cursor.setPosition(pos.x, pos.y);
}

void Gui::update(std::shared_ptr<ld::Player> player_1,
                 std::shared_ptr<ld::Player> player_2, bool human_active) {

    const std::string label_turn = (human_active ? "Your turn" : "AI turn");

    button_turn.visible_ = human_active;

    panel_coins.visible_ = human_active;
    panel_coins.set_text(std::to_string(player_1->coins_) + " coins");

    panel_tiles.visible_ = human_active;
    panel_tiles.set_text("Land: " + std::to_string(player_1->tiles_) + "/" +
                         std::to_string(player_1->all_tiles_));

    panel_turn.set_text(label_turn);
}

void Gui::set_positions() {
    // BEWARE, lots of hardcoded stuff :[

    const int margin = 8;

    panel_turn.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_turn.sprite_.getLocalBounds().width - margin,
        margin);

    button_turn.sprite_.setPosition(
        ld::config::get_screen_width() -
            button_turn.sprite_.getLocalBounds().width - margin,
        2 * margin + button_turn.sprite_.getLocalBounds().height);

    panel_coins.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_coins.sprite_.getLocalBounds().width - margin,
        5 * margin + panel_coins.sprite_.getLocalBounds().height * 2);

    panel_info.sprite_.setPosition(
        ld::config::get_screen_width() / 2 -
            panel_info.sprite_.getLocalBounds().width / 2,
        margin);

    panel_tiles.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_tiles.sprite_.getLocalBounds().width - margin,
        6 * margin + panel_tiles.sprite_.getLocalBounds().height * 3);

    button_warrior.sprite_.setPosition(
        ld::config::get_screen_width() -
            button_warrior.sprite_.getLocalBounds().width - margin,
        8 * margin + button_warrior.sprite_.getLocalBounds().height * 4);

    button_armored.sprite_.setPosition(
        ld::config::get_screen_width() -
            button_armored.sprite_.getLocalBounds().width - margin,
        9 * margin + button_armored.sprite_.getLocalBounds().height * 5);

    button_special.sprite_.setPosition(
        ld::config::get_screen_width() -
            button_special.sprite_.getLocalBounds().width - margin,
        10 * margin + button_special.sprite_.getLocalBounds().height * 6);

    panel_unit_name.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_unit_name.sprite_.getLocalBounds().width - margin,
        11 * margin + panel_unit_name.sprite_.getLocalBounds().height * 8);

    panel_unit_faction.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_unit_faction.sprite_.getLocalBounds().width - margin,
        12 * margin + panel_unit_faction.sprite_.getLocalBounds().height * 9);

    panel_unit_strength.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_unit_strength.sprite_.getLocalBounds().width - margin,
        13 * margin + panel_unit_strength.sprite_.getLocalBounds().height * 10);
}

ld::GuiAction Gui::handle_button_click(const sf::Vector2i &pos) {
    if (button_turn.check_click(pos)) {
        return ld::GuiAction::EndTurn;
    } else if (button_warrior.check_click(pos)) {
        return ld::GuiAction::AddWarriorUnit;
    } else if (button_armored.check_click(pos)) {
        return ld::GuiAction::AddArmoredUnit;
    } else if (button_special.check_click(pos)) {
        return ld::GuiAction::AddSpecialUnit;
    }

    return ld::GuiAction::NoAction;
}

Button::Button(const std::shared_ptr<ld::Resources> &resources,
               const std::string &resource_name, bool enabled)
    : enabled_(enabled), visible_(true), resources_(resources) {

    sprite_ = sf::Sprite(resources_->get_texture(resource_name));
    label_.setFont(resources->basic_font_);
    label_.setCharacterSize(20);
    label_.SFML_SET_COLOR(sf::Color::White);
}

bool Button::check_click(const sf::Vector2i &pos) const {
    if (!enabled_)
        return false;

    const auto &bounds_sprite = sprite_.getGlobalBounds();

    return (pos.x >= bounds_sprite.left and
            pos.x <= bounds_sprite.left + bounds_sprite.width and
            pos.y >= bounds_sprite.top and
            pos.y <= bounds_sprite.top + bounds_sprite.height);
}

void Button::set_text(const std::string &text) {
    label_.setString(text);

    const auto &pos = sprite_.getPosition();
    const auto &bounds_sprite = sprite_.getLocalBounds();
    const auto &bounds_label = label_.getLocalBounds();

    label_.setPosition(pos.x + bounds_sprite.width / 2 - bounds_label.width / 2,
                       pos.y + bounds_sprite.height / 2 -
                           bounds_label.height / 2 - 4);
}

void Button::render(sf::RenderWindow &window) const {
    if (visible_) {
        window.draw(sprite_);
        window.draw(label_);
    }
}
}
