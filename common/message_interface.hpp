#pragma once

namespace udp_chat
{

struct user_desc_s
{
    unsigned int ip;
    unsigned short port;
}; // struct user_desc_s

struct desc_s
{
    unsigned int signature;
    unsigned char type;
}; // struct desc_s

namespace query
{
    
const unsigned int query_signature = 6593485;  
  
enum QueryType 
{
    CONNECTION,
    WHO_IS_ONLINE,
    MESSAGE,
    PRIVATE_MESSAGE,
    CHECK_CONNECTION
}; // enum QueryType

struct connection_query_desc_s : public desc_s
{
    connection_query_desc_s() {signature = query_signature; type = QueryType::CONNECTION;}
}; // struct connection_query_desc_s

struct who_is_online_query_desc_s : public desc_s
{
    who_is_online_query_desc_s() {signature = query_signature; type = QueryType::WHO_IS_ONLINE;}
}; // struct who_is_online_query_desc_s

struct message_query_desc_s : public desc_s
{
    message_query_desc_s() {signature = query_signature; type = QueryType::MESSAGE;}
    unsigned int message_length;
}; // struct message_query_desc_s

struct private_message_query_desc_s : public desc_s
{
    private_message_query_desc_s() {signature = query_signature; type = QueryType::PRIVATE_MESSAGE;}
}; // struct private_message_query_desc_s    

struct check_connection_query_desc_s : public desc_s
{
    check_connection_query_desc_s() {signature = query_signature; type = QueryType::CHECK_CONNECTION;}
}; // struct check_connection_query_desc_s

} //namespace query  

namespace answer
{
const unsigned int answer_signature = 43454523;

enum AnswerType
{
    CONNECTED,
    USER_ONLINE,
    USER_OFFLINE,
    MESSAGE,
    CHECK_CONNECTION
}; // enum AnswerType

struct connected_answer_desc_s : public desc_s
{
    connected_answer_desc_s() {signature = answer_signature; type = AnswerType::CONNECTED;}
}; // struct connected_answer_desc_s

struct user_online_answer_desc_s : public desc_s
{
    user_online_answer_desc_s() {signature = answer_signature; type = AnswerType::USER_ONLINE;}
}; // struct user_online_answer_desc_s
 
struct user_offline_answer_desc_s : public desc_s
{
    user_offline_answer_desc_s() {signature = answer_signature; type = AnswerType::USER_OFFLINE;}
}; // struct user_offline_answer_desc_s 
 
struct message_answer_desc_s : public desc_s
{
    message_answer_desc_s() {signature = answer_signature; type = AnswerType::MESSAGE;}
}; // struct message_answer_desc_s

struct check_connection_answer_desc_s : public desc_s
{
    check_connection_answer_desc_s() {signature = answer_signature; type = AnswerType::CHECK_CONNECTION;}
}; // struct check_connection_answer_desc_s

} //namespace answer
} //namespace udp_chat
