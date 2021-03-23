#pragma once
#include "wheel.h"

#include <vector>

class Car
{
public:
    Car(b2World *world);
    ~Car();

    void update();
private:
    b2Body *m_body = nullptr;
    std::vector<Wheel*> m_wheels;
    b2RevoluteJoint *m_flJoint, *m_frJoint;
};
