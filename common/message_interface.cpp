#include "message_interface.hpp"

namespace udp_chat
{

bool operator<(const user_desc_s &lhs, const user_desc_s &rhs)
{
    if (lhs.ip == rhs.ip)
    {
        return lhs.port < rhs.port;
    }
    return lhs.ip < rhs.ip;
}

bool operator==(const user_desc_s &lhs, const user_desc_s &rhs)
{
    if (lhs.ip == rhs.ip)
    {
        return lhs.port == rhs.port;
    }
    return false;
}

QString to_string(quint32 ip)
{
    return QHostAddress(ip).toString();
}

QString to_string(quint32 ip, quint16 port)
{
    return to_string(ip) + "::" + QString::number(port);
}

} //namespace udp_chat
