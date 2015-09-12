#pragma once
#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QByteArray;

namespace udp_chat
{
namespace client
{

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QHostAddress &server_address, const quint16 server_port, const quint16 port, QObject *parent = nullptr);
    virtual ~Client();
    void start();
    void send_connection_msg();
    void set_nickname(const QString &nickname);
private slots:
    void readPendingDatagrams();
private:
    QUdpSocket *m_socket;
    QHostAddress m_server_address;
    QString m_nickname;
    quint16 m_server_port;
};

} //namespace client
} //namespace udp_chat
