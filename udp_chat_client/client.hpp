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
    explicit Client(const QHostAddress &address, const unsigned short port, QObject *parent = nullptr);
    virtual ~Client();
    void start();
private slots:
    void readPendingDatagrams();
private:
    QUdpSocket *m_socket;
    QHostAddress m_address;
    unsigned short m_port;
};

} //namespace client
} //namespace udp_chat
