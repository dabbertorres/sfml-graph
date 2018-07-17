#include <cmath>
#include <chrono>
#include <random>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "Space.hpp"

int main(int argc, char** argv)
{
    auto scroll = [](sf::View& view, float amount)
    {
        static const sf::Vector2f viewMax = { 40.f, 40.f };
        static const sf::Vector2f viewMin = { 4.f, 4.f };

        view.zoom(1.0f - 0.1f * amount / std::abs(amount));
        auto size = view.getSize();
        size.x = std::min(viewMax.x, std::max(viewMin.x, size.x));
        size.y = std::min(viewMax.y, std::max(viewMin.y, size.y));
        view.setSize(size);
    };

    auto randByte = []()
    {
        static std::random_device rd;
        static std::mt19937 gen{ rd() };
        static std::uniform_int_distribution<> dist{ 0x0u, 0xffu };
        return static_cast<uint8_t>(dist(gen) % 0x100);
    };

    sf::RenderWindow window{{1280, 720}, "Graphing Test"};
    window.setVerticalSyncEnabled(true);

    auto viewportToTarget = [&](sf::FloatRect r)
    {
        auto w = static_cast<float>(window.getSize().x);
        auto h = static_cast<float>(window.getSize().y);
        r.left *= w;
        r.width *= w;
        r.top *= h;
        r.height *= h;
        return r;
    };

    sf::View topLeft{ {0.f, 0.f}, {10.f, 10.f} };
    topLeft.setViewport({ 0.f, 0.f, 0.49f, 0.49f });

    sf::View topRight{ { 0.0f, 0.f }, { 10.f, 10.f } };
    topRight.setViewport({ 0.51f, 0.f, 0.49f, 0.49f });

    sf::View botLeft{ { 0.0f, 0.f }, { 10.f, 10.f } };
    botLeft.setViewport({ 0.f, 0.51f, 0.49f, 0.49f });

    sf::View botRight{ { 0.0f, 0.f }, { 10.f, 10.f } };
    botRight.setViewport({ 0.51f, 0.51f, 0.49f, 0.49f });

    sfx::graph::Space space_1{ {-50.f, -50.f, 100.f, 100.f}, { 1.f, 1.f }, { 0.01f, 0.01f }, sf::Color::White};
    space_1.render([](float x) { return x * x; }, sf::Color::Red);
    space_1.render([](float x) { return -(x * x); }, sf::Color::Green);
    space_1.render([](float x) { return std::sqrt(x); }, sf::Color::Yellow);

    sfx::graph::Space space_2{ {-50.f, -50.f, 100.f, 100.f}, { 1.f, 1.f}, { 0.01f, 0.01f }, sf::Color::White};
    auto& line_1 = space_2.render([](float x) { return x - 5; }, sf::Color::Blue);
    auto& line_2 = space_2.render([](float x) { return -x + 5; }, sf::Color::Cyan);
    auto& line_3 = space_2.render([](float x) { return -x - 5; }, sf::Color::Red);
    auto& line_4 = space_2.render([](float x) { return x + 5; }, sf::Color::Magenta);

    sfx::graph::Space space_3{ {-50.f, -50.f, 100.f, 100.f}, { 1.f, 1.f}, { 0.01f, 0.01f }, sf::Color::White};
    space_3.render([](float x) { return std::cos(x * 3.141596); }, sf::Color::Red);
    space_3.render([](float x) { return std::sin(x * 3.141596); }, sf::Color::Green);
    space_3.render([](float x) { return std::tan(x * 3.141596); }, sf::Color::Yellow);

    sfx::graph::Space space_4{ {-50.f, -50.f, 100.f, 100.f}, { 1.f, 1.f}, { 0.01f, 0.01f }, sf::Color::White};
    space_4.render([](float x) { return std::pow(2.f, x); }, sf::Color::Magenta);
    space_4.render([](float x) { return -std::pow(2.f, x); }, sf::Color::Green);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        return 0;
                    break;

                case sf::Event::MouseWheelScrolled:
                    {
                        auto pos = window.mapPixelToCoords({ event.mouseWheelScroll.x, event.mouseWheelScroll.y });
                        if (viewportToTarget(topLeft.getViewport()).contains(pos))
                            scroll(topLeft, event.mouseWheelScroll.delta);
                        else if (viewportToTarget(topRight.getViewport()).contains(pos))
                            scroll(topRight, event.mouseWheelScroll.delta);
                        else if (viewportToTarget(botLeft.getViewport()).contains(pos))
                            scroll(botLeft, event.mouseWheelScroll.delta);
                        else if (viewportToTarget(botRight.getViewport()).contains(pos))
                            scroll(botRight, event.mouseWheelScroll.delta);
                    }
                    break;

                case sf::Event::Closed:
                    return 0;

                default:
                    break;
            }
        }

        line_1.setColor({ randByte(), randByte(), randByte() });
        line_2.setColor({ randByte(), randByte(), randByte() });
        line_3.setColor({ randByte(), randByte(), randByte() });
        line_4.setColor({ randByte(), randByte(), randByte() });

        window.clear();

        window.setView(topLeft);
        window.draw(space_1);

        window.setView(topRight);
        window.draw(space_2);

        window.setView(botLeft);
        window.draw(space_3);

        window.setView(botRight);
        window.draw(space_4);

        window.setView(window.getDefaultView());

        window.display();
    }

    return 0;
}

