#ifndef GUI_H
#define GUI_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "resources.hpp"

namespace ld {

class Button {
  public:
    Button(const sf::Texture &texture, bool enabled);

    void render(sf::RenderWindow &window) const;

    bool enabled_;
    bool visible_;

    sf::Sprite sprite_;
};

class Gui {
  public:
    Gui(const std::shared_ptr<ld::Resources> &resources);
    void render(sf::RenderWindow &window) const;
    void update(const sf::Time &delta);

    const std::shared_ptr<ld::Resources> resources_;
    ld::Button panel_turn;

  private:
    void set_positions();
};
}

#endif /* GUI_H */
