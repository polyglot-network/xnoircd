#ifndef IRCSERVER_H
#define IRCSERVER_H

#include <string>
#include <unordered_map>

class Nameable;
class IRCServer {
private:
  std::unordered_map<std::string, Nameable*> nameable_map;
  std::string hostname;
public:

  IRCServer(std::string);

  void register_addressable_name(std::string name, Nameable* bind);
  bool is_addressable_name_free(std::string name);
  bool has_addressable_name(std::string name);
  void remove_addressable_name(std::string name);
  Nameable* resolve_addressable_name(std::string name);
  std::string get_hostname();
};

#endif