namespace udp_chat
{

struct user_s
{
    unsigned int ip;
    unsigned short port;
}; // struct user_s

namespace query
{
    
enum QueryType 
{
    CONNECTION,
    WHO_IS_ONLINE,
    MESSAGE,
    PRIVATE_MESSAGE
}; // enum QueryType

struct query_s
{
    QueryType type;
}; // struct query_s

struct connection_query_s : public query_s
{
    
}; // struct connection_query_s

struct who_is_online_query_s : public query_s
{
    
}; // struct who_is_online_query_s

struct message_query_s : public query_s
{
    
}; // struct message_query_s

struct private_message_query_s : public query_s
{
    
}; // struct private_message_query_s    

} //namespace query  

namespace answer
{
enum AnswerType
{
    CONNECTED,
    USER_ONLINE,
    USER_OFFLINE,
    MESSAGE
} // enum AnswerType

struct answer_s
{
    AnswerType type;
}; // struct answer_s

struct connected_answer_s : public answer_s
{
    
}; // struct connected_answer_s

struct user_online_answer_s : public answer_s
{
 
}; // struct user_online_answer_s
 
struct user_offline_answer_s : public answer_s
{
 
}; // struct user_offline_answer_s 
 
struct message_answer_s : public answer_s
{
    
}; // struct message_answer_s

} //namespace answer
} //namespace udp_chat