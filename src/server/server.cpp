#include "server.h"

Server::Server()
{

}

bool Server::startServer(int port, const QHostAddress &address)
{
    if(this->isListening())
    {
        return false;
    }
    return this->listen(address,port);
}

void Server::stopServer()
{
    if(this->isListening())
    {
        this->close();
    }
}

void Server::incomingConnection(qintptr handle)
{
    auto *t = new Thread(handle);
    t->start();
}

