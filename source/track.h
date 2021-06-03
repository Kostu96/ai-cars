#pragma once
#include "common.h"

#include <box2d/box2d.h>

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

    float length, angle, prevangle;
    b2Vec2 pos = { -7.f, -9.f };
    b2Vec2 vec = { 0, 0 };
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

    prevangle = angle;
    angle = 30.f;
    clength = cosf(angle * DEG_TO_RAD) * length;
    slength = sinf(angle * DEG_TO_RAD) * length;
    pos += { -slength / 2.f + cosf(prevangle * DEG_TO_RAD) * length - 0.1f, length / 2.f + clength / 2.f };
    addWall(world, length, pos, 110.f);
    pos += { -length, 0.f };
    addWall(world, length, pos, 110.f);

    prevangle = angle;
    angle = -30.f;
    clength = cosf(angle * DEG_TO_RAD) * length;
    slength = sinf(angle * DEG_TO_RAD) * length;
    pos += { -slength / 2.f + cosf(prevangle * DEG_TO_RAD) * length - 0.5f, length / 2.f + clength / 2.f };
    addWall(world, length, pos, -110.f);
    pos += { -length, 0.f };
    addWall(world, length, pos, -110.f);

    vec = { (-slength / 2.f) - 0.36f, 0.1f + length / 2.f + clength / 2.f };
    addWall(world, 4.f, pos + vec, 90.f);

    vec += {length, 0};
    pos += { -length / 2.f, length / 2.f };
    addWall(world, length, pos + vec, 0.f);
    vec += {length, 0};
    addWall(world, length, pos + vec, 0.f);
    vec += {0, -length};
    addWall(world, length, pos + vec, 0.f);

    pos += { 3*length + 0.45f, length / 2.f + 0.85f};
    addWall(world, length, pos, 150.f); 
    pos += { 0, -length};
    addWall(world, length, pos, 150.f);

    pos += { cosf(angle* DEG_TO_RAD)* length + 0.3f, -1.f};
    addWall(world, length, pos, 0.f);
    pos += { 0, length};
    addWall(world, length, pos, 0.f);

    pos += { length - 0.35f, -1.f};
    addWall(world, length, pos, 150.f);
    pos += { 0, -length};
    addWall(world, length, pos, 150.f);

    pos += { -cosf(150.f * DEG_TO_RAD) * length + 0.2f, -sinf(150.f* DEG_TO_RAD) * length / 2.0f};
    addWall(world, length, pos, 0.f);
    pos += { 0, length};
    addWall(world, length, pos, 0.f);
    pos += { length, 0};
    addWall(world, length, pos, 0.f);

    pos += { length/2.f + 0.1f, -length / 2.f};
    addWall(world, length, pos, 90.f);
    pos += { 0, -length};
    addWall(world, length, pos, 90.f);
    pos += { -length, 0};
    addWall(world, length, pos, 90.f); 

    pos += { length, -length};
    addWall(world, length, pos, 90.f);
    pos += { -length/2.0f, -length/2.0f};
    addWall(world, length, pos, 0.f);
    pos += { -length, 0};
    addWall(world, length, pos, 0.f);
    pos += { 0, length};
    addWall(world, length, pos, 0.f);
    pos += { -length, 0};
    addWall(world, length, pos, 0.f);
    pos += { -length/2.0f, -length / 2.0f};
    addWall(world, length, pos, 90.f);
    pos += { 0, -length};
    addWall(world, length, pos, 90.f);
    pos += { length, 0};
    addWall(world, length, pos, 90.f);

	/*b2ChainShape shape;
	b2Vec2 vs[2] = {
		{ 0.f, 0.f },
		{ 4.f, 0.f }
	};*/
}
