#include <QHostAddress>
#include <QUdpSocket>
#include "client.hpp"

namespace udp_chat
{
namespace client
{

Client::Client(QObject *parent):
    QObject(parent),
    m_mutex(),
    m_checked(true),
    m_connected(false),
    m_checker(new CheckConnection(this))
{    
    m_socket = new QUdpSocket(this);
}

Client::~Client()
{

}

void Client::start(const QHostAddress &server_address, const quint16 server_port)
{
    m_server_address = server_address;
    m_server_port = server_port;
    m_socket->bind(QHostAddress::Any);
    send_connection_query();
    QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(listen()));
    m_checker->start();
}

void Client::stop()
{
    m_checker->terminate();
    m_socket->close();
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
    status("Connecting to " + to_string(m_server_address.toIPv4Address(), m_server_port) + "...");
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
}

void Client::send_private_message_query(const quint32 ip, const quint16 port, const QString &msg)
{
    QByteArray byte_array;
    quint32 msg_size = msg.size();
    const quint32 datagram_size = sizeof(query::private_message_query_desc_s) + sizeof(user_desc_s) + sizeof(quint32) + sizeof(char) * msg_size;
    byte_array.resize(datagram_size);

    Datagram datagram(byte_array);
    //desc
    query::private_message_query_desc_s desc;
    datagram.write(&desc, sizeof(query::private_message_query_desc_s));
    //user_desc
    user_desc_s user_desc(ip, port);
    datagram.write(&user_desc, sizeof(user_desc_s));
    //message
    datagram.write(&msg_size, sizeof(quint32));
    datagram.write(msg.toLocal8Bit().data(), msg_size);

    m_socket->writeDatagram(byte_array, m_server_address, m_server_port);
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
    if (!m_checked)
    {
        return false;
    }
    m_checked = false;
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
    UserList::instance().add(user_desc, nickname);
    user_online(user_desc.ip, user_desc.port);
    status("User " + QString(nickname) + " online!");
}

void Client::process_user_offline_answer(Datagram &data)
{
    //user desc
    user_desc_s user_desc;
    data.read(&user_desc, sizeof(user_desc_s));
    user_offline(user_desc.ip, user_desc.port);
    status("User " + UserList::instance().get(user_desc) + " offline!");
    UserList::instance().remove(user_desc);
}

void Client::process_message_answer(Datagram &data)
{
    //user desc
    user_desc_s user_desc;
    data.read(&user_desc, sizeof(user_desc_s));
    //msg
    quint32 msg_size;
    QByteArray msg;
    data.read(&msg_size, sizeof(quint32));
    msg.resize(msg_size);
    data.read(msg.data(), msg_size);
    show_message("<font color=blue><b>"+UserList::instance().get(user_desc) + "</b></font>"+": " + QString(msg));
}

void Client::process_private_message_answer(Datagram &data)
{
    //sender
    user_desc_s sender;
    data.read(&sender, sizeof(user_desc_s));
    //reciever
    user_desc_s reciever;
    data.read(&reciever, sizeof(user_desc_s));
    //msg
    quint32 msg_size;
    QByteArray msg;
    data.read(&msg_size, sizeof(quint32));
    msg.resize(msg_size);
    data.read(msg.data(), msg_size);

    const QString sender_nickname = UserList::instance().get(sender);
    const QString reciever_nickname = UserList::instance().get(reciever);
    show_message("<font color=blue><b>"+sender_nickname+"</b></font>" + " private massage  "
                 +"<font color=red><b>"+ reciever_nickname+"</b></font>"
                 + ": " +"<i>" +QString(msg)+"</i>");
}

void Client::read_datagram(QByteArray &byte_array)
{
    Datagram data(byte_array);
    desc_s desc;
    data.read(&desc, sizeof(desc_s));

   if (answer::answer_signature == desc.signature)
    {
        if (answer::AnswerType::CONNECTED == desc.type)
        {
            status("Connected!");
            m_connected = true;
            connected();
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
        else if (answer::AnswerType::PRIVATE_MESSAGE == desc.type)
        {
            process_private_message_answer(data);
        }
        else if (answer::AnswerType::CHECK_CONNECTION == desc.type)
        {
            m_mutex.lock();
            m_checked = true;
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


void Client::listen()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram.data(), datagram.size());
        read_datagram(datagram);
    }
}

} //namespace client
} //namespace udp_chat
