#include <SFML/Graphics.hpp>
#include "GameBase.hpp"
#include "Network.hpp"

int main ()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Yellow);

    //system("dir"); //Place Game Resources in this path.
    
    Character guy("Drawing.png");
    guy.setScale(.25, .25);

    Background bg("cute_image.jpg");

    while (window.isOpen())
    {
        window.clear();
        window.draw(bg);
        window.draw(shape);
        window.draw(guy);

        sf::Event event;
        while (window.pollEvent(event))
        {
            int moveValue = 25;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                guy.move(-moveValue,0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                guy.move(moveValue,0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                guy.move(0,-moveValue);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                guy.move(0,moveValue);
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.display();
    }
    return 0;
}
