#pragma once
#include "helpers/debug_draw.h"
#include "car/car.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_world.h>
#include <memory>

class Application {
public:
    Application();
    void run();
private:
    void processEvents();
    void update(const sf::Time& dt);
    void render();

    sf::RenderWindow m_window;
    std::unique_ptr<b2World> m_world;
    DebugDraw m_dbgDrawHelper;
    std::unique_ptr<Car> m_car;
};
