#include "Graph.hpp"

#include <cmath>
#include <vector>

namespace sfx
{

Graph::Graph(float minX, float maxX, float stepX,
             float minY, float maxY, float stepY,
             bool lines, sf::Color fgcolor, sf::Color bgcolor)
    : background{ sf::PrimitiveType::Lines, sf::VertexBuffer::Usage::Static },
      foreground{ lines ? sf::PrimitiveType::LineStrip : sf::PrimitiveType::Points, sf::VertexBuffer::Usage::Dynamic },
      fgcolor{ fgcolor },
      minX{ minX },
      maxX{ maxX },
      stepX{ stepX }
{
    // 2 verts per axis, plus 2 verts per tick (step) mark
    std::vector<sf::Vertex> bgVerts;
    bgVerts.reserve(4 + static_cast<size_t>(std::ceil((maxX - minX + 1) / stepX)) * 2
        + static_cast<size_t>(std::ceil((maxY - minY + 1) / stepY)) * 2);

    if (!background.create(bgVerts.capacity()))
        return;

    if (!foreground.create(static_cast<size_t>(std::ceil(maxX - minX + 1) / stepX)))
        return;

    float stepHeightX = std::max(stepY / 2.f, 0.5f);
    float stepHeightY = std::max(stepX / 2.f, 0.5f);

    // x-axis
    bgVerts.emplace_back(sf::Vector2f{ minX, 0.f }, bgcolor);
    bgVerts.emplace_back(sf::Vector2f{ maxX, 0.f }, bgcolor);

    for (float x = minX; x <= maxX; x += stepX)
    {
        bgVerts.emplace_back(sf::Vector2f{ x, -stepHeightX }, bgcolor);
        bgVerts.emplace_back(sf::Vector2f{ x, stepHeightX }, bgcolor);
    }

    // y-axis
    bgVerts.emplace_back(sf::Vector2f{ 0.f, minY }, bgcolor);
    bgVerts.emplace_back(sf::Vector2f{ 0.f, maxY }, bgcolor);

    for (float y = minY; y <= maxY; y += stepY)
    {
        bgVerts.emplace_back(sf::Vector2f{ -stepHeightY, y }, bgcolor);
        bgVerts.emplace_back(sf::Vector2f{ stepHeightY, y }, bgcolor);
    }

    background.update(bgVerts.data());
}

void Graph::setForeground(sf::Color c)
{
    fgcolor = c;
}

void Graph::setBackground(sf::Color c)
{
    // TODO update background (refactor axis creation into a helper function)
}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(background, states);
    target.draw(foreground, states);
}

}

