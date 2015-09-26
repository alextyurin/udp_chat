#include "mainwindow.h"
#include <QApplication>
#include <QHostAddress>
#include <QTime>
#include <QTimer>
#include "client.hpp"

namespace
{
    const QHostAddress server_address = QHostAddress::LocalHost;
    const unsigned short server_port = 10000;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    udp_chat::client::Client client(server_address, server_port);
    MainWindow gui;
    gui.show();
    QObject::connect(&gui, SIGNAL(send_msg(QString)), &client, SLOT(send_message_query(QString)));
    QObject::connect(&gui, SIGNAL(send_private_msg(quint32, quint16, QString)), &client, SLOT(send_private_message_query(quint32, quint16, QString)));
    QObject::connect(&client, SIGNAL(show_message(QString)), &gui, SLOT(recieve_msg(QString)));
    QObject::connect(&client, SIGNAL(status(QString)), &gui, SLOT(show_status(QString)));
    QObject::connect(&client, SIGNAL(user_online(quint32, quint16)), &gui, SLOT(append_user(quint32, quint16)));
    QObject::connect(&client, SIGNAL(user_offline(quint32, quint16)), &gui, SLOT(remove_user(quint32, quint16)));
    QObject::connect(&client, SIGNAL(connected()), &gui, SLOT(enable()));
    QObject::connect(&client, SIGNAL(disconnected()), &gui, SLOT(disable()));
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    QString nick = "User" + QString::number(qrand() % 10000);
    client.set_nickname(nick);
    client.start();

    return a.exec();
}
