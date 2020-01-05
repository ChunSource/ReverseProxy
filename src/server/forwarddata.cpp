#include "forwarddata.h"
#include <QDebug>

ForwardData::ForwardData(QTcpSocket *p, QTcpSocket *c, QObject *parent) : QObject(parent)
{
    this->server = p;
    this->client = c;
}

ForwardData::~ForwardData()
{
    qDebug()<<"[*]---DataExchangeFinsh";
    if(server != nullptr)
    {
        server->disconnectFromHost();
        server->deleteLater();
    }
    
    if(client != nullptr)
    {
        client->disconnectFromHost();
        client->deleteLater();
    }
}

void ForwardData::forward()
{
    qDebug()<<"[*]---DataExchange";
    if(this->server == nullptr || this->client == nullptr)
    {
        this->deleteLater();
        return;
    }
    
    connect(server,&QTcpSocket::readyRead,this,&ForwardData::writeToClient);
    connect(client,&QTcpSocket::readyRead,this,&ForwardData::writeToProxy);
    connect(server,&QTcpSocket::disconnected,this,&ForwardData::deleteLater);
    connect(client,&QTcpSocket::disconnected,this,&ForwardData::deleteLater);
}

void ForwardData::writeToProxy()
{
    this->server->write(this->client->readAll());
}

void ForwardData::writeToClient()
{
    this->client->write(this->server->readAll());
}
