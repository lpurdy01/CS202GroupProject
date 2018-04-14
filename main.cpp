#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory.h>
#include <functional>
#include "GameBase.hpp"
#include "Network.hpp"
#include "ResourceFunctions.hpp"
#include "Network.hpp"


void runServer()
{
    std::cout << "Starting Server" << std::endl;
    NetworkServer server;
    server.prepare();
    while(true)
    {
        NetworkPackage datapack;
        datapack = server.recieveNet();
        //cout << "Recieved Datapack" << endl;
        vector<Character> decodedChars = datapack.decodeCharacters();
        for(Character i:decodedChars){
            cout << "ID:  " << i.getID() << " XCor: " << i.getxPos() << " YCor: " << i.getyPos() << endl;
        }
    }
}
void clientSync( NetworkClient & serverConnection, Character & mainCharacter)
{
    while(true){
        NetworkPackage pack;
        pack.encodeCharacter(mainCharacter);
        pack.composePackage();
        serverConnection.send(pack);
        sf::sleep(sf::milliseconds(100));
    }
}

void runGame (NetworkClient & serverConnection)
{
    Clock::clock.restart();
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

    sf::Thread clientSnc([&serverConnection, &guy]()
    {
        clientSync(serverConnection, guy);
    });
    clientSnc.launch();

    while (window.isOpen())
    {
        window.clear();
        window.draw(bg);
        window.draw(guy);

        sf::Event event;

        guy.updateChar();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.display();
    }
    clientSnc.terminate();
}

void getTime ()
{
    // This is just a placeholder function for me to remember how to access the clock
    sf::Time time = Clock::clock.getElapsedTime();
    std::cout << time.asSeconds() << std::endl;
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
        runServer();
    }
    else if(consoleType == type::Client) //********************Client***********
    {
        string ip = getLine("Please enter ip of server"); //Make sure to add check for valid IP
        NetworkClient client;
        if(client.connect(ip))
        {
            runGame(client);
        }
        return 0;
    }
    else if(consoleType == type::Offline) //******************Offline*************
    {
        sf::Thread server(&runServer);
        server.launch();
        sf::sleep(sf::seconds(1));
        NetworkClient client;
        if(client.connect(""))
        {
            runGame(client);
        }
        server.terminate();
    }
}
