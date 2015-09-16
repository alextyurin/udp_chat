#pragma once
#include <QObject>
#include <QHostAddress>
#include <QMap>
#include <QThread>
#include <QMutex>
#include "../common/datagram.hpp"
#include "../common/message_interface.hpp"
#include "mainwindow.h"

class QUdpSocket;
class QByteArray;

namespace udp_chat
{
namespace client
{

typedef QMap<user_desc_s, QString> users_t;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QHostAddress &server_address, const quint16 server_port, const quint16 port, QObject *parent = nullptr);
    virtual ~Client();
    void start();
    void set_nickname(const QString &nickname);
    bool send_check_connection_query();
signals:
    void show_message(const QString &msg);
    void user_online(const QString &nickname);
    void user_offline(const QString &nickname);
private slots:
    void listen();
    void send_message_query(const QString &msg);
private:
    void read_datagram(QByteArray &byte_array, const QHostAddress &address, const quint16 port);
    void process_user_online_answer(Datagram &data);
    void process_user_offline_answer(Datagram &data);
    void process_message_answer(Datagram &data);
    void send_connection_query();
    void send_who_is_online_query();
    void send_check_connection_answer();
    void send_descriptor(desc_s desc);
    users_t m_users;
    QUdpSocket *m_socket;
    QHostAddress m_server_address;
    QString m_nickname;
    quint16 m_server_port;
    QMutex m_mutex;
    bool m_connected;
};

class CheckConnection : public QThread
{
    Q_OBJECT
public:
    explicit CheckConnection(Client *client):QThread(client), m_client(client) {}
    virtual ~CheckConnection() {}
private:
    void run()
    {
        while (true)
        {
            if (!m_client->send_check_connection_query())
            {
                qDebug() << "Connection lost...";
                return;
            }
            QThread::sleep(1);
        }
    }
    Client *m_client;
};

} //namespace client
} //namespace udp_chat
