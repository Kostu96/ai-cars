#pragma once
#include <box2d/b2_draw.h>
#include <SFML/Graphics/Color.hpp>

namespace sf {
    class RenderWindow;
}

class DebugDraw : public b2Draw
{
public:
    DebugDraw(sf::RenderWindow& window) : m_window(window) {}

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

    inline void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override {
        DrawCircle(center, radius, color);
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

    inline void DrawTransform(const b2Transform& xf) override {}

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
private:
    inline sf::Color DebugDraw::B2SFColor(const b2Color& color, sf::Uint8 alpha = 255) {
        return {
            static_cast<sf::Uint8>(color.r * 255),
            static_cast<sf::Uint8>(color.g * 255),
            static_cast<sf::Uint8>(color.b * 255),
            alpha };
    }

    sf::RenderWindow& m_window;
};
