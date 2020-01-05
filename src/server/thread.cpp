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
        forward->deleteLater();
    }
}

void Thread::run()
{
    qDebug()<<"new Proxy "<<this->currentThreadId();
    forward = new Forward(this->socketID);
    forward->init();
    connect(forward,&Forward::finsh,this,&Thread::exit,Qt::DirectConnection);
    this->exec();
}
