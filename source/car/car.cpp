#include "car.h"
#include "windows.h"
#include "../neural_network.h"
#include "../../third_party/SFML/src/SFML/Graphics/GLLoader.hpp"

Car::Car(b2World* world)
{
	b2BodyDef bodyDef;
	bodyDef.position = { -7.f, -7.f };
	bodyDef.type = b2_dynamicBody;
	bodyDef.enabled = true;
	m_body = world->CreateBody(&bodyDef);
	this->world = world;

	b2Vec2 vertices[6] = {
		{  0.6f, -1.1f },
		{  0.6f,  0.8f },
		{  0.3f,  1.2f },
		{ -0.3f,  1.2f },
		{ -0.6f,  0.8f },
		{ -0.6f, -1.1f }
	};
	b2PolygonShape polygonShape;
	b2Filter filterDef;
	filterDef.categoryBits = 0x1;
	filterDef.maskBits = 0x2;
	filterDef.groupIndex = 0x0;
	polygonShape.Set(vertices, 6);
	b2Fixture* fixture = m_body->CreateFixture(&polygonShape, 0.1f);
	fixture->SetFilterData(filterDef);


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
	
	steering_network = new Neural_network(8, 2, neurons_count);
}

Car::~Car()
{
	for (auto wheel : m_wheels)
		delete wheel;

	m_body->GetWorld()->DestroyBody(m_body);
}

void Car::update()
{
	b2Vec2 angle = this->m_body->GetPosition();
	for (auto wheel : m_wheels)
	{
		wheel->updateDrive(rotation, speed);
	}

	for (int i = 0; i < 8; i++)
	{
		b2RayCastInput input;
		input.p1 = m_body->GetPosition();
		if (i < 4) { //kola
			input.p2 = m_wheels[i]->getBody()->GetPosition();
		}	
		if (i == 4) {//przod
			b2Vec2 tmp = m_wheels[0]->getBody()->GetPosition();
			tmp += m_wheels[1]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		if (i == 5) {//tyl
			b2Vec2 tmp = m_wheels[2]->getBody()->GetPosition();
			tmp += m_wheels[3]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		if (i == 6) {//lewy bok
			b2Vec2 tmp = m_wheels[1]->getBody()->GetPosition();
			tmp += m_wheels[2]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		if (i == 7) {// prawy bok
			b2Vec2 tmp = m_wheels[3]->getBody()->GetPosition();
			tmp += m_wheels[0]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		input.maxFraction = 30;

		double closestFraction = 30.0; //start with end of line as p2
		b2Vec2 intersectionNormal(0, 0);
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
				b2Body* body = f->GetBody();
				if (body->GetType() == b2_staticBody) {
					b2RayCastOutput output;
					if (!f->RayCast(&output, input, 1))
						continue;
					if (output.fraction < closestFraction) {
						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
		}
		closestFraction -= 1.19;
		switch (i)
		{
		case 0:
			sensors[0].push_back(closestFraction-0.36f);
			break;
		case 1:
			sensors[1].push_back(closestFraction-0.15f);
			break;
		case 2:
			sensors[2].push_back(closestFraction);
			break;
		case 3:
			sensors[3].push_back(closestFraction);
			break;
		case 4:
			sensors[4].push_back(closestFraction-0.43f);
			break;
		case 5:
			sensors[5].push_back(closestFraction-0.1f);
			break;
		case 6:
			sensors[6].push_back(closestFraction-0.18f);
			break;
		case 7:
			sensors[7].push_back(closestFraction-0.18f);
			break;

		}

		/*
		if(i < 4)printf("Kolo %i = %f\n", i, sensors[i].back());
		else if (i == 4)printf("Przod = %f\n", sensors[i].back());
		else if (i == 5)printf("Tyl = %f\n", sensors[i].back());
		else if (i == 6)printf("Bok lewy = %f\n", sensors[i].back());
		else if (i == 7)printf("Bok prawy = %f\n", sensors[i].back());
		*/
		if ((sensors[i].back()) <= min) min = (sensors[i].back());
	}
	//printf("min = %f\n", this->min);
	if (min < 0.1f)
	{
		b2Vec2 tmp = m_body->GetPosition();
		printf("koniec\n");
		printf("%f\n",ret);
		m_body->SetTransform(-m_body->GetPosition(), 0);
		m_body->SetTransform({ -7.f, -7.f }, 0);
		for (int i = 0; i < 4; i++)
		{
			m_wheels[i]->getBody()->SetTransform(-m_body->GetPosition(), 0);
			m_wheels[i]->getBody()->SetTransform({ -7.f, -7.f }, 0);
		}
		min = 1000.f;
		ret = 0.f;
	}

	double sensorsInput[8];
	for (int i = 0; i < 8; i++)
	{
		sensorsInput[i] = sensors[i].back();
	}

	steering_network->setInputs(sensorsInput);
	steering_network->setRandomWeights();
	steering_network->activate_network();
	double* network_outputs;
	network_outputs = steering_network->getOutputs();

	if (network_outputs[0] < (0.5))
	{
		rotation = -1;
	}
	/*else if (network_outputs[0] < (0.6))
	{
		rotation = 0;
	}*/
	else
	{
		rotation = 1;
	}

	if (network_outputs[1] < (0.5))
	{
		speed = -1;
	}
	/*else if (network_outputs[1] < (0.6))
	{
		speed = 0;
	}
	else*/
	{
		speed = 1;
		ret += 0.1f;
	}
}
