#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "tile.hpp"

void update(sf::RenderWindow &window, const sf::Time &delta, ld::Map &map) {
    map.update(window, delta);
}

void render(sf::RenderWindow &window, const ld::Map &map) {
    window.clear();
    map.render(window);
    window.display();
}

void handle_events(sf::RenderWindow &window, ld::Map &map) {
    sf::Event event;

    while (window.pollEvent(event)) {
        const sf::Vector2i pos = sf::Mouse::getPosition(window);

        if (event.type == sf::Event::Closed)
            window.close();
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            map.handle_left_mouse_click(pos);
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else if (event.key.code == sf::Keyboard::Return) {
                map.end_human_turn();
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(ld::config::get_screen_width(),
                                          ld::config::get_screen_height()),
                            "knights");

    window.setMouseCursorVisible(false);

    sf::Time per_frame = sf::seconds(1.0f / 60.0f);
    sf::Clock clock;
    sf::Time last_update;

    std::shared_ptr<ld::Resources> resources =
        std::make_shared<ld::Resources>();
    resources->load();

    ld::Map map(ld::map_1, resources);

    while (window.isOpen()) {
        sf::Time delta = clock.getElapsedTime() - last_update;

        if (delta > per_frame) {
            handle_events(window, map);
            update(window, delta, map);
            render(window, map);
            last_update = clock.getElapsedTime();
        }
    }

    return 0;
}
