#include "wheel.h"

Wheel::Wheel(b2World* world, b2Vec2 pos)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.08f, 0.3f);
	m_body->CreateFixture(&polygonShape, 1);

	//m_body->SetUserData(this);
}

Wheel::~Wheel()
{
	m_body->GetWorld()->DestroyBody(m_body);
}

void Wheel::updateFriction()
{
	b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
	if (impulse.Length() > m_maxLateralImpulse)
		impulse *= m_maxLateralImpulse / impulse.Length();
	m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);
	m_body->ApplyAngularImpulse(0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	m_body->ApplyForce(dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter(), true);
}

void Wheel::updateDrive(int rotation, int speed)
{
	switch (rotation)
	{
	case -1:
		m_body->ApplyTorque(-m_maxLateralImpulse, true);
		break;
	case 1:
		m_body->ApplyTorque(m_maxLateralImpulse, true);
		break;
	default:
		break;
	}

	switch (speed)
	{
	case -1:
		m_body->ApplyForce(-m_maxDriveForce * m_body->GetWorldVector(b2Vec2(0, 1)), m_body->GetWorldCenter(), true);
		break;
	case 1:
		m_body->ApplyForce(m_maxDriveForce * m_body->GetWorldVector(b2Vec2(0, 1)), m_body->GetWorldCenter(), true);
		break;
	default:
		break;
	}

	this->updateFriction();
}

b2Vec2 Wheel::getLateralVelocity() const
{
	b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 Wheel::getForwardVelocity() const
{
	b2Vec2 currentUpNormal = m_body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentUpNormal, m_body->GetLinearVelocity()) * currentUpNormal;
}
