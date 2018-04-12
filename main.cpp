#include <SFML/Graphics.hpp>
#include "GameBase.hpp"
#include "Network.hpp"
#include "ResourceFunctions.hpp"
#include "Network.hpp"



int main ()
{
    enum class type {Client , Server};
    type consoleType;
    while(true){
    int t = getInt("Welcome to our Game, are you a server (1) or client (2) or offline (3)?");
    if(t == 1){
        consoleType = type::Server;
        break;
    }

    else if (t == 2) {
        consoleType = type::Client;
        break;
    }
    else {
        std::cout << "Please try again" << std::endl;
    }
    }
    if(consoleType == type::Server){
        std::cout << "Entering Server Mode" << std::endl;
        NetworkServer server;
        server.prepare();
    }
    else if(consoleType == type::Client)
    {
        string ip = getLine("Please enter ip of server");
        NetworkClient client;
        client.connect(ip);
    }


    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Yellow);

    //system("dir"); //Place Game Resources in this path.

    sf::Texture texture;
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
