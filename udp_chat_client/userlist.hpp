#pragma once
#include <QString>
#include <QMap>
#include <QHostAddress>
#include "../common/message_interface.hpp"
#include "../common/exception.hpp"


namespace udp_chat
{
namespace client
{

class UserList
{
    typedef QMap<user_desc_s, QString> users_t;
public:
    static UserList& instance()
    {
            static UserList theSingleInstance;
            return theSingleInstance;
    }
    QString get(const user_desc_s &key) const
    {
        return std::move(m_users.value(key));
    }
    void add(const user_desc_s &key, const QString &value)
    {
        if (m_users.contains(key))
        {
            throw exception("User List already contains user: " + QHostAddress(key.ip).toString().toStdString() + "::" + std::to_string(key.port));
        }
        m_users.insert(key, value);
    }
    void remove(const user_desc_s &key)
    {
        if (!m_users.contains(key))
        {
            throw exception("User List doesn't contain user: " + QHostAddress(key.ip).toString().toStdString() + "::" + std::to_string(key.port));
        }
        m_users.remove(key);
    }
    void clear()
    {
        m_users.clear();
    }

private:
    UserList(){}
    UserList(const UserList& root);
    UserList& operator=(const UserList&);
    users_t m_users;
}; //class UserList

} //namespace client
} //namespace udp_chat
