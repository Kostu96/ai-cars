#pragma once
#include <box2d/b2_draw.h>
#include <SFML/Graphics.hpp>

class DebugDraw : public b2Draw
{
public:
    DebugDraw(sf::RenderWindow& window) : m_window(window) {}

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
        DrawSolidPolygon(vertices, vertexCount, color);
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
        auto vCenter = m_window.getDefaultView().getCenter();
        sf::ConvexShape polygon;
        polygon.setPointCount(vertexCount);
        for (int32 i = 0; i < vertexCount; ++i) {
            b2Vec2 vertex = vertices[i];
            polygon.setPoint(i, { vertex.x * RENDER_SCALE + vCenter.x, vertex.y * RENDER_SCALE + vCenter.y });
        }
        polygon.setFillColor(B2SFColor(color));
        m_window.draw(polygon);
    }

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override {
        auto vCenter = m_window.getDefaultView().getCenter();
        sf::CircleShape circle(radius);
        circle.setFillColor(B2SFColor(color));
        circle.setPosition(center.x + vCenter.x, center.y + vCenter.y);
        m_window.draw(circle);
    }

    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override {
        
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {
        auto vCenter = m_window.getDefaultView().getCenter();
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x * RENDER_SCALE + vCenter.x, p1.y * RENDER_SCALE + vCenter.y)),
            sf::Vertex(sf::Vector2f(p2.y * RENDER_SCALE + vCenter.x, p2.y * RENDER_SCALE + vCenter.y))
        };
        m_window.draw(line, 2, sf::Lines);
    }

    void DrawTransform(const b2Transform& xf) override {

    }

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override {

    }
private:
    sf::Color DebugDraw::B2SFColor(const b2Color& color, int alpha = 255) {
        return { (sf::Uint8)(color.r * 255), (sf::Uint8)(color.g * 255), (sf::Uint8)(color.b * 255), (sf::Uint8)alpha };
    }

    sf::RenderWindow& m_window;
};
