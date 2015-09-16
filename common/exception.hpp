#pragma once
#include <exception>
#include <string>

namespace udp_chat
{

class exception : public std::exception
{
public:
  exception(const std::string &message) : msg(message) { }
#ifdef __linux__
  virtual char const *what() const noexcept { return msg.c_str(); }
#elif _WIN32
  virtual char const *what() const { return msg.c_str(); }
#endif
private:
  std::string msg;
};

} //namespace udp_chat
