#pragma once

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

namespace sfx
{

class Graph : public sf::Drawable, public sf::Transformable
{
public:
    Graph(float minX = -10, float maxX = 10, float stepX = 1,
          float minY = -10, float maxY = 10, float stepY = 1,
          bool lines = true,
          sf::Color fgcolor = sf::Color::White, sf::Color bgcolor = sf::Color::Black);

    virtual ~Graph() = default;

    void setForeground(sf::Color c);
    void setBackground(sf::Color c);

    template<typename Func>
    bool render(Func&& func)
    {
        std::vector<sf::Vertex> vertices;
        vertices.reserve(foreground.getVertexCount());

        for (auto x = minX; x <= maxX; x += stepX)
        {
            auto y = func(x);
            vertices.emplace_back(sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) }, fgcolor);
        }

        return foreground.update(vertices.data());
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexBuffer background;
    sf::VertexBuffer foreground;
    sf::Color fgcolor;
    float minX;
    float maxX;
    float stepX;
};

}

