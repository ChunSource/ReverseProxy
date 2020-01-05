#ifndef FORWARD_H
#define FORWARD_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "forwarddata.h"
#include "Command.h"
class Forward : public QObject
{
    Q_OBJECT
public:
    explicit Forward(int id,QObject *parent = nullptr);
    ~Forward();
    void init();
signals:
    void finsh(int id = 0);
    void clientWillConnectPort(QString ip,int port); //返回父对象我们供客户连接的端口，让客户端显示出来
private:
    QTcpSocket* proxyServer = nullptr; 
    QTcpServer* forwardServer = nullptr;  //监听端口供客户端连接进来
    QTcpServer* listenOffer = nullptr;;
    int socketID = 0;
    int serverPort = 0;
    int listenOfferPort = 0;
    QVector<QTcpSocket*> vector;
    QVector<QTcpSocket*> clientList;
private slots:
    void commandGetnewOffer();
    void newOffer();
    void newConnect();
};

#endif // FORWARD_H
