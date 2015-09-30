#pragma once
#include <QObject>
#include <QHostAddress>
#include <QMap>
#include <QThread>
#include <QMutex>
#include "userlist.hpp"
#include "../common/datagram.hpp"
#include "mainwindow.h"

struct desc_s;
class QUdpSocket;
class QByteArray;

namespace udp_chat
{
namespace client
{

class CheckConnection;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client();
    bool send_check_connection_query();
    void send_connection_query();
    void lock() { m_mutex.lock(); }
    void unlock() { m_mutex.unlock(); }
signals:
    void show_message(const QString &msg);
    void status(const QString &msg);
    void user_online(const quint32 address, const quint16 port);
    void user_offline(const quint32 address, const quint16 port);
    void connected();
    void disconnected();
public slots:
    void set_nickname(const QString &nickname);
    void start(const QHostAddress &server_address, const quint16 server_port);
    void stop();
private slots:
    void listen();
    void send_message_query(const QString &msg);
    void send_private_message_query(const quint32 ip, const quint16 port, const QString &msg);
private:
    void read_datagram(QByteArray &byte_array);
    void process_user_online_answer(Datagram &data);
    void process_user_offline_answer(Datagram &data);
    void process_message_answer(Datagram &data);
    void process_private_message_answer(Datagram &data);
    void send_who_is_online_query();
    void send_check_connection_answer();
    void send_descriptor(desc_s desc);
    QUdpSocket *m_socket;
    QHostAddress m_server_address;
    QString m_nickname;
    quint16 m_server_port;
    QMutex m_mutex;
    CheckConnection *m_checker;
    bool m_checked;
    bool m_connected;
};

class CheckConnection : public QThread
{
    Q_OBJECT
public:
    explicit CheckConnection(Client *client):QThread(client), m_client(client) {}
    virtual ~CheckConnection() {}
public slots:
    void run()
    {
        while (true)
        {
            if (!m_client->send_check_connection_query())
            {
                m_client->status("Connection lost...");
                m_client->disconnected();
                return;
            }
            QThread::sleep(1);
        }
    }
private:
    Client *m_client;
};

} //namespace client
} //namespace udp_chat
