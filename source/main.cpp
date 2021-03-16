#include "application.h"

// Track is build out of segments.
// Every segment is a row in this table
// and consists of:
// - width of the segment in meters
// - length of the segment in meters
// - turn angle of the segment in degrees
//#define NUM_TRACK_SEGMENTS 5
//float track[NUM_TRACK_SEGMENTS][3] = {
//    { 3.f, 4.f, 0.f},
//    { 3.f, 4.f, 15.f},
//    { 3.f, 4.f, -20.f},
//    { 2.f, 3.5f, -25.f},
//    { 3.f, 3.f, -20.f }
//};

//void createTrack(b2World& world) {
//    b2BodyDef bodyDef;
//    bodyDef.position = { 0.f, -6.f };
//    bodyDef.type = b2_staticBody;
//    b2Body* body;
//    b2PolygonShape shape;
//
//    body = world.CreateBody(&bodyDef);
//    bodyDef.position.y = track[0][1] / 2.f;
//    shape.SetAsBox(2.f, 0.05f);
//    body->CreateFixture(&shape, 0.f);
//    for (unsigned int i = 0; i < 3; ++i) {
//        float width = track[i][0];
//        float length = track[i][1];
//        float angle = (-90.f + track[i][2]) * DEG_TO_RAD;
//
//        shape.SetAsBox(length / 2.f, 0.05f);
//        bodyDef.angle = angle;
//
//        bodyDef.position = { -length / 2.f, bodyDef.position.y + length };
//        body = world.CreateBody(&bodyDef);
//        body->CreateFixture(&shape, 0.f);
//
//        bodyDef.position = { length / 2.f, bodyDef.position.y };
//        body = world.CreateBody(&bodyDef);
//        body->CreateFixture(&shape, 0.f);
//    }
//}

int main()
{
    Application app;
    app.run();
    return 0;
}
