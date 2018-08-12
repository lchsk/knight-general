#include "gui.hpp"

namespace ld {

Gui::Gui(const std::shared_ptr<ld::Resources> &resources)
    : resources_(resources),
      panel_turn(resources->get_texture("button_brown.png"), true) {
    set_positions();
}

void Gui::render(sf::RenderWindow &window) const { panel_turn.render(window); }

void Gui::update(const sf::Time &delta) {}

void Gui::set_positions() {
    // BEWARE, lots of hardcoded stuff :[

    const int margin = 8;

    panel_turn.sprite_.setPosition(
        ld::config::get_screen_width() -
            panel_turn.sprite_.getLocalBounds().width - margin,
        margin);
}

Button::Button(const sf::Texture &texture, bool enabled)
    : enabled_(enabled), visible_(true), sprite_(texture) {}

void Button::render(sf::RenderWindow &window) const {
    if (visible_) {
        window.draw(sprite_);
    }
}
}
