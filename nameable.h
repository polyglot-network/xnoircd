#ifndef NAMEABLE_H
#define NAMEABLE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "connection.h"
#include "ircserver.h"
#include "irccommand.h"

enum NameableType {
  NT_None = 1,
  NT_User = 2,
  NT_Channel = 3
};

class Nameable {
private:
  std::string addressable_name = "";
  std::vector<Nameable*> associates;

public:
  IRCServer* server;
  Nameable(IRCServer* server);
  ~Nameable();

  std::string get_addressable_name();
  bool set_addressable_name(std::string name);
  bool has_addressable_name();
  void remove_addressable_name(std::string name);
  bool rename(std::string to);

  std::vector<Nameable*> get_associates();
  void add_associate(Nameable* associate);
  bool has_associate(Nameable* associate);
  void remove_associate(Nameable* associate);
  std::string associates_as_string();

  virtual void send_direct(std::vector<IRCCommand> messages);
  void send_relay(std::vector<IRCCommand> messages);

  virtual NameableType what_are_you();
};

#endif