#pragma once
#include "wheel.h"
#include "../neural_network.h"

#include <memory>
#include <vector>

class Car
{
public:
    Car(b2World* world);
    ~Car();

    void update();
    int speed, rotation;
    double rotationAngle;
    //double min = 1000.0;
	
private:
    b2Body* m_body = nullptr;
    b2World* world;
    std::vector<Wheel*> m_wheels;
    b2RevoluteJoint *m_flJoint, *m_frJoint;
    std::vector<double> sensors[8];
	Neural_network *steering_network;
	int neurons_count[2] = { 5,2 };
};
