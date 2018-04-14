//Levi Purdy
//Netowrk.cpp


#include "Network.hpp"


int NetworkClient::connect(string ip, int port)
{
    cout << "Connecting ..." << endl;
    if(ip == "")
    {
        auto status = socket.connect(sf::IpAddress::getLocalAddress(), port);
    }
    else
    {
        auto status = socket.connect(ip, port);
    }
    if (status != sf::Socket::Done)
    {
        cout << "Failed to Connect" << endl;
        return 0;
    }
    cout << "Connected" << endl;
    return 1;
}

int NetworkServer::listen(int port)
{
    cout << "Starting to Listen" << endl;
    if (listener.listen(port) != sf::Socket::Done)
    {
        cout << "Port failed to bind" << endl;
        return 0;
    }
    cout << "Listening" << endl;
    return 1;
}


int NetworkServer::acceptClient()
{
    cout << "Waiting for Client" << endl;
    if (listener.accept(client) != sf::Socket::Done)
    {
        cout << "Something Bad happened" << endl;
        return 0;
    }
    cout << "Found Client: " << client.getRemoteAddress() << endl;
    return 1;
}

int NetworkServer::prepare()
{
    listen();
    acceptClient();
    return 1;
}

void NetworkPackage::encodeCharacter(Character & ckar, sf::Uint8 character_id)
{
    sf::Uint32 siz = sizeof(character_id) + sizeof(ckar.getXCorr()) + sizeof(ckar.getYCorr());
    packetContents << siz << character_id << ckar.getXCorr() << ckar.getYCorr(); //getNCorr is int32
    numberOfCharacters++;

}
void NetworkPackage::composePackage()
{
    *this << numberOfCharacters;
    *this << packetContents;
}


