#include "mainwindow.h"
#include <QApplication>
#include <QHostAddress>
#include <QTime>
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
    MainWindow gui;
    gui.show();
    QObject::connect(&gui, SIGNAL(send_msg(QString)), &client, SLOT(send_message_query(QString)));
    QObject::connect(&client, SIGNAL(show_message(QString)), &gui, SLOT(recieve_msg(QString)));
    QObject::connect(&client, SIGNAL(user_online(QString)), &gui, SLOT(append_user(QString)));
    QObject::connect(&client, SIGNAL(user_offline(QString)), &gui, SLOT(remove_user(QString)));
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    QString nick = "User" + QString::number(qrand() % 10000);
    client.set_nickname(nick);
    client.start();   
    return a.exec();
}
