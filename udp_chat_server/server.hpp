#pragma once

#include <QObject>
#include <QMap>
#include <QHostAddress>
#include <QString>
#include <QThread>
#include <QMutex>
#include "../common/datagram.hpp"
#include "../common/message_interface.hpp"

class QUdpSocket;
class QByteArray;

namespace udp_chat
{
namespace server
{

struct user_s
{
    QString nickname;
    bool checked;
}; //struct user_s

typedef QMap<user_desc_s, user_s> users_t;

class Server : public QObject
{
    Q_OBJECT
public:
    Server(const quint16 port, QObject *parent = nullptr);
    virtual ~Server();
    void start();
    void send_check_connection_query();
private slots:
    void listen();
private:
    void send_connected_answer(const QHostAddress &address, const quint16 port);
    void send_user_online_answer(user_desc_s &user_desc, const user_s &user, const QHostAddress &address, const quint16 port);
    void send_user_offline_answer(user_desc_s &user_desc, const QHostAddress &address, const quint16 port);
    void send_message_answer(const QString &msg, const user_desc_s &user_desc, const QHostAddress &address, const quint16 port);
    void send_private_message_answer(const QString &msg, const user_desc_s &src, const user_desc_s &dst, const QHostAddress &address, const quint16 port);
    void process_connection_query(Datagram &data, const QHostAddress &address, const quint16 port);
    QString process_message_query(Datagram &data);
    void process_private_message_query(Datagram &data, const user_desc_s &sender);
    void read_datagram(QByteArray &byte_array, const QHostAddress &address, const quint16 port);
    void send_descriptor(desc_s &desc, const QHostAddress &address, const quint16 port);
    void print_msg(const QString &msg);
    users_t m_users;
    QUdpSocket *m_socket;
    QMutex m_mutex;
};

class CheckConnection : public QThread
{
    Q_OBJECT
public:
    explicit CheckConnection(Server *server):QThread(server), m_server(server) {}
    virtual ~CheckConnection() {}
private:
    void run()
    {
        while (true)
        {
            m_server->send_check_connection_query();
            QThread::sleep(1);
        }
    }
    Server *m_server;
};

} //namespace server
} //namespace udp_chat
