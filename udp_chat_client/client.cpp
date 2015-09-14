#include <chrono>
#include <thread>
#include <QHostAddress>
#include <QUdpSocket>
#include "client.hpp"

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
namespace client
{

Client::Client(const QHostAddress &server_address, const quint16 server_port, const quint16 port, QObject *parent):
    QObject(parent),
    m_server_address(server_address),
    m_server_port(server_port),
    m_socket(new QUdpSocket(parent)),
    m_mutex()
{
    m_socket->bind(QHostAddress::Any, port);
}

Client::~Client()
{
}

void Client::start()
{
    qDebug() << "Client started...";
    send_connection_query();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(listen()));
}

void show_message(const QString &msg)
{
    emit msg;
}

void Client::send_connection_query()
{
    QByteArray byte_array;
    quint32 nickname_size = m_nickname.size();
    const quint32 datagram_size = sizeof(query::connection_query_desc_s) + sizeof(quint32) + sizeof(char) * nickname_size;
    byte_array.resize(datagram_size);

    Datagram datagram(byte_array);

    query::connection_query_desc_s desc;
    datagram.write(&desc, sizeof(query::connection_query_desc_s));
    datagram.write(&nickname_size, sizeof(quint32));
    datagram.write(m_nickname.toLocal8Bit().data(), nickname_size);

    m_socket->writeDatagram(byte_array, m_server_address, m_server_port);
    qDebug() << "Connecting to " << m_server_address.toString() << "::" << m_server_port << "...";
}

void Client::send_message_query(const QString &msg)
{
    QByteArray byte_array;
    quint32 msg_size = msg.size();
    const quint32 datagram_size = sizeof(query::message_query_desc_s) + sizeof(quint32) + sizeof(char) * msg_size;
    byte_array.resize(datagram_size);

    Datagram datagram(byte_array);
    //desc
    query::message_query_desc_s desc;
    datagram.write(&desc, sizeof(query::message_query_desc_s));
    //message
    datagram.write(&msg_size, sizeof(quint32));
    datagram.write(msg.toLocal8Bit().data(), msg_size);

    m_socket->writeDatagram(byte_array, m_server_address, m_server_port);
    qDebug() << "Sending message " << msg << " to" << m_server_address.toString() << "::" << m_server_port << "...";
}

void Client::send_descriptor(desc_s desc)
{
    QByteArray datagram;
    datagram.resize(sizeof(desc_s));
    memcpy(datagram.data(), &desc, sizeof(desc_s));
    m_socket->writeDatagram(datagram, m_server_address, m_server_port);
}

void Client::send_who_is_online_query()
{
    qDebug() << "Sending who_is_online query...";
    query::who_is_online_query_desc_s desc;
    send_descriptor(desc);
}

void Client::send_check_connection_answer()
{
    answer::check_connection_answer_desc_s desc;
    send_descriptor(desc);
}

bool Client::send_check_connection_query()
{
    m_mutex.lock();
    if (!m_connected)
    {
        return false;
    }
    m_connected = false;
    m_mutex.unlock();
    query::check_connection_query_desc_s desc;
    send_descriptor(desc);
    return true;
}

void Client::set_nickname(const QString &nickname)
{
    m_nickname = nickname;
}

void Client::process_user_online_answer(Datagram &data)
{
    //user desc
    user_desc_s user_desc;
    data.read(&user_desc, sizeof(user_desc_s));
    //nickname
    quint32 nickname_length;
    QByteArray nickname;
    data.read(&nickname_length, sizeof(quint32));
    nickname.resize(nickname_length);
    data.read(nickname.data(), nickname_length);
    m_users[user_desc] = QString(nickname);
    qDebug() << QString(nickname) << user_desc.port << " online!";
}

void Client::process_user_offline_answer(Datagram &data)
{
    //user desc
    user_desc_s user_desc;
    data.read(&user_desc, sizeof(user_desc_s));
    qDebug() << m_users[user_desc] << " ofline!";
    m_users.remove(user_desc);
}

void Client::process_message_answer(Datagram &data)
{
    //user desc
    user_desc_s user_desc;
    data.read(&user_desc, sizeof(user_desc_s));
    quint32 msg_size;
    QByteArray msg;
    data.read(&msg_size, sizeof(quint32));
    msg.resize(msg_size);
    data.read(msg.data(), msg_size);
    show_message(QString(msg));
    qDebug() << "Recieve msg from " << m_users[user_desc] << ": " << QString(msg);
}

void Client::read_datagram(QByteArray &byte_array, const QHostAddress &address, const quint16 port)
{
    Datagram data(byte_array);
    desc_s desc;
    data.read(&desc, sizeof(desc_s));

    if (address == m_server_address && port == m_server_port)
    {
        if (answer::answer_signature == desc.signature)
        {
            if (answer::AnswerType::CONNECTED == desc.type)
            {
                qDebug() << "Connected!";
                m_mutex.lock();
                m_connected = true;
                m_mutex.unlock();
                CheckConnection *checker = new CheckConnection(this);
                checker->start();
                send_who_is_online_query();
            }
            else if (answer::AnswerType::USER_ONLINE == desc.type)
            {
                process_user_online_answer(data);
            }
            else if (answer::AnswerType::USER_OFFLINE == desc.type)
            {
                process_user_offline_answer(data);
            }
            else if (answer::AnswerType::MESSAGE == desc.type)
            {
                process_message_answer(data);
            }
            else if (answer::AnswerType::CHECK_CONNECTION == desc.type)
            {
                m_mutex.lock();
                m_connected = true;
                m_mutex.unlock();
            }
        }
        else if (query::query_signature == desc.signature)
        {
            if (query::QueryType::CHECK_CONNECTION == desc.type)
            {
                send_check_connection_answer();
            }
        }
    }
}

void Client::listen()
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

} //namespace client
} //namespace udp_chat
