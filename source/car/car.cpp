#include "car.h"

Car::Car(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.position = { 8.f, 2.f };
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2Vec2 vertices[6] = {
        {  0.6f, -1.1f },
        {  0.6f,  0.8f },
        {  0.3f,  1.2f },
        { -0.3f,  1.2f },
        { -0.6f,  0.8f },
        { -0.6f, -1.1f }
    };
    b2PolygonShape polygonShape;
    polygonShape.Set(vertices, 6);
    b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 0.1f);

    Wheel* wheel = nullptr;
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.localAnchorB = { 0.f, 0.f };

    wheel = new Wheel(world, bodyDef.position + b2Vec2(-0.45f, 0.75f));
    jointDef.bodyB = wheel->getBody();
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorA = m_body->GetLocalPoint(wheel->getBody()->GetWorldCenter());
    m_flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(wheel);

    wheel = new Wheel(world, bodyDef.position + b2Vec2(0.45f, 0.75f));
    jointDef.bodyB = wheel->getBody();
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorA = m_body->GetLocalPoint(wheel->getBody()->GetWorldCenter());
    m_frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
    m_wheels.push_back(wheel);

    wheel = new Wheel(world, bodyDef.position + b2Vec2(0.45f, -0.7f));
    jointDef.bodyB = wheel->getBody();
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorA = m_body->GetLocalPoint(wheel->getBody()->GetWorldCenter());
    world->CreateJoint(&jointDef);
    m_wheels.push_back(wheel);

    wheel = new Wheel(world, bodyDef.position + b2Vec2(-0.45f, -0.7f));
    jointDef.bodyB = wheel->getBody();
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorA = m_body->GetLocalPoint(wheel->getBody()->GetWorldCenter());
    world->CreateJoint(&jointDef);
    m_wheels.push_back(wheel);
}

Car::~Car()
{
    for (auto wheel : m_wheels)
        delete wheel;

    m_body->GetWorld()->DestroyBody(m_body);
}

void Car::update()
{
}
