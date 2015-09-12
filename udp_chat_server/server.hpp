#pragma once

#include <QObject>
#include <QVector>
#include <QHostAddress>
#include <QString>
#include "../message_interface.hpp"

class QUdpSocket;
class QByteArray;

namespace udp_chat
{
namespace server
{

struct user_s
{
    QHostAddress address;
    quint16 port;
    QString nickname;
}; //struct user_s

typedef QVector<user_s> users_t;

class Server : public QObject
{
    Q_OBJECT
public:
    Server(const quint16 port, QObject *parent = nullptr);
    virtual ~Server();
    void start();
    void send_connected_msg(const QHostAddress &address, const quint16 port);
private slots:
    void readPendingDatagrams();
private:
    users_t m_users;
    QUdpSocket *m_socket;
};


} //namespace server
} //namespace udp_chat
