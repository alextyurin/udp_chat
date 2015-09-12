#include <chrono>
#include <thread>
#include <QHostAddress>
#include <QUdpSocket>
#include "../message_interface.hpp"
#include "client.hpp"

namespace udp_chat
{
namespace client
{

Client::Client(const QHostAddress &server_address, const quint16 server_port, const quint16 port, QObject *parent):
    QObject(parent),
    m_server_address(server_address),
    m_server_port(server_port),
    m_socket(new QUdpSocket(parent))
{
    m_socket->bind(port);
}

Client::~Client()
{
}

void Client::start()
{
    qDebug() << "Client started...";
    send_connection_msg();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void Client::send_connection_msg()
{
    QByteArray datagram;
    const quint32 nickname_length = m_nickname.size();
    datagram.resize(sizeof(query::connection_query_desc_s) + sizeof(quint32) + sizeof(char) * nickname_length);
    query::connection_query_desc_s desc;
    memcpy(datagram.data(), &desc, sizeof(query::connection_query_desc_s));
    memcpy(datagram.data() + sizeof(query::connection_query_desc_s), &nickname_length, sizeof(quint32));
    memcpy(datagram.data() + sizeof(query::connection_query_desc_s) + sizeof(quint32), &(m_nickname[0]), nickname_length);
    m_socket->writeDatagram(datagram, m_server_address, m_server_port);
}

void Client::set_nickname(const QString &nickname)
{
    m_nickname = nickname;
}

void Client::readPendingDatagrams()
{
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;
    while (m_socket->hasPendingDatagrams())
    {
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        answer::answer_desc_s desc;
        memcpy(&desc, datagram.data(), sizeof(answer::answer_desc_s));
        if (answer::answer_signature == desc.signature)
        {
            if (sender == m_server_address)
            {
                if(answer::AnswerType::CONNECTED == desc.type)
                {
                    qDebug() << "Connected!";
                }
            }
        }
    }
}

} //namespace client
} //namespace udp_chat
