#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <box2d/b2_world.h>
#include <memory>

#include "helpers/debug_draw.h"

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
};
