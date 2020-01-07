#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QVector>
#include <QMap>
#include "thread.h"
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();
    bool startServer(int port , const QHostAddress& address = QHostAddress::AnyIPv4);
    void stopServer();

protected slots:
    void incomingConnection(qintptr handle) override; 
    
private slots:
    void getBackToClientWillConnectPort(int id,QString ip,int port);
    void getBackToClientOffLine(int id);
    void guiConnectToServer();
    void sendMessageToAllClient(QByteArray byte);
    void sendCurrentOfferToNewClient(QTcpSocket* socket);
    QString formatOnlineInfo(int id,QString ip,int port);  //格式化登陆协议的字符串
private:
    struct infoStruct
    {
        int id;
        QString ip;
        int port;
    };
    QTcpServer* guiServer = nullptr;
    QMap<int,infoStruct> infoStructElement;
    QVector<QTcpSocket*> guiElement;
    QVector<QTcpSocket*>::Iterator guiElementIterator;

};

#endif // SERVER_H
