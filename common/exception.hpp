#pragma once
#include <exception>
#include <string>

namespace udp_chat
{

class exception : public std::exception
{
public:
  exception(const std::string &message) : msg(message) { }
  virtual char const *what() const noexcept { return msg.c_str(); }
private:
  std::string msg;
};

} //namespace udp_chat
