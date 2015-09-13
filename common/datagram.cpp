#include <cstring>
#include "exception.hpp"
#include "datagram.hpp"

namespace udp_chat
{

Datagram::Datagram(QByteArray &byte_array):
    m_data(byte_array.data()),
    m_pos(byte_array.data()),
    m_size(byte_array.size())
{

}

Datagram::~Datagram() {}

void Datagram::read(void *dst, const unsigned int size)
{
    if (m_pos - m_data + size > m_size)
    {
        throw exception("Error while reading datagram");
    }
    memcpy(dst, m_pos, size);
    m_pos += size;
}

void Datagram::write(void *src, const unsigned int size)
{
    if (m_pos - m_data + size > m_size)
    {
        throw exception("Error while writing datagram");
    }
    memcpy(m_pos, src, size);
    m_pos += size;
}

void Datagram::seek(const unsigned int offset, const bool from_beg)
{
    m_pos = from_beg ? m_data + offset : m_pos + offset;
    if (m_pos < m_data || m_pos - m_data >= m_size)
    {
        throw exception("Error while seek the datagram");
    }
}

char * Datagram::get()
{
    return m_data;
}

} // namespace udp_chat
