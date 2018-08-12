#include "gui.hpp"

namespace ld {

Gui::Gui(const std::shared_ptr<ld::Resources> &resources)
    : resources_(resources), panel_turn(resources, "button_brown.png", false),
      button_turn(resources, "button_brown_pressed.png", true) {
    set_positions();

    button_turn.set_text("End turn");
}

void Gui::render(sf::RenderWindow &window) const {

    panel_turn.render(window);
    button_turn.render(window);
}

void Gui::update(std::shared_ptr<ld::Player> player_1,
                 std::shared_ptr<ld::Player> player_2, bool human_active) {

    const std::string label_turn = (human_active ? "Your turn" : "AI turn");

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
            panel_turn.sprite_.getLocalBounds().width - margin,
        2 * margin + panel_turn.sprite_.getLocalBounds().height);
}

Button::Button(const std::shared_ptr<ld::Resources> &resources,
               const std::string &resource_name, bool enabled)
    : enabled_(enabled), visible_(true), resources_(resources) {

    sprite_ = sf::Sprite(resources_->get_texture(resource_name));
    label_.setFont(resources->basic_font_);
    label_.setCharacterSize(20);
    label_.setFillColor(sf::Color::White);
}

void Button::set_text(const std::string &text) {
    label_.setString(text);

    const auto &pos = sprite_.getPosition();
    const auto &bounds_sprite = sprite_.getLocalBounds();
    const auto &bounds_label = label_.getLocalBounds();

    label_.setPosition(pos.x + bounds_sprite.width / 2 - bounds_label.width / 2,
                       pos.y + bounds_label.height / 2);
}

void Button::render(sf::RenderWindow &window) const {
    if (visible_) {
        window.draw(sprite_);
        window.draw(label_);
    }
}
}
