//Levi Purdy
//Netowrk.cpp


#include "Network.hpp"


int NetworkClient::connect(string ip, int port)
{
    cout << "Connecting ..." << endl;
    if(ip == "")
    {
        sf::IpAddress ipLocal = sf::IpAddress::getLocalAddress();
        cout << "Connecting to " << ipLocal << endl;
        auto status = socket.connect(ipLocal, port);
        sf::sleep(sf::milliseconds(500));
        return 1;
    }
    else
    {
        cout << "Connecting to " << ip << endl;
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
int NetworkClient::send(sf::Packet packet)
{
    cout << "Sending Packet" << endl;
    socket.send(packet);
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

sf::Packet NetworkServer::recieve()
{
    sf::Packet data;
    client.receive(data);
    return data;
}
NetworkPackage NetworkServer::recieveNet()
{
    NetworkPackage data;
    client.receive(data);
    return data;
}

int NetworkServer::prepare()
{
    listen();
    acceptClient();
    return 1;
}

void NetworkPackage::encodeCharacter(Character & ckar)
{
    sf::Uint32 siz = sizeof(ckar.getID()) + (sizeof(ckar.getxPos())*2) + (sizeof(ckar.getxVel())*2);
    packetContents << siz << ckar.getID() << ckar.getxPos() << ckar.getyPos() << ckar.getxVel() << ckar.getyVel(); //getNCorr is int32
    numberOfCharacters++;

}
void NetworkPackage::composePackage()
{
    *this << numberOfCharacters;
    *this << packetContents;
}
vector<Character> NetworkPackage::decodeCharacters()
{
    if(charactersDecoded)
    {
        return decodeCharacters();
    }
    else
    {
        vector<Character> charvect;
        *this >> numberOfCharacters;
        cout << "Number of Characters Recieved: " << numberOfCharacters << endl;
        bool okToDecode = true;
        for(size_t i = 0; i < numberOfCharacters; i++)
        {
            if(okToDecode)
            {
                sf::Uint32 characterDataSize;
                sf::Uint8 character_id;
                sf::Int32 xCor;
                sf::Int32 yCor;
                double xVel;
                double yVel;
                *this >> characterDataSize;
                Character holder;
                sf::Uint32 siz = sizeof(holder.getID()) + (sizeof(holder.getxPos())*2) + (sizeof(holder.getxVel())*2);
                if(characterDataSize == siz){
                    *this >> character_id >> xCor >> yCor >> xVel >> yVel;
                    holder.setID(character_id);
                    holder.setxPos(xCor);
                    holder.setyPos(yCor);
                    holder.setxVel(xVel);
                    holder.setyVel(yVel);
                    charvect.push_back(holder);

                }
                else{
                    okToDecode = false;
                    cout << "INCOMPATABLE CHARACTER DATA SIZE ABORTING DECODE" << "  Expected size: " << siz << " Recieved size: " << characterDataSize << endl;

                }
            }
        }
        charactersDecoded = true;
        return charvect;
    }
}


