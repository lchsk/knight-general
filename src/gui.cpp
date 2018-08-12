#include "gui.hpp"

namespace ld {

Gui::Gui(const std::shared_ptr<ld::Resources> &resources)
    : resources_(resources), panel_turn(resources, true) {
    set_positions();
}

void Gui::render(sf::RenderWindow &window) const { panel_turn.render(window); }

void Gui::update(std::shared_ptr<ld::Player> player_1,
                 std::shared_ptr<ld::Player> player_2, bool human_active) {

    const std::string label_turn = (human_active ? "Human turn" : "AI turn");

    panel_turn.set_text(label_turn);
}

void Gui::set_positions() {
    // BEWARE, lots of hardcoded stuff :[

    const int margin = 8;

    panel_turn.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_turn.sprite_.getLocalBounds().width - margin,
        margin);
}

Button::Button(const std::shared_ptr<ld::Resources> &resources, bool enabled)
    : enabled_(enabled), visible_(true), resources_(resources) {

    sprite_ = sf::Sprite(resources_->get_texture("button_brown.png"));
    label_.setFont(resources->basic_font_);
    label_.setCharacterSize(20);
    label_.setFillColor(sf::Color::White);
}

void Button::set_text(const std::string &text) {
    label_.setString(text);

    const auto &bounds = label_.getLocalBounds();
    label_.setPosition(sprite_.getPosition().x + bounds.width / 2,
                       sprite_.getPosition().y + bounds.height / 2);
}

void Button::render(sf::RenderWindow &window) const {
    if (visible_) {
        window.draw(sprite_);
        window.draw(label_);
    }
}
}
