#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    
    InitGui();
    connect(ui->ButtonApplyChange,&QPushButton::clicked,this,&MainWindow::ButtonApplyInfo);
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::SocketProcess);
    connect(socket,&QTcpSocket::disconnected,this,[=](){
       ui->ButtonApplyChange->setEnabled(1); 
       QMessageBox::information(this,tr("警告"),tr("服务器断开连接"));
    });
    //服务器信息修改
    connect(ui->LineServerPort,&QLineEdit::textChanged,this,[=]{
        ui->ButtonApplyChange->setEnabled(true);
        this->serverPort = ui->LineServerPort->text().toInt();
    });
    connect(ui->LineServerAddress,&QLineEdit::textChanged,this,[=]{
        ui->ButtonApplyChange->setEnabled(true);
        this->serverIP = ui->LineServerAddress->text();
    });
}

MainWindow::~MainWindow()
{
    socket->disconnectFromHost();
    socket->deleteLater();
    menu->clear();
    menu->close();
    delete menu;
    delete ui;
}

void MainWindow::InitGui()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //设置自适应列宽
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);   //设置一次只能选中一行
    ui->tableWidget->installEventFilter(this);
    //右键菜单
    menu = new QMenu(this);
    QAction* useAction = menu->addAction("使用");
    menu->addAction(useAction);
    connect(useAction,&QAction::triggered,this,&MainWindow::UseChoeseOffer);
}

void MainWindow::ButtonModeChange()
{
    ui->ButtonApplyChange->setEnabled(true);
    if(isLocalMode) //切换为远程模式
    {
        ui->ButtonLocalMode->setText("本地模式");
        ui->LineServerPort->setEnabled(true);
        ui->LineServerAddress->setEnabled(true);
        isLocalMode = false;
    }else  //切换为本地模式
    {
        ui->ButtonLocalMode->setText("远程模式");
        ui->LineServerPort->setDisabled(true);
        ui->LineServerAddress->setDisabled(true);
        isLocalMode = true;
    }
}

void MainWindow::ButtonApplyInfo()  //服务器信息更改，重新连接
{
    qDebug()<<"[*]---服务器IP: "<<this->serverIP;
    qDebug()<<"[*]---服务器端口: "<<this->serverPort;
    ui->ButtonApplyChange->setDisabled(true);  //修改之后将按钮关闭
    socket->disconnectFromHost();              //关闭原有的连接
    
    
    if(isLocalMode)
    {
        qDebug()<<"[*]---本地模式";
        socket->connectToHost(QHostAddress::LocalHost,80);
    }
    else
    {
        qDebug()<<"[*]---远程模式";
        socket->connectToHost(serverIP,serverPort);
    }
    if(socket->waitForConnected())
    {
        socket->write(Command_LOGIN); //发送登陆协议信息
    }
    else
    {
        QMessageBox::information(this,tr("警告"),tr("无法连接到服务器"));
        ui->ButtonApplyChange->setEnabled(true);
    }
    
}

void MainWindow::SocketProcess()
{
    QString buff =  socket->readAll();
    qDebug()<<"[*]---"<<buff;
    if(buff.indexOf(Command_ONLINE) != -1) //有上线消息
    {
        buff.replace(Command_ONLINE,"");
        QStringList list = buff.split("#");
        
        if(list.size() != 3)  //再次检查发来的数据是否符合
        {
            return;
        }
        
        AddonlineInfo(list.at(0),list.at(1),list.at(2));
    }
    if(buff.indexOf(Command_OFFLINE) != -1)   //有提供者下线
    {
        buff.replace(Command_OFFLINE,"");
        RemoveofflineInfo(buff.toInt());
    }
    
}

void MainWindow::AddonlineInfo(QString id, QString ip, QString port)
{
    int column = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(column);
    QTableWidgetItem* idItem = new QTableWidgetItem(id);
    QTableWidgetItem* ipItem = new QTableWidgetItem(ip);
    QTableWidgetItem* portItem = new QTableWidgetItem(port);
    
    ui->tableWidget->setItem(column,0,idItem);
    ui->tableWidget->setItem(column,1,ipItem);
    ui->tableWidget->setItem(column,2,portItem);
}

void MainWindow::RemoveofflineInfo(int id)
{
    qDebug()<<"[*]---id: "<<id;
    for (int i=0;i<ui->tableWidget->rowCount();i++) {
         qDebug()<<"[*]---item(i,0)->text() "<<ui->tableWidget->item(i,0)->text().toInt();
        if(id == ui->tableWidget->item(i,0)->text().toInt())
        {
            ui->tableWidget->removeRow(i);
        }
    }
}

void MainWindow::UseChoeseOffer()
{
    
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==(QObject*)ui->tableWidget) {
            if (event->type() == QEvent::ContextMenu) {
                menu->exec(QCursor::pos());
            }
        }
     
        return QObject::eventFilter(watched, event);
}

