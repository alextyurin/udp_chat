#include "mainwindow.h"
#include <QApplication>
#include <QHostAddress>
#include "client.hpp"

namespace
{
    const QHostAddress address = QHostAddress::LocalHost;
    const unsigned short port = 10000;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    udp_chat::client::Client client(address, port);
    client.start();
    return a.exec();
}
