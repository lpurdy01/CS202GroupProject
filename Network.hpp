//Levi Purdy
//Network.hpp
//4/10/2018

#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED
#define CharacterPacket 2
#define SquakPacket 1
#define ClientIDPacket 3
#define ObjectPacket 4


#include <SFML/Network.hpp>
#include "GameBase.hpp"
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;


class NetworkPackage : public sf::Packet
{
public:
    void encodeCharacter(Character & ckar);
    vector<Character> decodeCharacters();
    void composePackage();

protected:
    sf::Uint16 numberOfCharacters = 0;
    sf::Packet packetContents;
    vector<Character> decodedCharacters;
    bool charactersDecoded = false;
    sf::Uint8 PacketType = CharacterPacket;

};


class NetworkClient
{
public:
    int connect(string ip , int port = 53000);
    int send(sf::Packet packet);
    sf::Uint8 clientSquak();
    sf::Packet recieve();

protected:
    sf::TcpSocket socket;
    sf::Socket::Status status;
    sf::Uint8 clientID;

};

class NetworkServer
{
public:
    int prepare();
    int listen(int port = 53000);
    int acceptClient();
    sf::Packet recieve();
    NetworkPackage recieveNet();
    bool handleClientSquak(sf::Packet packet);
    int getGreatestClient() {return greatestClient;}

protected:
    vector<sf::TcpSocket> goodClients;
    sf::TcpSocket client;
    sf::TcpListener listener;
    sf::Int32 greatestClient = 1;
};



#endif // NETWORK_HPP_INCLUDED
