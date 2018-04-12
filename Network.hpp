//Levi Purdy
//Network.hpp
//4/10/2018

#ifndef NETWORK_HPP_INCLUDED
#define NETWORK_HPP_INCLUDED


#include <SFML/Network.hpp>
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;



class NetworkClient {
    public:
    int connect(string ip, int port = 53000);

    protected:
    sf::TcpSocket socket;
    sf::Socket::Status status;

};

class NetworkServer {
public:
    int prepare();
    int listen(int port = 53000);
    int acceptClient();

protected:
    sf::TcpListener listener;
    sf::TcpSocket client;
};

#endif // NETWORK_HPP_INCLUDED
