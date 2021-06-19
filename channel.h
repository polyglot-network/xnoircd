#ifndef CHANNEL_H
#define CHANNEL_H

#include <string>
#include "nameable.h"
#include "ircserver.h"

class Channel: public Nameable {
private:
  std::string channel_name;
  std::string topic;

public:
  Channel(IRCServer* server, std::string channel_name);
  ~Channel();

  void send_direct(std::string message);

  std::string get_topic();
  bool has_topic();
  void set_topic(std::string topic);

  NameableType what_are_you();
};

#endif