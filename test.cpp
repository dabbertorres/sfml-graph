#include <cmath>
#include <stdexcept>
#include <iostream>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "Space.hpp"

int main(int argc, char** argv)
{
    sf::RenderWindow window{{1280, 720}, "Graphing Test"};
    window.setVerticalSyncEnabled(true);

    sf::View view{{0.f, 0.f}, {20.f, 20.f}};
    window.setView(view);

    sfx::graph::Space space{ {-10.f, -10.f, 20.f, 20.f}, { 0.5f, 1.0f }, { 0.01f, 0.01f }, sf::Color::White};
    space.render([](float x) { return x * x; }, sf::Color::Red);
    space.render([](float x) { return -(x * x); }, sf::Color::Green);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    break;

                case sf::Event::MouseButtonReleased:
                    break;

                case sf::Event::MouseWheelScrolled:
                    break;

                default:
                    break;
            }
        }

        window.clear();
        window.draw(space);
        window.display();
    }

    return 0;
}

