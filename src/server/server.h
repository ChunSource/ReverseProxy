#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QVector>
#include "thread.h"
class Server : public QTcpServer
{
    Q_OBJECT
public:
    enum STARTSTYLE{LOCAL,IPV4,IPV6};
    Server(const STARTSTYLE& style,int localPort=80);
    bool startServer(int port , const QHostAddress& address = QHostAddress::AnyIPv4);
    void stopServer();

protected slots:
    void incomingConnection(qintptr handle) override; 
    
private slots:
    void getBackToClientWillConnectPort(int id,QString ip,int port);
    void getBackToClientOffLine(int id);
    void guiConnectToServer();
    void sendMessageToAllClient(QByteArray byte);
private:
    QTcpServer* guiServer = nullptr;
    QVector<QTcpSocket*> guiElement;
    QVector<QTcpSocket*>::Iterator guiElementIterator;
};

#endif // SERVER_H
