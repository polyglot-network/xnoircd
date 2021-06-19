#include "user.h"
#include <iostream>

std::vector<std::string> split_string(std::string str, std::string delim) {
  std::vector<std::string> to_return;
  int delim_len = delim.length();
  int index = str.find_first_of(delim);
  while (index != std::string::npos) {
    to_return.push_back(str.substr(0, index));
    str = str.substr(index+delim_len);
    index = str.find_first_of(delim);
  }
  to_return.push_back(str);
  return to_return;
}

std::unordered_map<std::string, std::tuple<int, std::function<void(User*, IRCCommand*)>>> User::commands = {
  {"NICK", {0, &User::nick_cmd}},
  {"PING", {1, &User::ping_cmd}},
  {"WHO", {1, &User::who_cmd}},
  {"WHOIS", {1, &User::whois_cmd}},
  {"JOIN", {1, &User::join_cmd}},
  {"PART", {1, &User::part_cmd}},
  {"QUIT", {0, &User::quit_cmd}},
  {"PRIVMSG", {2, &User::privmsg_cmd}},
  {"TOPIC", {2, &User::topic_cmd}},
  {"NAMES", {1, &User::names_cmd}}
};

User::User(IRCServer* server, UserConnection* conn): Nameable(server) {
  this->connection = conn;
}
User::~User() {
  this->server->remove_addressable_name(this->get_nick());  
  this->send_relay(":" + this->get_nick() + " QUIT :Quit: ");
  for (Nameable* associate : this->get_associates()) 
    associate->remove_associate(this);
  delete connection;
}

void User::nick_cmd(IRCCommand* cmd) {
  set_nick(cmd->get_payload(0));
}

void User::ping_cmd(IRCCommand* cmd) {
  send_direct(":" + server->get_hostname() + " PONG " + server->get_hostname() + " " + cmd->get_payload(0));
}

void User::join_cmd(IRCCommand* cmd) {
  std::string channels_parameter = cmd->get_payload(0);

  std::vector<std::string> named_channels = split_string(channels_parameter, ",");
  std::cout << named_channels.size() << "\n";

  for (std::string named_channel : named_channels) {
    Nameable* channel;
    if (server->has_addressable_name(named_channel)) {
      channel = server->resolve_addressable_name(named_channel);
    } else {
      channel = new Channel(server, named_channel);
    }
    
    if (channel->what_are_you() != NT_Channel)
      continue;

    Channel* chan = (Channel*)channel;

    this->add_associate(chan);
    chan->add_associate(this);
    chan->send_direct(":" + this->get_nick() + " JOIN " + named_channel);
    std::vector<Nameable*> channel_members = chan->get_associates();

    this->send_direct(":" + server->get_hostname() + " 353 " + this->get_nick() + " " + named_channel + " :" + chan->associates_as_string());
    if (chan->has_topic())
      this->send_direct(":" + server->get_hostname() + " 332 " + this->get_nick() + " " + named_channel + " :" + chan->get_topic());
  }
}

void User::part_cmd(IRCCommand* cmd) {
  std::string channels_parameter = cmd->get_payload(0);

  std::vector<std::string> named_channels = split_string(channels_parameter, ",");

  for (std::string named_channel : named_channels) {
    if (server->has_addressable_name(named_channel)) {
      Nameable* channel = server->resolve_addressable_name(named_channel);
      this->remove_associate(channel);
      channel->remove_associate(this);
      channel->send_direct(":" + this->get_nick() + " PART " + named_channel);
    }
  }
}

void User::privmsg_cmd(IRCCommand* cmd) {
  std::string named_target = cmd->get_payload(0);

  if (server->has_addressable_name(named_target)) {
    Nameable* target = server->resolve_addressable_name(named_target);
    for (Nameable* associate : target->get_associates()) {
      if (associate->get_addressable_name() != this->get_nick())
        associate->send_direct(":" + this->get_nick() + " PRIVMSG " + named_target + " :" + cmd->get_payload(1));
    }
  }
}

void User::quit_cmd(IRCCommand* cmd) {
  destroy();
}

void User::who_cmd(IRCCommand* cmd) {
  if (!server->has_addressable_name(cmd->get_payload(0)))
    return;
  
  Nameable* target = server->resolve_addressable_name(cmd->get_payload(0));
  if (target->what_are_you() != NT_User)
    return;
  
  User* user = (User*)target;

  send_direct(":" + server->get_hostname() + " 352 " + this->get_nick() + " * " + user->get_username() + " " /*+ user->get_hostname()*/ + " " + server->get_hostname() + " H :0 " + user->get_realname());
  send_direct(":" + server->get_hostname() + " 315 " + this->get_nick() + " " + user->get_nick() + " :End of WHO listing.");
}

void User::whois_cmd(IRCCommand* cmd) {
  if (!server->has_addressable_name(cmd->get_payload(0)))
    return;
  
  Nameable* target = server->resolve_addressable_name(cmd->get_payload(0));
  if (target->what_are_you() != NT_User)
    return;
  
  User* user = (User*)target;
}

void User::topic_cmd(IRCCommand* cmd) {
  if (!server->has_addressable_name(cmd->get_payload(0)))
    return;

  Nameable* target = server->resolve_addressable_name(cmd->get_payload(0));
  if (target->what_are_you() != NT_Channel)
    return;

  ((Channel*)target)->set_topic(cmd->get_payload(1));
}

void User::names_cmd(IRCCommand* cmd) {
  std::string named_channel = cmd->get_payload(0);
  if (!server->has_addressable_name(named_channel))
    return;
  Nameable* resolved = server->resolve_addressable_name(named_channel);
  if (resolved->what_are_you() != NT_Channel)
    return;
  Channel* chan = (Channel*)resolved;
  this->send_direct(":" + server->get_hostname() + " 353 " + this->get_nick() + " " + named_channel + " :" + chan->associates_as_string());
}

void User::process_cmd(IRCCommand* cmd) {
  auto command = commands.find(cmd->get_command());
  if (command == commands.end()) {
    send_direct(":" + server->get_hostname() + " 421 " + get_nick() + " " + cmd->get_command() + " :Unknown Command.");
    return;
  }
  
  if (cmd->get_payload_count() < std::get<0>(command->second)) {
    send_direct(":" + server->get_hostname() + " 461 " + get_nick() + " " + cmd->get_command() + " :Too few parameters.");
    return;
  }
  
  std::get<1>(command->second)(this, cmd);
}

void User::set_nick(std::string nick) {
  bool success;
  std::string old_nick = this->get_nick();
  if (this->has_nick())
    success = this->rename(nick);
  else
    success = this->set_addressable_name(nick);
  
  if (success) {
    if (old_nick != "")
      send_direct(":" + old_nick + " NICK :" + this->get_nick());
  } else {
    send_direct(":" + server->get_hostname() + " 433 * " + nick + " Nickname is already in use!");
  }
}

void User::set_username(std::string username) {
  this->username = username;
}

void User::set_realname(std::string realname) {
  this->realname = realname;
}

bool User::has_nick() {
  return has_addressable_name();
}

bool User::has_username(){
  return (username!="");
}

bool User::has_realname(){
  return (realname!="");
}

std::string User::get_nick(){
  return get_addressable_name();
}

std::string User::get_username(){
  return username;
}

std::string User::get_realname(){
  return realname;
}

void User::send_direct(std::string message) {
  std::cout << message << "\n";
  this->connection->send(message);
}

NameableType User::what_are_you() {
  return NT_User;
}

void User::destroy() {
  Connection* conn = connection;
  delete this;
  conn->kill();
}