#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "Network.hpp"

int main ()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Yellow);

    sf::Texture texture;
    system("dir"); //Place Game Resources in this path.
    if (!texture.loadFromFile("Drawing.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite guy(texture);
    guy.setScale(.25, .25);

    sf::Texture background;
    if (!background.loadFromFile("cute_image.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Sprite background1(background);
    background1.setScale(1.5,1.5);

    while (window.isOpen())
    {
        window.clear();
        window.draw(background1);
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
