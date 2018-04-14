#include <SFML/Graphics.hpp>
#include "GameBase.hpp"
#include "Network.hpp"
#include "ResourceFunctions.hpp"
#include "Network.hpp"

void runOfflineGame () {
    int windowHeight = 600;
    int windowWidth = 800;
    sf::RenderWindow window(sf::VideoMode(2*windowWidth, 2*windowHeight), "SFML works!");
    //sf::CircleShape shape(50);
    //shape.setFillColor(sf::Color::Yellow);

    //system("dir"); //Place Game Resources in this path


    Character guy("Drawing.png");
    guy.setScale(.25, .25);

    Background bg("cute_image.jpg");
    bg.setScale(2,2);

    while (window.isOpen())
    {
        window.clear();
        window.draw(bg);
        window.draw(guy);

        sf::Event event;
        while (window.pollEvent(event))
        {
            int moveValue = 25;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                guy.move(-moveValue,0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                guy.move(moveValue,0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                guy.move(0,-moveValue);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                guy.move(0,moveValue);
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.display();
    }
}

int main ()
{
    enum class type {Client, Server, Offline};
    type consoleType;
    while(true)
    {
        int t = getInt("Welcome to our Game, are you a server (1) or client (2) or offline (3)?");
        if(t == 1)
        {
            consoleType = type::Server;
            break;
        }

        else if (t == 2)
        {
            consoleType = type::Client;
            break;
        }
        else if(t == 3)
        {
            consoleType = type::Offline;
            break;
        }
        else
        {
            std::cout << "Please try again" << std::endl;
        }
    }
    if(consoleType == type::Server)  // ************************Server **************
    {
        std::cout << "Entering Server Mode" << std::endl;
        NetworkServer server;
        server.prepare();
    }
    else if(consoleType == type::Client) //********************Client***********
    {
        string ip = getLine("Please enter ip of server"); //Make sure to add check for valid IP
        NetworkClient client;
        if(client.connect(ip)){
            runOfflineGame();
        }
        return 0;
    }
    else if(consoleType == type::Offline) //******************Offline*************
    {
        runOfflineGame();
    }
}
