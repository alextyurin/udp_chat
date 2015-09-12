#include <exception>
#include <iostream>
#include <QCoreApplication>
#include "server.hpp"

namespace
{
const int port = 10000;
} //anonymous namespace

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //try
    {
        udp_chat::server::Server server(port);
        server.start();
    }
    /*catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }*/
    return a.exec();
}
