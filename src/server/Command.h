#ifndef COMMAND_H
#define COMMAND_H
#include <QByteArray>
const QByteArray Command_LOGIN="FORWARDLOGIN";     //登陆认证
const QByteArray Command_NEWHAND="FORWARDNEWHAND"; //让服务这新连接一个句柄供客户使用
const QByteArray Command_OUTOFSTOCK="OUT_OF_STOCK";//让客户知道现在是拥堵状态
const QByteArray Command_OFFERPORT="OFFERPORT";
const QByteArray Command_OFFLINE="OFFLINE";
const QByteArray Command_ONLINE="ONLINE";
#endif // COMMAND_H
