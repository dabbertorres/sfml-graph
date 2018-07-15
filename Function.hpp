#pragma once

#include <functional>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

namespace sfx
{
namespace graph
{

class Function : public sf::Drawable
{
public:
    using Signature = std::function<float(float)>;

    Function(Signature func, sf::Color color, bool line);
    virtual ~Function() = default;

    bool operator==(const Function& other) const;
    bool operator!=(const Function& other) const;

    void setColor(sf::Color color);
    void setLine(bool line);
    bool update(sf::FloatRect area, sf::Vector2f step);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Signature function;
    sf::Color color;
    sf::VertexBuffer buffer;
};

}
}

