#ifndef FORWARD_H
#define FORWARD_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "forwarddata.h"
class Forward : public QObject
{
    Q_OBJECT
public:
    explicit Forward(int id,QObject *parent = nullptr);
    ~Forward();
    void init();
    const QByteArray Command_LOGIN="FORWARDLOGIN";     //登陆认证
    const QByteArray Command_NEWHAND="FORWARDNEWHAND"; //让服务这新连接一个句柄供客户使用
    const QByteArray Command_OUTOFSTOCK="OUT_OF_STOCK";
signals:
    void finsh(int id = 0);
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
    void leave();  //清洁函数
};

#endif // FORWARD_H
