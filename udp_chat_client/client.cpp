#include <chrono>
#include <thread>
#include <QHostAddress>
#include <QUdpSocket>
#include "client.hpp"

namespace udp_chat
{
namespace client
{

Client::Client(const QHostAddress &address, const unsigned short port, QObject *parent):
    QObject(parent),
    m_address(address),
    m_port(port),
    m_socket(new QUdpSocket(parent))
{
}

Client::~Client()
{
}

void Client::start()
{
    qDebug() << "Client started...";
    m_socket->bind(m_address, m_port);
    QByteArray data;
    data.append("Msg from client");
    m_socket->writeDatagram(data, m_address, m_port);
    qDebug() << "msg sended...";
    connect(m_socket, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
}

void Client::readPendingDatagrams()
{
    qDebug() << "Connected...";
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size());
        qDebug() << datagram.data();
    }
}

} //namespace client
} //namespace udp_chat
