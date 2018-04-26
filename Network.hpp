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
#include <memory>
#include <list>


class ClientSocket : public sf::TcpSocket{
public:
    int hasConnected = 0;
};

class NetworkPackage : public sf::Packet
{
public:
    void encodeCharacter(Character & ckar);
    vector<Character> decodeCharacters();
    void composePackage();
    void setPackageType(int type) {PacketType = type;}

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
    int connect(string ip , int port = 80);
    int send(sf::Packet packet);
    sf::Uint8 clientSquak();
    bool recieve(vector<Character> & characters);
    bool recieve(sf::Packet & pack);
    sf::Packet recieve(bool Blocking = true);
    void updateCharactersVector(vector<Character> & serverCharacters, vector<Character> & incomingCharacters);

protected:
    sf::TcpSocket socket;
    sf::Socket::Status status;
    sf::Uint8 clientID;

};

class NetworkServer
{
public:
    int prepare();
    int listen(int port = 80);
    int acceptClient(bool blocking);
    bool recieve(sf::Packet & pack);
    //sf::Packet recieve(bool Blocking = true);
    bool handleClientSquak(sf::Packet & packet, ClientSocket & client);
    int getGreatestClient() {return greatestClient;}
    bool sendCharacters(vector<Character> & characters);
    void printGoodClients();
    void updateCharactersVector(vector<Character> & serverCharacters, vector<Character> & incomingCharacters);

protected:
    std::list<ClientSocket> goodClients;
    sf::TcpListener listener;
    sf::Uint8 greatestClient = 1;
};





#endif // NETWORK_HPP_INCLUDED
