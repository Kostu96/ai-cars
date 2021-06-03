#include "car.h"
#include "../../third_party/SFML/src/SFML/Graphics/GLLoader.hpp"

Car::Car(b2World* world)
{
<<<<<<< HEAD
	b2BodyDef bodyDef;
	bodyDef.position = { -7.f, -7.f };
	bodyDef.type = b2_dynamicBody;
	bodyDef.enabled = true;
	m_body = world->CreateBody(&bodyDef);
	this->world = world;

    b2BodyDef bodyDef;
    bodyDef.position = { -7.f, -7.f };
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
	b2Vec2 angle = this->m_body->GetPosition();
	for (auto wheel : m_wheels)
	{
		wheel->updateDrive(rotation, speed);
	}

	for (int i = 0; i < 8; i++)
	{
		b2RayCastInput input;
		input.p1 = m_body->GetPosition();
		if (i < 4) {
			input.p2 = m_wheels[i]->getBody()->GetPosition();
		}	
		if (i == 4) {
			b2Vec2 tmp = m_wheels[0]->getBody()->GetPosition();
			tmp += m_wheels[1]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		if (i == 5) {
			b2Vec2 tmp = m_wheels[2]->getBody()->GetPosition();
			tmp += m_wheels[3]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		if (i == 6) {
			b2Vec2 tmp = m_wheels[1]->getBody()->GetPosition();
			tmp += m_wheels[2]->getBody()->GetPosition();
			tmp *= 0.5;
			input.p2 = tmp;
		}
		if (i == 7) {
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
			sensors[0].push_back(closestFraction);
			break;
		case 1:
			sensors[1].push_back(closestFraction);
			break;
		case 2:
			sensors[2].push_back(closestFraction);
			break;
		case 3:
			sensors[3].push_back(closestFraction);
			break;
		case 4:
			sensors[4].push_back(closestFraction);
			break;
		case 5:
			sensors[5].push_back(closestFraction);
			break;
		case 6:
			sensors[6].push_back(closestFraction);
			break;
		case 7:
			sensors[7].push_back(closestFraction);
			break;

		}
		/*
		if(i < 4)printf("Kolo %i = %f\n", i, closestFraction);
		else if (i == 4)printf("Przod = %f\n", closestFraction);
		else if (i == 5)printf("Tyl = %f\n", closestFraction);
		else if (i == 6)printf("Bok lewy = %f\n", closestFraction);
		else if (i == 7)printf("Bok prawy = %f\n", closestFraction);
		if ((closestFraction) <= min) min = (closestFraction);
		*/
	}
	//printf("min = %f\n", this->min);

}
