#include "channel.h"

Channel::Channel(IRCServer* server, std::string channel_name): Nameable(server) {
  this->channel_name = channel_name;
  server->register_addressable_name(channel_name, this);
}

Channel::~Channel() {
  remove_addressable_name(channel_name);
}

void Channel::send_direct(std::vector<IRCCommand> messages, std::string exclude) {
  send_relay(messages, exclude);
}

std::string Channel::get_topic() {
  return topic;
}

void Channel::set_topic(std::string topic) {
  this->topic = topic;
}
bool Channel::has_topic() {
  return (this->topic!="");
}
NameableType Channel::what_are_you() {
  return NT_Channel;
}