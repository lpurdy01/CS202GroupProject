#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory.h>
#include <functional>
#include "GameBase.hpp"
#include "Network.hpp"
#include "ResourceFunctions.hpp"
#include "Network.hpp"

#define serverDebug 0
#define clientRecieveDebug 0



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
        if(clientclockrecieve.getElapsedTime() > sf::milliseconds(110))
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
                    for(auto & i:otherCharacters)
                    {
                        cout << "Client Side: ID:  " << (int)(i.getID()) << " XCor: " << i.getxPos() << " YCor: " << i.getyPos() << endl;
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
    float windowHeight = 768;
    float windowWidth = 1366;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "hElP Me!");

    sf::Text deathText;
    sf::Font deathFont;
    deathFont.loadFromFile("comicbd.ttf");
    deathText.setFont(deathFont);
    deathText.setCharacterSize(50);
    deathText.setFillColor(sf::Color::White);
    deathText.setString("YOU ARE DEAD. PRESS ENTER TO TRY AGAIN");
    deathText.setPosition(windowWidth / 25, windowHeight / 2 - 50);

	sf::Font font;
	font.loadFromFile("comicbd.ttf");

	sf::Text text;
	text.setFont(font);
	text.setPosition(200, 200);
	text.setCharacterSize(50);
	text.setFillColor(sf::Color::Yellow);
	text.setString("This is our game!\n Press space to continue");

    //system("dir"); //Place Game Resources in this path
    auto clientID = serverConnection.clientSquak();

    Character guy("Drawing.png");
    guy.sf::Sprite::setScale(.25, .25);
    guy.setGrid(0, guy.getLocalBounds().width*.25, 0, guy.getLocalBounds().height*.25);
    cout << "Client ID: " << (int)clientID << endl;
    guy.setID(clientID);
    cout << "Guy ID: " << (int)(guy.getID()) << endl;


    Block ground(0,windowHeight-windowHeight/10,windowWidth,windowHeight/10);
    ground.setFillColor(sf::Color::Black);

    Block leftBound(-2,0,1,windowHeight);
    Block rightBound(windowWidth+1,0,1,windowHeight);

    Block block1(windowHeight/2,windowHeight-windowHeight/5.33,windowHeight/10,windowHeight/10);
    block1.setFillColor(sf::Color::Black);

    Block block2(windowWidth / 2, windowHeight - windowHeight / 2, windowWidth / 8, windowHeight / 10);
	block2.setFillColor(sf::Color::Black);

    Block block3(3*windowWidth / 4, windowHeight - windowHeight / 2, windowWidth / 8, windowHeight / 10, Collidable::GOAL);
	block3.setFillColor(sf::Color::Yellow);

    sf::RectangleShape bg(sf::Vector2f(windowWidth,windowHeight));
    bg.setFillColor(sf::Color::White);

    sf::Thread clientSnc([&serverConnection, &guy, &clientSyncLock, & otherCharacters]()
    {
        clientSync(serverConnection, guy, clientSyncLock, otherCharacters);
    });
    clientSnc.launch();

	bool wait = false;

    while (window.isOpen())
    {
        clientSyncLock.lock(); //Stops Threads from editing variables
        //Place any variable manipulation here

        sf::Event event;
        window.clear();
        window.draw(text);

//        if (event.type == sf::Event::KeyPressed)
//        {
//            wait = false;
//        }

        window.clear();
        window.draw(bg);
        window.draw(ground);
        window.draw(block1);
        window.draw(block2);
        window.draw(block3);
        for(int i = 0; i < otherCharacters.size(); i++)
        {
            otherCharacters[i].sf::Sprite::setScale(.25, .25);
            window.draw(otherCharacters[i]);
        }

        window.draw(guy);
        
        guy.updateChar();

        if (guy.checkIfDead())
        {
            window.clear();
            window.draw(deathText);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                window.close();
                runGame(serverConnection);
            }
        }

        if (guy.checkIfWin())
        {
            window.clear();
            sf::Text winText;
            sf::Font winFont;
            winFont.loadFromFile("comicbd.ttf");
            winText.setFont(winFont);
            winText.setCharacterSize(50);
            winText.setFillColor(sf::Color::White);
            winText.setString("CONGRATULATIONS, YOU WON!\n PRESS ENTER TO PLAY AGAIN");
            winText.setPosition(windowWidth / 5, windowHeight / 2 - 50);
            window.draw(winText);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                window.close();
//                for (int i = 0; i < Collidable::collideVec.size(); i++)
//                {
//                    if (Collidable::collideVec[i] == &guy)
//                    {
//                        Collidable::collideVec.erase(Collidable::collideVec.begin()+i);
//                    }
//                }
                runGame(serverConnection);
            }
        }

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
