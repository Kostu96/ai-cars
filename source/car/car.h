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

	static int stoppedCars;
	static std::vector<double ***>carsWeightsSorted;

    void update();
	void stop();
	void reset();
	bool isStopped();
	void randomizeNetworkWeights();
	void setNetworkWeights(double ***weights);
	void crossover(double ***p1_weights, double ***p2weights);
	double ***getNetworkWeights();
	void mutate();
    int speed, rotation;
    double rotationAngle;
    double min = 1000.0;
    double ret = 0;
	
	
private:
	

    b2Body* m_body = nullptr;
    b2World* world;
    std::vector<Wheel*> m_wheels;
    b2RevoluteJoint *m_flJoint, *m_frJoint;
    std::vector<double> sensors[8];
	Neural_network *steering_network;
	int neurons_count[2] = { 7,1 };
	bool stopped = false;
	
};
