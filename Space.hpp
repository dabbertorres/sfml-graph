#pragma once

#include <list>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

#include "Function.hpp"

namespace sfx
{
namespace graph
{

class Space : public sf::Drawable, public sf::Transformable
{
public:
    Space(float minX = -10, float maxX = 10, float tickStepX = 1,
          float minY = -10, float maxY = 10, float tickStepY = 1,
          float plotStepX = 1, float plotStepY = 1,
          sf::Color axesColor = sf::Color::Black);
    Space(sf::FloatRect area, sf::Vector2f tickStep, sf::Vector2f plotStep, sf::Color axesColor = sf::Color::Black);

    virtual ~Space() = default;

    Function& render(Function::Signature callable, sf::Color color = sf::Color::White, bool line = true);
    Function& render(Function f);
    void remove(const Function& f);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexBuffer axes;
    std::list<Function> funcs;
    sf::FloatRect area;
    sf::Vector2f step;
};

}
}

