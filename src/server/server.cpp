#include "server.h"

Server::Server(const STARTSTYLE& style, int localPort)
{
    guiServer = new QTcpServer();
    QHostAddress address;
    switch (style) {
    case LOCAL:
    {
        address = QHostAddress::LocalHost;
        break;
    }
    case IPV4:
    {
        address = QHostAddress::AnyIPv4;
        break;
    }
    case IPV6:
    {
        address = QHostAddress::AnyIPv6;
        break;
    }
    }
    guiServer->listen(address,localPort);
    connect(guiServer,&QTcpServer::newConnection,this,&Server::guiConnectToServer);
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
    connect(t,&Thread::SIGNAL_ClientWillConnectPort,this,&Server::getBackToClientWillConnectPort);
    connect(t,&Thread::SIGNAL_Finsh,this,&Server::getBackToClientOffLine);
    t->start();
}

void Server::getBackToClientWillConnectPort(int id, QString ip, int port)
{
    QString result = Command_ONLINE;
    result += QString::number(id);
    result += "#";
    result += ip;
    result +="#";
    result +=QString::number(port);
    sendMessageToAllClient(result.toUtf8());
}

void Server::getBackToClientOffLine(int id)
{
    QString result = Command_OFFLINE;
    result += "#";
    result += QString::number(id);
    sendMessageToAllClient(result.toUtf8());
}

void Server::guiConnectToServer()
{
    QTcpSocket *socket = this->guiServer->nextPendingConnection();
    guiElement.push_back(socket);
}

void Server::sendMessageToAllClient(QByteArray byte)
{
    for (guiElementIterator=guiElement.begin();guiElementIterator!=guiElement.end();guiElementIterator++)  
    {  
        static_cast<QTcpSocket*>(*guiElementIterator)->write(byte);
    } 
}

