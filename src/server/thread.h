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
};

#endif // THREAD_H
