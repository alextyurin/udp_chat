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

Server::Server(const quint16 port, QObject *parent):
    QObject(parent),
    m_socket(new QUdpSocket(this))
{
    m_socket->bind(QHostAddress::Any, port);
}

Server::~Server()
{

}

void Server::readPendingDatagrams()
{   
    QByteArray byte_array;
    QHostAddress sender;
    quint16 sender_port;
    user_s user;
    //listen clients
    while (m_socket->hasPendingDatagrams())
    {
        qDebug() << 1;
        byte_array.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(byte_array.data(), byte_array.size(), &sender, &sender_port);

        Datagram data(byte_array.data(), byte_array.size());

        query::query_desc_s desc;
        data.read(&desc, sizeof(query::query_desc_s));

        //char *ptr = datagram.data();
        //memcpy(&desc, ptr, sizeof(query::query_desc_s));
        if (query::query_signature == desc.signature)
        {
            if(query::QueryType::CONNECTION == desc.type)
            {

                quint32 nickname_length;
                data.read(&nickname_length, sizeof(quint32));
                QString nickname;
                nickname.resize(nickname_length);
                data.read(&(nickname[0]), nickname_length);
                ////ptr += sizeof(query::connection_query_desc_s);
                ////memcpy(&nickname_length, ptr, sizeof(quint32));
                ////ptr += sizeof(quint32);
                ////QString nickname(nickname_length);
               //// memcpy(&nickname_length, ptr, nickname_length);

                std::cout << "User " << nickname.toStdString() << " from " << sender.toString().toStdString() << "::" << sender_port << " connects..." << std::endl;

                user.address = sender;
                user.port = sender_port;
                user.nickname = nickname;

                m_users.push_back(user);
                send_connected_msg(sender, sender_port);
            }
        }
    }
}

void Server::send_connected_msg(const QHostAddress &address, const quint16 port)
{
    QByteArray datagram;
    datagram.resize(sizeof(answer::connected_answer_desc_s));
    answer::connected_answer_desc_s desc;
    memcpy(datagram.data(), &desc, sizeof(answer::connected_answer_desc_s));
    m_socket->writeDatagram(datagram, address, port);
}

void Server::start()
{
    std::cout << "Server started..." << std::endl;
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

} //namespace server
} //namespace udp_chat
