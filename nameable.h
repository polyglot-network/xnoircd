#ifndef NAMEABLE_H
#define NAMEABLE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "connection.h"
#include "ircserver.h"

enum NameableType {
  NT_User,
  NT_Channel
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

  virtual void send_direct(std::string message);
  void send_relay(std::string message);

  virtual NameableType what_are_you();
};

#endif