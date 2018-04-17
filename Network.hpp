//Levi Purdy
//Network.hpp
//4/10/2018

#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED


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
    sf::Uint8 PacketType = 2;

};


class NetworkClient
{
public:
    int connect(string ip , int port = 53000);
    int send(sf::Packet packet);

protected:
    sf::TcpSocket socket;
    sf::Socket::Status status;

};

class NetworkServer
{
public:
    int prepare();
    int listen(int port = 53000);
    int acceptClient();
    sf::Packet recieve();
    NetworkPackage recieveNet();

protected:
    sf::TcpListener listener;
    sf::TcpSocket client;
};



#endif // NETWORK_HPP_INCLUDED
