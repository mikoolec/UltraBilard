#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;


class BilardBall : public sf::CircleShape
{
public:
    sf::Vector3i velocity;
    sf::IntRect bounds;
    BilardBall(float radius, const sf::Vector2f& position):
        sf::CircleShape(radius),
        velocity(sf::Vector3i(0,0,0)),
        bounds(sf::IntRect(0,0,0,0))
    {
        setPosition(position);
        setOrigin(sf::Vector2f(radius,radius));
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "My window");

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

          //my_rectangle.animate(elapsed);
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        // draw everything here...
          //window.draw(my_rectangle);
        // end the current frame
        window.display();
    }


    return 0;
}
