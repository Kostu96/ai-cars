#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <memory>

#include "common.h"
#include "debug_draw.h"

// Track is build out of segments.
// Every segment is a row in this table
// and consists of:
// - width of the segment in meters
// - length of the segment in meters
// - turn angle of the segment in degrees
#define NUM_TRACK_SEGMENTS 5
float track[NUM_TRACK_SEGMENTS][3] = {
    { 3.f, 4.f, 0.f},
    { 3.f, 4.f, 15.f},
    { 3.f, 4.f, -20.f},
    { 2.f, 3.5f, -25.f},
    { 3.f, 3.f, -20.f }
};

void createTrack(b2World& world) {
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(-2.f, 0.f);
    bodyDef.type = b2_staticBody;
    b2Body* body = world.CreateBody(&bodyDef);
    
    b2PolygonShape shape;
    shape.SetAsBox(3.f, 0.1f, { 2.f, -7.05f }, 0.f);
    body->CreateFixture(&shape, 0.f);

    //shape.SetAsBox(0.1f, 4.f, { -1.f, -3.1f }, 0.f);
    //body->CreateFixture(&shape, 0.f);
    //shape.SetAsBox(0.1f, 4.f, { 5.f, -3.1f }, 0.f);
    //body->CreateFixture(&shape, 0.f);
    //shape.SetAsBox(0.1f, 4.f, { -2.f, 4.7f }, 15.f * DEG_TO_RAD);
    //body->CreateFixture(&shape, 0.f);
    //shape.SetAsBox(0.1f, 4.f, { 4.f, 4.7f }, 15.f * DEG_TO_RAD);
    //body->CreateFixture(&shape, 0.f);

    for (unsigned int i = 0; i < NUM_TRACK_SEGMENTS; ++i) {

    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "AI Cars");
    window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 0.f);
    auto world = std::make_unique<b2World>(gravity);
    DebugDraw dbgDraw(window);
    dbgDraw.SetFlags(DebugDraw::e_shapeBit);
    world->SetDebugDraw(&dbgDraw);

    createTrack(*world.get());

    while (window.isOpen())
    {
        // Events
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        // Update
        world->Step(1 / 60.f, 10, 8);

        // Render
        window.clear();
        world->DebugDraw();
        window.display();
    }

    return 0;
}
