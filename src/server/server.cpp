#include "server.h"
#include <QDebug>

Server::Server()
{
    guiServer = new QTcpServer();
    guiServer->listen(QHostAddress::AnyIPv4,80);
    connect(guiServer,&QTcpServer::newConnection,this,&Server::guiConnectToServer);
}

Server::~Server()
{
    guiServer->close();
    guiServer->deleteLater();
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

void Server::getBackToClientWillConnectPort(int id, QString ip, int port) //发送给所有的客户端，我们已有的提供者信息
{
    infoStruct info;
    info.id = id;
    info.ip = ip;
    info.port = port;
    infoStructElement.insert(id,info);
    
    QString result = formatOnlineInfo(id,ip,port);
    sendMessageToAllClient(result.toUtf8());
}

void Server::getBackToClientOffLine(int id)  //告诉所有客户端，断开连接的提供者信息
{
    infoStructElement.remove(id);
    QString result = Command_OFFLINE;
    result += QString::number(id);
    sendMessageToAllClient(result.toUtf8());
}

void Server::guiConnectToServer()   //客户端连接到服务器之后
{
    QTcpSocket *socket = this->guiServer->nextPendingConnection();
    guiElement.push_back(socket);   //保存客户端的socket*
    sendCurrentOfferToNewClient(socket);  //发送所有当前的提供者信息给客户端
    
    connect(socket,&QTcpSocket::readyRead,this,[=](){
        if(socket->readAll().indexOf(Command_LOGIN) ==-1) //验证协议
        {
            socket->disconnectFromHost();
            socket->deleteLater();
            guiElement.removeOne(socket);
        }
    });
    
    connect(socket,&QTcpSocket::disconnected,this,[=](){ //处理断开
            socket->disconnectFromHost();
            socket->deleteLater();
            guiElement.removeOne(socket);
    });
    
}

void Server::sendMessageToAllClient(QByteArray byte) //给所有客户端发送信息
{
    qDebug()<<"[*]---send: "<<byte;
    for (guiElementIterator=guiElement.begin();guiElementIterator!=guiElement.end();guiElementIterator++)  
    {  
        QTcpSocket* p = static_cast<QTcpSocket*>(*guiElementIterator);
        if(p!=nullptr)
        {
            p->write(byte);
        }
    } 
}

void Server::sendCurrentOfferToNewClient(QTcpSocket *socket)  //发送所有当前的提供者信息给客户端
{

    QMapIterator<int,infoStruct> i(this->infoStructElement);
    while (i.hasNext()) {
        i.next();
        QString result = formatOnlineInfo(i.value().id,i.value().ip,i.value().port);
        socket->write(result.toUtf8());
    }
    
}

QString Server::formatOnlineInfo(int id, QString ip, int port)  //格式化我们的信息
{
    QString result = Command_ONLINE;
    result += QString::number(id);
    result += "#";
    result += ip;
    result +="#";
    result +=QString::number(port);
    return result;
}


