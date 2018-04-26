#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory.h>
#include <functional>
#include "GameBase.hpp"
#include "Network.hpp"
#include "ResourceFunctions.hpp"
#include "Network.hpp"

#define serverDebug 0
#define clientRecieveDebug 1



void runServer()
{
    std::cout << "Starting Server" << std::endl;
    sf::IpAddress ipLocal = sf::IpAddress::getLocalAddress();
    cout << "IP Address: " << ipLocal << endl;
    NetworkServer server;
    server.prepare();
    sf::Clock clock;
    vector<Character> serverCharacters;
    while(true)
    {
        sf::Uint8 packType = 0;
        NetworkPackage datapack;
        if(server.recieve(datapack))
        {
            //cout << "Recieved Datapack" << endl;
            datapack >> packType;
            if(packType == CharacterPacket)
            {
                vector<Character> decodedChars = datapack.decodeCharacters();
                server.updateCharactersVector(serverCharacters,decodedChars);
                #if(serverDebug)
            cout << "Number of Characters in Array: " << serverCharacters.size() << endl;
#endif
#if(serverDebug)
                for(Character & i:decodedChars)
                {
                    cout << "ID:  " << i.getID() << " XCor: " << i.getxPos() << " YCor: " << i.getyPos() << endl;
                }
#endif
            }
            if(packType == SquakPacket)
            {
                cout << "Recieved ID Request... This is a problem" << endl;
            }
        }
        if(clock.getElapsedTime() > sf::milliseconds(20))
        {
            server.acceptClient(false);

            if(serverCharacters.size())
            {
                #if(serverDebug)
                for(Character & i:serverCharacters)
                {
                    cout << "Character to send ID:  " << i.getID() << " XCor: " << i.getxPos() << " YCor: " << i.getyPos() << endl;
                }
                #endif
                server.sendCharacters(serverCharacters);
#if(serverDebug)
                cout << "Sending Characters" << endl;

#endif
            }
            clock.restart();
        }
    }
}

void clientSync( NetworkClient & serverConnection, Character & mainCharacter, sf::Mutex & clientSyncLock, vector<Character> & otherCharacters)
{
    sf::Clock clientclocksend;
    sf::Clock clientclockrecieve;
    while(true)
    {
        if(clientclocksend.getElapsedTime() > sf::milliseconds(100))
        {


            NetworkPackage pack;
            clientSyncLock.lock();
            pack.encodeCharacter(mainCharacter);
            pack.composePackage();
            serverConnection.send(pack);
            clientSyncLock.unlock();
            clientclocksend.restart();
        }
        if(clientclockrecieve.getElapsedTime() > sf::milliseconds(150))
        {


            sf::Uint8 packType = 0;
            sf::Packet clientdatapack;
            if(serverConnection.recieve(clientdatapack))
            {
                NetworkPackage clientNetworkPackage;
                clientNetworkPackage.append(clientdatapack.getData(),clientdatapack.getDataSize());
                clientNetworkPackage >> packType;
                if(packType == CharacterPacket)
                {
                    vector<Character> decodedChars = clientNetworkPackage.decodeCharacters();

                    clientSyncLock.lock();
                    serverConnection.updateCharactersVector(otherCharacters, decodedChars);
                    clientSyncLock.unlock();
#if(clientRecieveDebug)
                    cout << "Recieved Character Package From Server" << endl;
                    for(auto & i:decodedChars)
                    {
                        cout << "Client Side: ID:  " << i.getID() << " XCor: " << i.getxPos() << " YCor: " << i.getyPos() << endl;
                    }
#endif
                }
            }
        }
    }
}


void runGame (NetworkClient & serverConnection)
{
    sf::Mutex clientSyncLock;
    vector<Character> otherCharacters;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    Clock::clock.restart();
    int windowHeight = desktop.height/1.5;
    int windowWidth = desktop.width/1.5;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "TESTING!");
    //sf::CircleShape shape(50);
    //shape.setFillColor(sf::Color::Yellow);

    //system("dir"); //Place Game Resources in this path
    auto clientID = serverConnection.clientSquak();

    Character guy("Drawing.png");
    guy.sf::Sprite::setScale(.25, .25);
    std::cout << "width: " << guy.getLocalBounds().width << "  height: " << guy.getLocalBounds().height << std::endl;
    guy.setID(clientID);

    //  Background bg("cute_image.jpg");
    // bg.setScale(2,2);
    sf::RectangleShape bg(sf::Vector2f(windowWidth,windowHeight));
    bg.setFillColor(sf::Color::White);

    sf::Thread clientSnc([&serverConnection, &guy, &clientSyncLock, & otherCharacters]()
    {
        clientSync(serverConnection, guy, clientSyncLock, otherCharacters);
    });
    clientSnc.launch();

    while (window.isOpen())
    {
        clientSyncLock.lock(); //Stops Threads from editing variables
        //Place any variable manipulation here

        window.clear();
        window.draw(bg);
        window.draw(guy);

        sf::Event event;

        guy.updateChar();
        clientSyncLock.unlock(); //Allows Threads to edit Variables

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
