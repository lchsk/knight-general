#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "map.hpp"
#include "resources.hpp"
#include "tile.hpp"

void update(const sf::Time &delta) {}

void render(sf::RenderWindow &window, const ld::Map &map) {
    window.clear();
    map.render(window);
    window.display();
}

void handle_events(sf::RenderWindow &window) {
    sf::Event event;

    while (window.pollEvent(event)) {
        const sf::Vector2i pos = sf::Mouse::getPosition(window);

        if (event.type == sf::Event::Closed)
            window.close();
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        } else if (event.type == sf::Event::KeyPressed and
                   event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(832, 640), "knights");

    sf::Time per_frame = sf::seconds(1.0f / 60.0f);
    sf::Clock clock;
    sf::Time last_update;

    ld::Resources resources;
    resources.load();

    ld::Map map(ld::map_1, resources);

    while (window.isOpen()) {
        sf::Time delta = clock.getElapsedTime() - last_update;

        if (delta > per_frame) {
            handle_events(window);
            update(delta);
            render(window, map);
            last_update = clock.getElapsedTime();
        }
    }

    return 0;
}
