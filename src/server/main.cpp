#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QByteArray arg1;
    QByteArray arg2="80";
    
    if(argc < 3)
    {
        arg1 = argv[1];
        return 0;
    }
    if(argc <4 )
    {
        arg2 = argv[2];
    }

    Server::STARTSTYLE style;
    if(arg1 == "local")
    {
        style = Server::STARTSTYLE::LOCAL;
    }
    
    if(arg1 == "ipv4")
    {
        style = Server::STARTSTYLE::IPV4;
    }
    
    if(arg1 == "ipv6")
    {
        style = Server::STARTSTYLE::IPV6;
    }
    
    Server server(style,arg2.toInt());
    server.startServer(1611);
    return a.exec();
}
