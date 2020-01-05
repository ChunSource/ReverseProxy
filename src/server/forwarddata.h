#ifndef FORWARDDATA_H
#define FORWARDDATA_H

#include <QObject>
#include <QTcpSocket>

class ForwardData : public QObject
{
    Q_OBJECT
public:
    explicit ForwardData(QTcpSocket* p,QTcpSocket* c, QObject *parent = nullptr);
    ~ForwardData();
    void forward();
signals:
    
private slots:
    void writeToProxy();
    void writeToClient();
private:
    QTcpSocket* server = nullptr;
    QTcpSocket* client = nullptr;
};

#endif // FORWARDDATA_H
