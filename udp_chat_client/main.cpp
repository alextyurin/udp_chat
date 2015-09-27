#include "mainwindow.h"
#include <QApplication>
#include <QHostAddress>
#include <QTime>
#include <QTimer>
#include "client.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    udp_chat::client::Client client;
    MainWindow gui;
    gui.show();
    QObject::connect(&gui, SIGNAL(send_msg(QString)), &client, SLOT(send_message_query(QString)));
    QObject::connect(&gui, SIGNAL(set_nickname(QString)), &client, SLOT(set_nickname(QString)));
    QObject::connect(&gui, SIGNAL(disconnect()), &client, SLOT(stop()));
    QObject::connect(&gui, SIGNAL(try_to_connect(QHostAddress, quint16)), &client, SLOT(start(QHostAddress,quint16)));
    QObject::connect(&gui, SIGNAL(send_private_msg(quint32, quint16, QString)), &client, SLOT(send_private_message_query(quint32, quint16, QString)));
    QObject::connect(&client, SIGNAL(show_message(QString)), &gui, SLOT(recieve_msg(QString)));
    QObject::connect(&client, SIGNAL(status(QString)), &gui, SLOT(show_status(QString)));
    QObject::connect(&client, SIGNAL(user_online(quint32, quint16)), &gui, SLOT(append_user(quint32, quint16)));
    QObject::connect(&client, SIGNAL(user_offline(quint32, quint16)), &gui, SLOT(remove_user(quint32, quint16)));
    QObject::connect(&client, SIGNAL(connected()), &gui, SLOT(enable()));
    QObject::connect(&client, SIGNAL(disconnected()), &gui, SLOT(disable()));

    return a.exec();
}
