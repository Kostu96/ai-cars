#include "box2d//box2d.h"
#include "common.h"

void addWall(b2World& world, float length, b2Vec2 pos, float angle) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = pos;
    bodyDef.angle = angle * DEG_TO_RAD;
    auto body = world.CreateBody(&bodyDef);
    
    b2PolygonShape shape;
    shape.SetAsBox(length / 2.f, 0.05f);
    body->CreateFixture(&shape, 0.f);
}

void createTrack(b2World& world) {
    float length, angle;
    b2Vec2 pos = { 0.f, -7.f };
    addWall(world, 4.f, pos, 0.f);

    length = 4.f;
    pos += { -length / 2.f, length / 2.f };
    addWall(world, length, pos, 90.f);
    pos += { length, 0.f};
    addWall(world, length, pos, 90.f);

    angle = 20.f;
    auto clength = cosf(angle * DEG_TO_RAD) * length;
    auto slength = sinf(angle * DEG_TO_RAD) * length;
    pos += { -slength / 2.f, length / 2.f + clength / 2.f };
    addWall(world, length, pos, 110.f);
    pos += { -length, 0.f };
    addWall(world, length, pos, 110.f);

    angle = 30.f;
    clength = cosf(angle * DEG_TO_RAD) * length;
    slength = sinf(angle * DEG_TO_RAD) * length;
    pos += { -slength / 2.f, length / 2.f + clength / 2.f };
    addWall(world, length, pos, 140.f);
    pos += { -length, 0.f };
    addWall(world, length, pos, 140.f);
}
