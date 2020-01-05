#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "forward.h"

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(int id,QObject *parent = nullptr);
    ~Thread();
    void run() override;
private:
    int socketID = 0;
    Forward* forward = nullptr;
signals:
    void SIGNAL_ClientWillConnectPort(int id,QString ip,int port);  //发送客户连接用的端口
    void SIGNAL_Finsh(int id);
private slots:
    void SLOT_ClientWillConnectPort(QString ip,int port);
};

#endif // THREAD_H
