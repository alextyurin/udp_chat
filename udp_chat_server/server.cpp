#include <iostream>
#include <chrono>
#include <thread>
#include <QByteArray>
#include <QHostAddress>
#include <QUdpSocket>
#include "server.hpp"

namespace udp_chat
{
namespace server
{

Server::Server(const unsigned short port, QObject *parent):
    QObject(parent),
    m_port(port),
    m_socket(new QUdpSocket(this))
{

}

Server::~Server()
{

}

void Server::readPendingDatagrams()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        qDebug() << datagram;
        m_socket->writeDatagram(QByteArray("Msg from server"), sender, senderPort);
    }
}

void Server::start()
{
    std::cout << "Server started..." << std::endl;
    m_socket->bind(QHostAddress::Any, m_port);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

} //namespace server
} //namespace udp_chat
