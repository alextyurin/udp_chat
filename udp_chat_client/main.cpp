#include "mainwindow.h"
#include <QApplication>
#include <QHostAddress>
#include "client.hpp"

namespace
{
    const QHostAddress server_address = QHostAddress::LocalHost;
    const unsigned short server_port = 10000;
    const unsigned short port = 11111;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    udp_chat::client::Client client(server_address, server_port, port);
    client.set_nickname("Alex");
    client.start();
    return a.exec();
}
