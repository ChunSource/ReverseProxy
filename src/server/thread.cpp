#include "thread.h"
#include <QDebug>

Thread::Thread(int id, QObject *parent) : QThread(parent),socketID(id)
{
    
}

Thread::~Thread()
{
    qDebug()<<"[*]---exit Thread";
    if(forward!=nullptr)
    {
    //    forward->deleteLater();
    }
}

void Thread::run()
{
    qDebug()<<"new Proxy "<<this->currentThreadId();
    forward = new Forward(this->socketID);
    connect(forward,&Forward::finsh,this,[=](){
        qDebug()<<"[*]---Thread deleteLater";
        emit SIGNAL_Finsh(this->socketID);  //告诉父对象我们退出了，让他在客户端列表中除去我们
        this->quit();
        this->wait();
        this->deleteLater();
    },Qt::DirectConnection);
    connect(forward,&Forward::clientWillConnectPort,this,&Thread::SLOT_ClientWillConnectPort,Qt::DirectConnection);
    forward->init();
    this->exec();
}

void Thread::SLOT_ClientWillConnectPort(QString ip, int port)
{
    emit SIGNAL_ClientWillConnectPort(this->socketID,ip,port);  //返回上层我们的监听端口，给客户选择
}
