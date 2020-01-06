#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringList>
#include "Command.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QTcpSocket     *socket = nullptr;
    
    bool isLocalMode = false;
    QString serverIP ="127.0.0.1";
    int     serverPort = 80;
    
    
private slots:
    void ButtonModeChange();//客户端修改信息
    void ButtonApplyInfo(); //客户端修改后重新连接服务器
    void SocketProcess();   //处理服务器发回来的数据
    void AddonlineInfo(QString id,QString ip,QString port);  //在界面添加新的提供者信息
    void RemoveofflineInfo(int id);                          //移除界面中已有的提供者
};
#endif // MAINWINDOW_H
