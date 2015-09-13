#include <iostream>
#include <QCoreApplication>
#include "server.hpp"
#include "../common/exception.hpp"

namespace
{
const int port = 10000;
} //anonymous namespace

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    udp_chat::server::Server server(port);
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
