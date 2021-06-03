#pragma once
#include <box2d/box2d.h>

class Wheel
{
public:
    /*enum class Control : unsigned int {
        Left  = 1 << 0,
        Right = 1 << 1,
        Up    = 1 << 2,
        Down  = 1 << 3
    };*/

    Wheel(b2World* world, b2Vec2 pos);
    ~Wheel();

    b2Body* getBody() { return m_body; }

    void updateFriction();
    void updateDrive(int rotation, int speed);
private:
    b2Vec2 getLateralVelocity() const;
    b2Vec2 getForwardVelocity() const;

    b2Body* m_body = nullptr;

    float m_maxForwardSpeed = 60.f;
    float m_maxBackwardSpeed = -20.f;
    float m_maxDriveForce = 25.f;
    float m_maxLateralImpulse = 15.f;
};
