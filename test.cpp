#include <cmath>
#include <stdexcept>
#include <iostream>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "Graph.hpp"

int main(int argc, char** argv)
{
    sf::RenderWindow window{{1280, 720}, "Graph"};
    window.setVerticalSyncEnabled(true);

    sf::View view{{0.f, 0.f}, {20.f, -20.f}};
    window.setView(view);

    sfx::Graph graph{ -10.f, 10.f, 0.2f,
                      -10.f, 10.f, 0.2f,
                      true, sf::Color::Red, sf::Color::White };
    graph.render([](float x) { return x * x; });

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
        window.draw(graph);
        window.display();
    }

    return 0;
}

