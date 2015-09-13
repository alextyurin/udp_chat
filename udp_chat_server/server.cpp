#include <iostream>
#include <QByteArray>
#include <QUdpSocket>

#include "server.hpp"

namespace
{
    using udp_chat::user_desc_s;
    bool operator<(const user_desc_s &lhs, const user_desc_s &rhs)
    {
        if (lhs.ip == rhs.ip)
        {
            return lhs.port < rhs.port;
        }
        return lhs.ip < rhs.ip;
    }
} // anonymous namespace

namespace udp_chat
{
namespace server
{

Server::Server(const quint16 port, QObject *parent):
    QObject(parent),
    m_socket(new QUdpSocket(this)),
    m_mutex()
{
    m_socket->bind(port);
}

Server::~Server()
{

}


void Server::listen()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 sender_port;
        m_socket->readDatagram(datagram.data(), datagram.size(), &sender, &sender_port);
        read_datagram(datagram, sender, sender_port);
    }
}

void Server::send_descriptor(desc_s desc, const QHostAddress &address, const quint16 port)
{
    QByteArray datagram;
    datagram.resize(sizeof(desc_s));
    memcpy(datagram.data(), &desc, sizeof(desc_s));
    m_socket->writeDatagram(datagram, address, port);
}

void Server::send_connected_answer(const QHostAddress &address, const quint16 port)
{
    answer::connected_answer_desc_s desc;
    send_descriptor(desc, address, port);
}

void Server::send_user_online_answer(const user_s &user, const QHostAddress &address, const quint16 port)
{
    QByteArray byte_array;
    quint32 nickname_size = user.nickname.length();
    const quint32 datagram_size = sizeof(answer::user_online_answer_desc_s) + sizeof(user_desc_s) + sizeof(quint32) + nickname_size;
    byte_array.resize(datagram_size);
    Datagram datagram(byte_array);
    //desc
    answer::user_online_answer_desc_s desc;
    datagram.write(&desc, sizeof(answer::user_online_answer_desc_s));
    //user desc
    user_desc_s user_desc;
    user_desc.ip = address.toIPv4Address();
    user_desc.port = port;
    datagram.write(&user_desc, sizeof(user_desc_s));
    //nickname
    datagram.write(&nickname_size, sizeof(quint32));
    datagram.write(user.nickname.toLocal8Bit().data(), nickname_size);
    m_socket->writeDatagram(byte_array, address, port);
}
void Server::send_check_connection_query()
{
    m_mutex.lock();
    for (auto &user : m_users.keys())
    {
        if (!m_users[user].checked)
        {
            std::cout << "User " << m_users[user].nickname.toStdString() << " disconected" << std::endl;
            m_users.remove(user);
        }
    }
    m_mutex.unlock();
    QByteArray byte_array;
    const quint32 datagram_size = sizeof(query::check_connection_query_desc_s);
    byte_array.resize(datagram_size);
    Datagram datagram(byte_array);
    //desc
    query::check_connection_query_desc_s desc;
    datagram.write(&desc, sizeof(query::check_connection_query_desc_s));
    m_mutex.lock();
    for (auto &user : m_users.keys())
    {
        m_users[user].checked = false;
        m_socket->writeDatagram(byte_array, QHostAddress(user.ip), user.port);
    }
    m_mutex.unlock();
}

void Server::process_connection_query(Datagram &data, const QHostAddress &address, const quint16 port)
{
    QByteArray nickname;
    quint32 nickname_length;
    data.read(&nickname_length, sizeof(quint32));
    nickname.resize(nickname_length);
    data.read(nickname.data(), nickname_length);

    std::cout << "User " << QString(nickname).toStdString() << " from " << address.toString().toStdString() << "::" << port << " connects..." << std::endl;

    user_desc_s user_desc;
    user_desc.ip = address.toIPv4Address();
    user_desc.port = port;

    user_s user;
    user.nickname = QString(nickname);
    user.checked = true;

    m_mutex.lock();
    m_users.insert(user_desc, user);
    m_mutex.unlock();
}

void Server::read_datagram(QByteArray &byte_array, const QHostAddress &address, const quint16 port)
{

    Datagram data(byte_array);
    desc_s desc;
    data.read(&desc, sizeof(desc_s));

    if (query::query_signature == desc.signature)
    {
        if (query::QueryType::CONNECTION == desc.type)
        {
            process_connection_query(data, address, port);
            send_connected_answer(address, port);
        }
        else if (query::QueryType::WHO_IS_ONLINE == desc.type)
        {
            std::cout << "Recive who_is_online query from " << address.toString().toStdString() << "::" << port << std::endl;
            m_mutex.lock();
            for (auto &user : m_users)
            {
                send_user_online_answer(user, address, port);
            }
            m_mutex.unlock();
        }
        else if (query::QueryType::MESSAGE == desc.type)
        {

        }
        else if (query::QueryType::PRIVATE_MESSAGE == desc.type)
        {

        }
        else if (query::QueryType::CHECK_CONNECTION == desc.type)
        {

        }
    }
    else if (answer::answer_signature == desc.signature)
    {
        if (answer::AnswerType::CHECK_CONNECTION == desc.type)
        {
            user_desc_s user_desc;
            user_desc.ip = address.toIPv4Address();
            user_desc.port = port;
            m_mutex.lock();
            m_users[user_desc].checked = true;
            m_mutex.unlock();
        }
    }
}

void Server::start()
{
    std::cout << "Server started..." << std::endl;
    CheckConnection *checker = new CheckConnection(this);
    checker->start();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(listen()));
}

} //namespace server
} //namespace udp_chat
