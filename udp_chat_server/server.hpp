#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QHostAddress>

class QUdpSocket;
class QByteArray;

namespace udp_chat
{
namespace server
{

class Server : public QObject
{
    Q_OBJECT
public:
    struct User
    {
        QString nickname;
        QHostAddress address;
    };
    enum MessageType
    {
        MT_MESSAGE,
        MT_ONLINE,
        MT_PRIVATE_MESSAGE
    };
    struct ServerMessage
    {
        MessageType type;
        QByteArray data;
    };
    Server(const unsigned short port, QObject *parent = nullptr);
    virtual ~Server();
    void start();
private slots:
    void readPendingDatagrams();
private:
    QVector <User> m_users;
    QUdpSocket *m_socket;
    unsigned short m_port;
};


} //namespace server
} //namespace udp_chat
