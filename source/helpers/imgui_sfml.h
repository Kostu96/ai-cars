#pragma once

namespace sf {
    class RenderWindow;
    class Event;
    class Time;
}

namespace im_sf {
    void init(sf::RenderWindow& window);
    void processEvents(const sf::Event& e);
    void update(const sf::Time& dt);
    void render();
    void shutdown();
}
