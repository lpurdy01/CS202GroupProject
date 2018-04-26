//Levi Purdy
//Netowrk.cpp


#include "Network.hpp"
#define acceptClientDebug 0
#define clientRecieveDebug 0
#define clientSendDebug 0


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
        return 1;
    }
    cout << "Connected" << endl;
    return 1;
}


int NetworkClient::send(sf::Packet packet)
{
    #if(clientSendDebug)
    cout << "Sending Packet" << endl;
    #endif
    socket.setBlocking(true);
    socket.send(packet);
    socket.setBlocking(false);
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


int NetworkServer::acceptClient(bool blocking)
{
    if(blocking)
    {
        #if(acceptClientDebug == 1)
        cout << "Blocking accept Client Called" << endl;
        printGoodClients();
        #endif
        listener.setBlocking(true);
        if(goodClients.back().hasConnected == 0)
        {
            goodClients.back().setBlocking(true);
        }
        else
        {
            goodClients.emplace_back();
            cout << "Adding Client To GoodClient List From Blocking" << endl;
        }
        cout << "Waiting for Client" << endl;
        if (listener.accept(goodClients.back()) != sf::Socket::Done)
        {
            cout << "Something Bad happened, Client didn't Connect" << endl;
            return 0;
            //goodClients.pop_back();
        }
        cout << "Found Client: " << goodClients.back().getRemoteAddress() << endl;
        sf::Packet clientSquak;
        goodClients.back().receive(clientSquak);
        if(handleClientSquak(clientSquak, goodClients.back()))
        {
            goodClients.back().setBlocking(false);
            goodClients.back().hasConnected = 1;
            return 1;
        }
        else
        {
            return 0;
            //goodClients.pop_back();
        }
    }
    else
    {
        #if(acceptClientDebug == 1)
        cout << "Non Blocking Accept Called" << endl;
        printGoodClients();
        #endif
        listener.setBlocking(false);
        if(goodClients.back().hasConnected == 1)
        {
            goodClients.emplace_back();
            cout << "Adding Client To GoodClient List From NonBlocking" << endl;
            goodClients.back().setBlocking(false);
            return 0;
        }
        else if(goodClients.back().hasConnected == 0)
        {
            goodClients.back().setBlocking(false);
        }
        if (listener.accept(goodClients.back()) == sf::Socket::Done)
        {
            cout << "Accepting New Client" << endl;
            cout << "Found Client: " << goodClients.back().getRemoteAddress() << endl;
            sf::Packet clientSquak;
            goodClients.back().setBlocking(true);
            goodClients.back().receive(clientSquak);
            if(handleClientSquak(clientSquak, goodClients.back()))
            {
                goodClients.back().setBlocking(false);
                goodClients.back().hasConnected = 1;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

}

bool NetworkServer::recieve(sf::Packet & pack)
{
    for(auto & client:goodClients)
    {
        sf::Packet tempPack;
        if(client.receive(tempPack) == sf::Socket::Status::Done)
        {
            pack = tempPack;

            return true;
        }
    }
    return false;

}

bool NetworkServer::sendCharacters(vector<Character> & characters)
{
    NetworkPackage package;
    for(auto & i:characters)
    {
        package.encodeCharacter(i);
    }
    package.composePackage();

    for(auto & i:goodClients)
    {
        if(i.hasConnected == 1)
        {
            i.setBlocking(true);
           // cout << "Server Sending Package with data address: " << package.getData() << endl;
            i.send(package);
            //cout << "Sending Package Size: " << package.getDataSize() << endl;
            i.setBlocking(false);
        }
    }
	return false;
}

bool NetworkClient::recieve(sf::Packet & pack)
{
    sf::Packet tempPack;
    socket.setBlocking(true);
        if(socket.receive(tempPack) == sf::Socket::Status::Done)
        {
            //cout << "Temp Pack size: "<<tempPack.getDataSize() << " Temp Pack Data: " << tempPack.getData() << endl;
            pack.append(tempPack.getData(),tempPack.getDataSize());
            return true;
        }
        else return false;
}

bool NetworkClient::recieve(vector<Character> & characters)
{
    NetworkPackage pack;
    socket.setBlocking(false);
    if(socket.receive(pack) == sf::Socket::Status::Done)
    {
        #if(clientRecieveDebug)
        cout << "Client Recieved Packet" << endl;
        #endif
        sf::Uint8 packType;
        pack >> packType;
        if(packType == CharacterPacket)
        {
           characters = pack.decodeCharacters();
           return true;
        }
        else
        {
            cout << "Recieved non Character Packet" << endl;
            return false;
        }
    }
    else
    {
        return false;
    }

}
sf::Packet NetworkClient::recieve(bool Blocking)
{
    sf::Packet pack;
    socket.setBlocking(Blocking);
    socket.receive(pack);
    socket.setBlocking(false);
    return pack;
}

sf::Uint8 NetworkClient::clientSquak()
{
    cout << "Sending Client Squak Packet" << endl;
    sf::Packet dataPack;
    sf::Uint8 packetType = SquakPacket;
    dataPack << packetType;
    socket.send(dataPack);
    dataPack = recieve(true);
    dataPack >> packetType;
    if(packetType == ClientIDPacket)
    {
        sf::Uint8 charID;
        dataPack >> charID;
        clientID = charID;
        return charID;
    }
    else
    {
        cout << "Bad Client ID" << endl;
        return 0;
    }

}
bool NetworkServer::handleClientSquak(sf::Packet & packet, ClientSocket & client)
{
    sf::Uint8 packType = 0;
    packet >> packType;
    if(packType == SquakPacket)
    {
        cout << "Recieved ID Request" << endl;
        sf::Packet clientIDPacket;
        sf::Uint8 packetType = ClientIDPacket;
        clientIDPacket << packetType << greatestClient;
        greatestClient++;
        client.send(clientIDPacket);
        cout << "Gave Client ID: " << (greatestClient - 1) << endl;
        return 1;
    }
    else
    {
        cout << "Bad client Squak" << endl;
        return 0;
    }
}

int NetworkServer::prepare()
{
    listen();
    goodClients.emplace_back();
    acceptClient(true);
    return 1;
}

void NetworkPackage::encodeCharacter(Character & ckar)
{
    sf::Uint32 siz = sizeof(ckar.getID()) + (sizeof((sf::Int32)(ckar.sf::Sprite::getPosition().x))*2) + (sizeof(ckar.getxVel())*2);
    packetContents << siz << ckar.getID() << ((sf::Int32)(ckar.sf::Sprite::getPosition().x)) << ((sf::Int32)(ckar.sf::Sprite::getPosition().y)) << ckar.getxVel() << ckar.getyVel(); //getNCorr is int32
    numberOfCharacters++;

}
void NetworkPackage::composePackage()
{
    *this << PacketType;
    *this << numberOfCharacters;
    this -> append(packetContents.getData(),packetContents.getDataSize());
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
        //cout << "Number of Characters Recieved: " << numberOfCharacters << endl;
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
                //cout << characterDataSize << endl;
                Character holder;
                sf::Uint32 siz = sizeof(holder.getID()) + (sizeof(holder.getxPos())*2) + (sizeof(holder.getxVel())*2);
                if(characterDataSize == siz)
                {
                    *this >> character_id >> xCor >> yCor >> xVel >> yVel;
                    holder.setID(character_id);
                    holder.setxPos(xCor);
                    holder.setyPos(yCor);
                    holder.setxVel(xVel);
                    holder.setyVel(yVel);
                    charvect.push_back(holder);

                }
                else
                {
                    okToDecode = false;
                    cout << "INCOMPATABLE CHARACTER DATA SIZE ABORTING DECODE" << "  Expected size: " << siz << " Recieved size: " << characterDataSize << endl;

                }
            }
        }
        charactersDecoded = true;
        return charvect;
    }
}

void NetworkServer::printGoodClients()
{
    for(ClientSocket& i:goodClients)
    {
        cout << "Client: " << i.getRemoteAddress() << "Is Connected = " << i.hasConnected << endl;
    }
}

void NetworkServer::updateCharactersVector(vector<Character> & serverCharacters, vector<Character> & incomingCharacters)
{
    for(Character & i:incomingCharacters)
    {
        bool present = false;
        for(Character & a:serverCharacters)
        {
            if(i.getID() == a.getID())
            {
                a.setxPos(i.getxPos());
                a.setyPos(i.getyPos());
                a.setxVel(i.getxVel());
                a.setyVel(i.getyVel());
                present = true;
            }
        }
        if(!present)
        {
            serverCharacters.push_back(i);
        }
    }
}

void NetworkClient::updateCharactersVector(vector<Character> & serverCharacters, vector<Character> & incomingCharacters)
{
    for(Character & i:incomingCharacters)
    {
        bool present = false;
        for(Character & a:serverCharacters)
        {
            if(i.getID() == a.getID())
            {
                a.setxPos(i.getxPos());
                a.setyPos(i.getyPos());
                a.setxVel(i.getxVel());
                a.setyVel(i.getyVel());
                present = true;
            }
        }
        if(!present)
        {
            serverCharacters.push_back(i);
        }
    }
}

