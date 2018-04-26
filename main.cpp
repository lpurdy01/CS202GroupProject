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
        if(clock.getElapsedTime() > sf::milliseconds(100))
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
            //clientSyncLock.lock();
            pack.encodeCharacter(mainCharacter);
            pack.composePackage();
            serverConnection.send(pack);
            //clientSyncLock.unlock();
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

                    //clientSyncLock.lock();
                    serverConnection.updateCharactersVector(otherCharacters, decodedChars);
                    //clientclockrecieve.restart();
                    //clientSyncLock.unlock();
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
    sf::Clock refreshTime;
    sf::Clock scaleTime;
    float winH = 768;
    float winW = 1366;

    float inc = 128;
    float stdBlock = 64;

    sf::View view;

    sf::RenderWindow window(sf::VideoMode(winW, winH), "hElP Me!");

    //system("dir"); //Place Game Resources in this path
    auto clientID = serverConnection.clientSquak();

    Character guy("Drawing.png");
    Collidable::collideVec.push_back(&guy);
    float charW = guy.getLocalBounds().width / 5;
    float charH = guy.getLocalBounds().height / 5;

    guy.sf::Sprite::setScale(charW/guy.getLocalBounds().width, charH/guy.getLocalBounds().height);

    float spawnY = 30*inc - charH - stdBlock - 1;
    guy.sf::Sprite::setPosition(0, spawnY);
    guy.setGrid(1, charW + 1, spawnY, spawnY + charH);

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(50*inc, 50*inc));
    bg.setPosition(-5*inc, -5*inc);
    bg.setFillColor(sf::Color::White);

    Block edge1(-stdBlock,20*inc,stdBlock,10*inc);
    Block edge2(0, 30*inc - stdBlock, 8*inc, stdBlock);
    Block edge3(8*inc, 25*inc, stdBlock, 5*inc);
    Block edge4(8*inc, 25*inc, 4*inc, stdBlock);
    Block edge5(12*inc, 15*inc, inc, 10*inc+stdBlock);
    Block edge6(-stdBlock, 20*inc - stdBlock, 6*inc, stdBlock);
    Block edge7(5*inc - stdBlock, 0, stdBlock, 20*inc);
    Block edge8(5*inc-stdBlock, -stdBlock, 4*inc, stdBlock);
    Block edge9(8*inc, 15*inc, 5*inc, stdBlock);
    Block edge10(8*inc, 0, stdBlock, 13*inc);
    Block edge11(8*inc, 13*inc - stdBlock, 12*inc, stdBlock);
    Block edge12(13*inc, 17*inc, 7*inc, stdBlock);
    Block edge13(20*inc - stdBlock, 17*inc, stdBlock, 3*inc);
    Block edge14(20*inc, 20*inc - stdBlock, 5*inc, stdBlock);
    Block edge15(25*inc, 8*inc, stdBlock, 17*inc);
    Block edge16(20*inc - stdBlock, 8*inc, stdBlock, 5*inc);
    Block edge17(20*inc-stdBlock, 8*inc - stdBlock, 6*inc, stdBlock);

    Block block1(inc, 28*inc, 4*inc, stdBlock);
    Block block2(4*inc, 27*inc - stdBlock, 3*inc, stdBlock);
    Block block3(inc, 23*inc, 3*inc, inc);
    Block block4(5*inc, 21*inc, inc, 4*inc);
    Block block5(6*inc, 19*inc, 3*inc, inc);
    Block block6(8*inc, 22*inc, 3*inc, inc);
    Block block7(7*inc, 24*inc + stdBlock, inc, stdBlock);
    Block block8(11*inc, 20*inc, inc, inc);
    Block block9(10*inc, 17*inc, inc, 2*inc);
    Block block10(6*inc, 16*inc+stdBlock, 3*inc, inc);
    Block block11(6*inc, 14*inc+stdBlock, inc, stdBlock);
    Block block12(5*inc, 13*inc, 1.5*inc, stdBlock);
    Block block13(6*inc, 11*inc, 2*inc, stdBlock);
    Block block14(5*inc, 9*inc, inc, inc);
    Block block15(7*inc, 8*inc, inc, inc);
    Block block16(5*inc, 6*inc, 2*inc, stdBlock);
    Block block17(7*inc, 4*inc, inc, inc);
    Block block18(5*inc, 2*inc+stdBlock, 2*inc, stdBlock);
    Block block19(10*inc, 14*inc+stdBlock, inc, stdBlock);
    Block block20(14*inc + stdBlock, 14*inc, 5*inc + stdBlock, inc);
    Block block21(23*inc, 12*inc, 2*inc, stdBlock);
    Block block22(20*inc, 11*inc, 2*inc, stdBlock);
    Block block23(23*inc, 10*inc+stdBlock, inc, stdBlock);
    Block block24(20*inc, 9*inc, 4*inc - stdBlock, stdBlock);
    Block block25(11*inc, 24*inc, inc, inc);
    Block block26(3.5*inc, 24*inc + stdBlock, inc, stdBlock);
    Block block27(7*inc, 28*inc, inc, stdBlock);
    Block block28(24*inc+stdBlock, 18*inc+stdBlock, stdBlock, stdBlock);
    Block block29(0, 27*inc, stdBlock, inc);
    Block block30(stdBlock, 27*inc + stdBlock, stdBlock, stdBlock);
    Block block31(5*inc, 25*inc, stdBlock, inc + stdBlock);
    Block block32(19*inc + stdBlock, 13*inc, stdBlock, inc);
    Block block33(24*inc + stdBlock, 14*inc + stdBlock, stdBlock, inc);
    Block block34(19*inc + stdBlock, 14*inc + stdBlock, 2*inc + stdBlock, stdBlock);
    Block block35(23*inc + stdBlock, 16*inc + stdBlock, stdBlock, stdBlock);
    Block block36(21*inc + stdBlock, 14*inc, stdBlock, stdBlock);
    Block block37(20*inc + stdBlock, 12*inc + stdBlock, 2*inc + stdBlock, stdBlock);

    Block death1(6*inc, inc, 2*inc, stdBlock, Collidable::DEATH);
    Block death2(2*inc, 21*inc, inc, inc, Collidable::DEATH);
    Block death3(inc, 25*inc-stdBlock, 2*inc, 2*inc, Collidable::DEATH);
    Block death4(7*inc, 21*inc, 2*inc, stdBlock, Collidable::DEATH);
    Block death5(10*inc, 13*inc, inc, stdBlock, Collidable::DEATH);
    Block death6(20*inc, 19*inc, 5*inc, stdBlock, Collidable::DEATH);
    Block death7(0, 28*inc, inc, stdBlock, Collidable::DEATH);
    Block death8(7*inc, 25*inc, inc, stdBlock, Collidable::DEATH);
    Block death9(4*inc + stdBlock, 23*inc, stdBlock, stdBlock, Collidable::DEATH);
    Block death10(5*inc, 20*inc, stdBlock, inc, Collidable::DEATH);
    Block death11(8*inc, 25*inc - stdBlock, 3*inc, stdBlock, Collidable::DEATH);
    Block death12(9*inc, 16*inc + stdBlock, stdBlock, inc, Collidable::DEATH);
    Block death13(10*inc + stdBlock, 15*inc, stdBlock, 2*inc, Collidable::DEATH);
    Block death14(6*inc, 9*inc, stdBlock, inc, Collidable::DEATH);
    Block death15(5*inc, 7*inc, 2*inc - stdBlock, stdBlock, Collidable::DEATH);
    Block death16(13*inc, 15*inc, stdBlock, stdBlock, Collidable::DEATH);
    Block death17(14*inc, 14*inc, stdBlock, inc + stdBlock, Collidable::DEATH);
    Block death18(23*inc, 16*inc - stdBlock, stdBlock, inc + stdBlock, Collidable::DEATH);
    Block death19(24*inc, 16*inc - stdBlock, stdBlock, inc + stdBlock, Collidable::DEATH);
    Block death20(20*inc + stdBlock, 14*inc, inc, stdBlock, Collidable::DEATH);
    Block death21(23*inc + stdBlock, 13*inc, inc + stdBlock, stdBlock, Collidable::DEATH);
    Block death22(23*inc + stdBlock, 9*inc, stdBlock, stdBlock, Collidable::DEATH);
    Block death23(23*inc, 9*inc + stdBlock, stdBlock, inc, Collidable::DEATH);

    Block win1(21*inc, 8*inc, inc, stdBlock, Collidable::GOAL);

    sf::Text deathText;
    sf::Font comicFont;
    comicFont.loadFromFile("comicbd.ttf");
    deathText.setFont(comicFont);
    deathText.setCharacterSize(44);
    deathText.setFillColor(sf::Color::White);
    deathText.setString("YOU HAVE DIED. PRESS ENTER TO TRY AGAIN");
    deathText.setPosition(-4*inc, 30*inc - charH - stdBlock);

    sf::Text winText;
    winText.setFont(comicFont);
    winText.setCharacterSize(45);
    winText.setFillColor(sf::Color::White);
    winText.setString("CONGRATULATIONS, YOU WON!\n PRESS ENTER TO PLAY AGAIN");
    winText.setPosition(-3*inc, 30*inc - charH - stdBlock);

    sf::Text introText;
    introText.setFont(comicFont);
    introText.setCharacterSize(45);
    introText.setFillColor(sf::Color::White);
    introText.setString("      THIS IS OUR GAME!\nPRESS RETURN TO CONTINUE");
    introText.setPosition(-3*inc, 30*inc - charH - stdBlock);

    view.setSize(winW/1.2, winH/1.2);

    cout << "Client ID: " << (int)clientID << endl;
    guy.setID(clientID);
    cout << "Guy ID: " << (int)(guy.getID()) << endl;

    //Shadow Testing *********************************


    //End ****************************

    int numChars = 0;
    vector<Character> charVec;

    sf::Thread clientSnc([&serverConnection, &guy, &clientSyncLock, & otherCharacters]()
    {
        clientSync(serverConnection, guy, clientSyncLock, otherCharacters);
    });
    clientSnc.launch();

    vector<Character> testChar;

    Character testyChar1(-55,0,"Drawing.png");
    testyChar1.sf::Sprite::setScale(0.2,0.2);
    testChar.push_back(testyChar1);
    Character testyChar2(-55,0,"Drawing.png");
    testyChar2.sf::Sprite::setScale(0.2,0.2);
    testChar.push_back(testyChar2);
    Character testyChar3(-55,0,"Drawing.png");
    testyChar3.sf::Sprite::setScale(0.2,0.2);
    testChar.push_back(testyChar3);
    Character testyChar4(-55,0,"Drawing.png");
    testyChar4.sf::Sprite::setScale(0.2,0.2);
    testChar.push_back(testyChar4);


    bool startOfGame = true;

    while (window.isOpen())
    {
        //clientSyncLock.lock(); //Stops Threads from editing variables
        //Place any variable manipulation here


        sf::Event event;

        if (otherCharacters.size() > numChars)
        {
            for (int i = 0; i < otherCharacters.size(); i++)
            {
                numChars++;
            }
        }

        window.clear();
        window.draw(bg);

        window.draw(edge1);
        window.draw(edge2);
        window.draw(edge3);
        window.draw(edge4);
        window.draw(edge5);
        window.draw(edge6);
        window.draw(edge7);
        window.draw(edge8);
        window.draw(edge9);
        window.draw(edge10);
        window.draw(edge11);
        window.draw(edge12);
        window.draw(edge13);
        window.draw(edge14);
        window.draw(edge15);
        window.draw(edge16);
        window.draw(edge17);

        window.draw(block1);
        window.draw(block2);
        window.draw(block3);
        window.draw(block4);
        window.draw(block5);
        window.draw(block6);
        window.draw(block7);
        window.draw(block8);
        window.draw(block9);
        window.draw(block10);
        window.draw(block11);
        window.draw(block12);
        window.draw(block13);
        window.draw(block14);
        window.draw(block15);
        window.draw(block16);
        window.draw(block17);
        window.draw(block18);
        window.draw(block19);
        window.draw(block20);
        window.draw(block21);
        window.draw(block22);
        window.draw(block23);
        window.draw(block24);
        window.draw(block25);
        window.draw(block26);
        window.draw(block27);
        window.draw(block28);
        window.draw(block29);
        window.draw(block30);
        window.draw(block31);
        window.draw(block32);
        window.draw(block33);
        window.draw(block34);
        window.draw(block35);
        window.draw(block36);
        window.draw(block37);

        window.draw(death1);
        window.draw(death2);
        window.draw(death3);
        window.draw(death4);
        window.draw(death5);
        window.draw(death6);
        window.draw(death7);
        window.draw(death8);
        window.draw(death9);
        window.draw(death10);
        window.draw(death11);
        window.draw(death12);
        window.draw(death13);
        window.draw(death14);
        window.draw(death15);
        window.draw(death16);
        window.draw(death17);
        window.draw(death18);
        window.draw(death19);
        window.draw(death20);
        window.draw(death21);
        window.draw(death22);
        window.draw(death23);

        window.draw(win1);

        if(refreshTime.getElapsedTime() > sf::milliseconds(20))
        {
            if(otherCharacters.size() >= 1)
            {
                for(int i = 0; i < otherCharacters.size(); i++)
                {
                    testChar[i].setxPos(otherCharacters[i].getxPos());
                    testChar[i].setyPos(otherCharacters[i].getyPos());
                }
            }
            refreshTime.restart();
        }

        for(int i = 0; i < otherCharacters.size(); i++)
        {
            if(i != clientID-1)
            {
                window.draw(testChar[i]);
            }
        }

        window.draw(guy);
        guy.updateChar();

        if (guy.checkIfDead())
        {
            window.clear();
            guy.sf::Sprite::setPosition(0, spawnY);
            guy.setGrid(1, charW + 1, spawnY, spawnY + charH);
            view.setCenter(guy.sf::Sprite::getPosition().x + charW/2, guy.sf::Sprite::getPosition().y + charH/2);
            window.setView(view);

            window.draw(deathText);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                guy.setIfDead(0);
            }
        }

        if (guy.checkIfWin())
        {
            window.clear();
            guy.sf::Sprite::setPosition(0, spawnY);
            guy.setGrid(1, charW + 1, spawnY, spawnY + charH);
            view.setCenter(guy.sf::Sprite::getPosition().x + charW/2, guy.sf::Sprite::getPosition().y + charH/2);
            window.setView(view);

            window.draw(winText);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                guy.setIfWin(0);
            }
        }

        if (startOfGame)
        {
            window.clear();
            guy.sf::Sprite::setPosition(0, spawnY);
            guy.setGrid(1, charW + 1, spawnY, spawnY + charH);
            window.draw(introText);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                startOfGame = false;
            }
        }

            //clientSyncLock.unlock(); //Allows Threads to edit Variables

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            view.setCenter(guy.sf::Sprite::getPosition().x + charW/2, guy.sf::Sprite::getPosition().y + charH/2);
            window.setView(view);
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
