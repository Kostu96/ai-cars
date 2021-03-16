#include "debug_draw.h"
#include "../common.h"

#include <SFML/Graphics.hpp>

float renderScaleFactor = 30.f;

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    auto vCenter = m_window.getDefaultView().getCenter();
    sf::ConvexShape polygon;
    polygon.setPointCount(vertexCount);
    for (int32 i = 0; i < vertexCount; ++i) {
        b2Vec2 vertex = vertices[i];
        polygon.setPoint(i, { vertex.x * renderScaleFactor + vCenter.x, vertex.y * renderScaleFactor + vCenter.y });
    }
    polygon.setFillColor(B2SFColor(color));
    m_window.draw(polygon);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
    auto vCenter = m_window.getDefaultView().getCenter();
    sf::CircleShape circle(radius);
    circle.setFillColor(B2SFColor(color));
    circle.setPosition(center.x * renderScaleFactor + vCenter.x, center.y * renderScaleFactor + vCenter.y);
    m_window.draw(circle);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    auto vCenter = m_window.getDefaultView().getCenter();
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(p1.x * renderScaleFactor + vCenter.x, p1.y * renderScaleFactor + vCenter.y)),
        sf::Vertex(sf::Vector2f(p2.y * renderScaleFactor + vCenter.x, p2.y * renderScaleFactor + vCenter.y))
    };
    m_window.draw(line, 2, sf::Lines);
}
