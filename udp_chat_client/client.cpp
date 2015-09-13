#include <chrono>
#include <thread>
#include <QHostAddress>
#include <QUdpSocket>
#include "client.hpp"

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

void Client::send_connection_query()
{
    QByteArray byte_array;
    quint32 nickname_length = m_nickname.size();
    const quint32 datagram_size = sizeof(query::connection_query_desc_s) + sizeof(quint32) + sizeof(char) * nickname_length;
    byte_array.resize(datagram_size);

    Datagram datagram(byte_array);

    query::connection_query_desc_s desc;
    datagram.write(&desc, sizeof(query::connection_query_desc_s));
    datagram.write(&nickname_length, sizeof(quint32));
    datagram.write(m_nickname.toLocal8Bit().data(), nickname_length);

    m_socket->writeDatagram(byte_array, m_server_address, m_server_port);
    qDebug() << "Connecting to " << m_server_address.toString() << "::" << m_server_port << "...";
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
    if (!m_connected)
    {
        return false;
    }
    m_connected = false;
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
    qDebug() << QString(nickname) << " online!";
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
                m_connected = true;
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

            }
            else if (answer::AnswerType::MESSAGE == desc.type)
            {

            }
            else if (answer::AnswerType::CHECK_CONNECTION == desc.type)
            {
                m_connected = true;
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
