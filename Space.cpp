#include "Space.hpp"

#include <cmath>
#include <vector>

namespace sfx
{
namespace graph
{

Space::Space(float minX, float maxX, float tickStepX,
             float minY, float maxY, float tickStepY,
             float plotStepX, float plotStepY,
             sf::Color axesColor)
    : Space({ minX, minY, maxX - minX, maxY - minY }, { tickStepX, tickStepY }, { plotStepX, plotStepY }, axesColor)
{}

Space::Space(sf::FloatRect area, sf::Vector2f tickStep, sf::Vector2f plotStep, sf::Color axesColor)
    : axes{ sf::PrimitiveType::Lines, sf::VertexBuffer::Usage::Static },
      area{ area },
      step{ plotStep }
{
    // 2 verts per axis, plus 2 verts per tick (step) mark
    std::vector<sf::Vertex> bgVerts;
    bgVerts.reserve(2 * (2 + static_cast<size_t>(std::ceil((area.width + 1) / tickStep.x))
        + static_cast<size_t>(std::ceil((area.height + 1) / tickStep.y))));

    if (!axes.create(bgVerts.capacity()))
        return;

    float stepHeightX = std::max(tickStep.y / 2.f, 0.5f);
    float stepHeightY = std::max(tickStep.x / 2.f, 0.5f);

    // x-axis
    bgVerts.emplace_back(sf::Vector2f{ area.left, 0.f }, axesColor);
    bgVerts.emplace_back(sf::Vector2f{ area.left + area.width, 0.f }, axesColor);

    for (float x = area.left; x <= area.left + area.width; x += tickStep.x)
    {
        bgVerts.emplace_back(sf::Vector2f{ x, -stepHeightX }, axesColor);
        bgVerts.emplace_back(sf::Vector2f{ x, stepHeightX }, axesColor);
    }

    // y-axis
    bgVerts.emplace_back(sf::Vector2f{ 0.f, area.top }, axesColor);
    bgVerts.emplace_back(sf::Vector2f{ 0.f, area.top + area.height }, axesColor);

    for (float y = area.top; y <= area.top + area.height; y += tickStep.y)
    {
        bgVerts.emplace_back(sf::Vector2f{ -stepHeightY, y }, axesColor);
        bgVerts.emplace_back(sf::Vector2f{ stepHeightY, y }, axesColor);
    }

    axes.update(bgVerts.data());
}

Function& Space::render(Function::Signature callable, sf::Color funcColor, bool line)
{
    funcs.emplace_back(callable, funcColor, line);
    funcs.back().update(area, step);
    return funcs.back();
}

void Space::remove(const Function& f)
{
    funcs.remove(f);
}

void Space::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    // flip the y-axis so the functions are plotted in a user-expected manner
    states.transform.scale(1.f, -1.f);

    target.draw(axes, states);
    for (auto& f : funcs)
        target.draw(f, states);
}

}
}

