#include <iostream>
#include <QCoreApplication>
#include "server.hpp"
#include "../common/exception.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc < 2)
    {
        std::cout << "Wrong command. Usage: udp_chat_server [port]" << std::endl;
        return 0;
    }
    udp_chat::server::Server server(static_cast<quint16>(atoi(argv[1])));
    try
    {   
        server.start();
    }
    catch (udp_chat::exception &ex)
    {
        std::cout << "E:"<< ex.what() << std::endl;
    }
    return a.exec();
}
