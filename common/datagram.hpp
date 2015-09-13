#pragma once
#include <QByteArray>

namespace udp_chat
{

class Datagram
{
public:
    Datagram(QByteArray &byte_array);
    ~Datagram();
    void read(void *dst, const unsigned int size);
    void write(void *src, const unsigned int size);
    void seek(const unsigned int offset, const bool from_beg = false);
    char * get();
private:
    char *m_data;
    char *m_pos;
    unsigned int m_size;
}; // class Datagram

} //namespace udp_chat
