#pragma once
#include <exception>

namespace udp_chat
{

class Datagram
{
public:
    Datagram(char *data, const size_t size)
    {
        m_data = data;
        m_size = size;
    }
    ~Datagram() {}
    void read(void *dst, const int size) 
    {
        if (m_pos - m_data + size > m_size)
        {
            throw std::exception("Error while reading datagram");
        }
        memcpy(dst, m_pos, size);
        m_pos += size;
    }
    void seek(const size_t offset, const bool from_beg = false)
    {
        m_pos = from_beg ? m_data + offset : m_pos + offset;
        if (m_pos < m_data || m_pos - m_data >= m_size)
        {
            throw std::exception("Error while seek the datagram");
        }
    }
    char * get()
    {
        return m_data;
    }
private:
    char *m_data;
    char *m_pos;
    size_t m_size;
}; // class Datagram

struct user_desc_s
{
    unsigned int ip;
    unsigned short port;
}; // struct user_desc_s

namespace query
{
    
const unsigned int query_signature = 6593485;  
  
enum QueryType 
{
    CONNECTION,
    WHO_IS_ONLINE,
    MESSAGE,
    PRIVATE_MESSAGE
}; // enum QueryType

struct query_desc_s
{
    unsigned int signature;
    QueryType type;
}; // struct query_desc_s

struct connection_query_desc_s : public query_desc_s
{
    connection_query_desc_s() {signature = query_signature; type = QueryType::CONNECTION;}
}; // struct connection_query_desc_s

struct who_is_online_query_desc_s : public query_desc_s
{
    who_is_online_query_desc_s() {signature = query_signature; type = QueryType::WHO_IS_ONLINE;}
}; // struct who_is_online_query_desc_s

struct message_query_desc_s : public query_desc_s
{
    message_query_desc_s() {signature = query_signature; type = QueryType::MESSAGE;}
    unsigned int message_length;
}; // struct message_query_desc_s

struct private_message_query_desc_s : public query_desc_s
{
    private_message_query_desc_s() {signature = query_signature; type = QueryType::PRIVATE_MESSAGE;}
}; // struct private_message_query_desc_s    

} //namespace query  

namespace answer
{
const unsigned int answer_signature = 43454523;

enum AnswerType
{
    CONNECTED,
    USER_ONLINE,
    USER_OFFLINE,
    MESSAGE
}; // enum AnswerType

struct answer_desc_s
{
    unsigned int signature;
    AnswerType type;
}; // struct answer_desc_s

struct connected_answer_desc_s : public answer_desc_s
{
    connected_answer_desc_s() {signature = answer_signature; type = AnswerType::CONNECTED;}
}; // struct connected_answer_desc_s

struct user_online_answer_desc_s : public answer_desc_s
{
    user_online_answer_desc_s() {signature = answer_signature; type = AnswerType::USER_ONLINE;}
}; // struct user_online_answer_desc_s
 
struct user_offline_answer_desc_s : public answer_desc_s
{
    user_offline_answer_desc_s() {signature = answer_signature; type = AnswerType::USER_OFFLINE;}
}; // struct user_offline_answer_desc_s 
 
struct message_answer_desc_s : public answer_desc_s
{
    message_answer_desc_s() {signature = answer_signature; type = AnswerType::MESSAGE;}
}; // struct message_answer_desc_s

} //namespace answer
} //namespace udp_chat
