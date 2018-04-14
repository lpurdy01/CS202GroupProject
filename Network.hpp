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
#include <ostream>
using std::ostream;
using std::cout;
using std::endl;
#include <vector>
using std::vector;



class NetworkClient
{
public:
    int connect(string ip , int port = 53000);

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

protected:
    sf::TcpListener listener;
    sf::TcpSocket client;
};

class NetworkPackage : public sf::Packet
{
public:
    NetworkPackage();
    void encodeCharacter(Character & ckar, sf::Uint8 character_id = 0);
    vector<Character> decodeCharacters();
    void composePackage();

protected:
    sf::Uint16 numberOfCharacters = 0;
    sf::Packet packetContents;

};

#endif // NETWORK_HPP_INCLUDED
