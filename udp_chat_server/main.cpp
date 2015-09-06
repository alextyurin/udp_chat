#include <QCoreApplication>
#include "server.hpp"

namespace
{
const int port = 10000;
} //anonymous namespace

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    udp_chat::server::Server server(port);
    server.start();
    return a.exec();
}
