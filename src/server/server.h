#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "thread.h"
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    bool startServer(int port , const QHostAddress& address = QHostAddress::AnyIPv4);
    void stopServer();

protected slots:
    void incomingConnection(qintptr handle) override; 
};

#endif // SERVER_H
