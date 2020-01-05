#include "forward.h"
#include <QDebug>
Forward::Forward(int id, QObject *parent) : QObject(parent)
{
    this->socketID =id;
}

Forward::~Forward()
{
    qDebug()<<"Finsh Forward";
    emit finsh();
    if(this->proxyServer != nullptr)
    {
        this->proxyServer->close();
        this->proxyServer->deleteLater();
    }
    if(this->forwardServer != nullptr)
    {
        this->forwardServer->close();
        this->forwardServer->deleteLater();
    }
    if(this->listenOffer != nullptr)
    {
        this->listenOffer->close();
        this->listenOffer->deleteLater();
    }
    
}

void Forward::init()
{
    this->proxyServer = new QTcpSocket();
    if(!proxyServer->setSocketDescriptor(this->socketID)) //绑定socket对象
    {
        this->deleteLater();
        return;
    }
    
    //监听新进来的提供者
    listenOffer = new QTcpServer();
    listenOffer->listen(QHostAddress::AnyIPv4);
    listenOfferPort = listenOffer->serverPort();
    connect(listenOffer,&QTcpServer::newConnection,this,&Forward::newOffer);
    qDebug()<<"[*]---offer port"<<listenOfferPort;
    //开启服务，并记录系统随机监听的端口
    this->forwardServer = new QTcpServer();
    this->forwardServer->listen(QHostAddress::AnyIPv4);
    this->serverPort = this->forwardServer->serverPort(); //记录端口
    qDebug()<<"[*]---client port"<<serverPort;
    connect(this->forwardServer,&QTcpServer::newConnection,this,&Forward::newConnect);
    connect(this->proxyServer,&QTcpSocket::disconnected,this,&Forward::deleteLater);
}

void Forward::commandGetnewOffer()
{
    qDebug()<<"[*]---commandGetnewOffer";
    //请求一个新的offer连接到我们的服务器，预备下一个客户
    this->proxyServer->write(Command_NEWHAND);
}

void Forward::newOffer()
{
    QTcpSocket* socket = listenOffer->nextPendingConnection();
    vector.push_back(socket);
    
    if(!clientList.isEmpty()) //通过判断用户表 判断当期是否网络拥塞状态
    {
        ForwardData *forwardData = new ForwardData(vector.first(),clientList.first());
        vector.pop_front();
        clientList.pop_front();
        forwardData->forward();
    }
}

void Forward::newConnect()  //有新的客户进来了
{
    qDebug()<<"[*]---new Client";
    QTcpSocket* socket = this->forwardServer->nextPendingConnection(); //获取新进来的对象
    commandGetnewOffer();
    
    if(vector.isEmpty() == false)
    {
        ForwardData *forwardData = new ForwardData(vector.first(),socket);
        forwardData->forward();
        vector.pop_front();
    }
    else
    {
        socket->write(Command_OUTOFSTOCK); //让对方得知我们正在等待新的提供者
        clientList.push_back(socket);
    }
}
/*
void Forward::leave()  //释放内存的
{
    
}
*/
