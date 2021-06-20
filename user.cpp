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

std::unordered_map<std::string, std::tuple<int, std::function<std::vector<IRCCommand>(User*, IRCCommand)>, bool>> User::commands = {
  {"NICK", {1, &User::nick_cmd, false}},
  {"USER", {4, &User::user_cmd, false}},
  {"PING", {1, &User::ping_cmd, false}},
  {"WHO", {1, &User::who_cmd, true}},
  {"WHOIS", {1, &User::whois_cmd, true}},
  {"JOIN", {1, &User::join_cmd, true}},
  {"PART", {1, &User::part_cmd, true}},
  {"QUIT", {0, &User::quit_cmd, true}},
  {"PRIVMSG", {2, &User::privmsg_cmd, true}},
  {"TOPIC", {2, &User::topic_cmd, true}},
  {"NAMES", {1, &User::names_cmd, true}},
  {"MOTD", {0, &User::motd_cmd, true}}
};

User::User(IRCServer* server, UserConnection* conn): Nameable(server) {
  this->connection = conn;
}
User::~User() {
  this->server->remove_addressable_name(this->get_nick());  
  this->send_relay({IRCCommand(this->get_nick(), "QUIT",  "Quit: ")});
  for (Nameable* associate : this->get_associates()) 
    associate->remove_associate(this);
  delete connection;
}

std::vector<IRCCommand> User::nick_cmd(IRCCommand cmd) {
  return {set_nick(cmd.get_parameter(0))};
}

std::vector<IRCCommand> User::user_cmd(IRCCommand cmd) {
  this->username = cmd.get_parameter(0);
  this->realname = cmd.get_parameter(3);

  if (this->has_nick()) {
    this->state = Registered;
  }
  
  std::vector<IRCCommand> to_return = {
    IRCCommand(server->get_hostname(), "001", this->get_nick(), "Welcome to the Polyglottal Network, " + this->get_nick()),
    IRCCommand(server->get_hostname(), "002", this->get_nick(), "Your host is " + server->get_hostname()),
    IRCCommand(server->get_hostname(), "003", this->get_nick(), "This server was never created."),
    IRCCommand(server->get_hostname(), "004", this->get_nick(), server->get_hostname(), "0", "ABCabc", "ABCabc")
  };

  auto motd = motd_cmd(IRCCommand());

  to_return.insert(to_return.end(), motd.begin(), motd.end());
  return to_return;
}

std::vector<IRCCommand> User::ping_cmd(IRCCommand cmd) {
  return {IRCCommand(server->get_hostname(), "PONG", server->get_hostname(), cmd.get_parameter(0))};
}

std::vector<IRCCommand> User::join_cmd(IRCCommand cmd) {
  std::string channels_parameter = cmd.get_parameter(0);

  std::vector<std::string> named_channels = split_string(channels_parameter, ",");
  std::cout << named_channels.size() << "\n";

  std::vector<IRCCommand> to_return;

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
    chan->send_direct({IRCCommand(this->get_nick(), "JOIN", named_channel)});
    std::vector<Nameable*> channel_members = chan->get_associates();

    to_return.push_back(IRCCommand(server->get_hostname(), "353", this->get_nick(), "=", named_channel, chan->associates_as_string()));
    if (chan->has_topic())
      to_return.push_back(IRCCommand(server->get_hostname(), "332", this->get_nick(), named_channel, chan->get_topic()));
  }

  return to_return;
}

std::vector<IRCCommand> User::part_cmd(IRCCommand cmd) {
  std::string channels_parameter = cmd.get_parameter(0);

  std::vector<std::string> named_channels = split_string(channels_parameter, ",");

  for (std::string named_channel : named_channels) {
    if (server->has_addressable_name(named_channel)) {
      Nameable* channel = server->resolve_addressable_name(named_channel);
      this->remove_associate(channel);
      channel->remove_associate(this);
      channel->send_direct({IRCCommand(this->get_nick(), "PART", named_channel)});
    }
  }

  return {};
}

std::vector<IRCCommand> User::privmsg_cmd(IRCCommand cmd) {
  std::string named_target = cmd.get_parameter(0);

  if (server->has_addressable_name(named_target)) {
    Nameable* target = server->resolve_addressable_name(named_target);
    /*for (Nameable* associate : target->get_associates()) {
      if (associate->get_addressable_name() != this->get_nick())
        associate->send_direct({IRCCommand(this->get_nick(), "PRIVMSG", named_target, cmd.get_parameter(1))});
    }*/
    target->send_direct({IRCCommand(this->get_nick(), "PRIVMSG", named_target, cmd.get_parameter(1))}, this->get_nick());
  }

  return {};
}

std::vector<IRCCommand> User::quit_cmd(IRCCommand cmd) {
  destroy();
  return {};
}

std::vector<IRCCommand> User::who_cmd(IRCCommand cmd) {
  if (!server->has_addressable_name(cmd.get_parameter(0)))
    return {};
  
  Nameable* target = server->resolve_addressable_name(cmd.get_parameter(0));
  if (target->what_are_you() != NT_User)
    return {};
  
  User* user = (User*)target;

  return {
    IRCCommand(server->get_hostname(), "352", this->get_nick(), "*", user->get_username(), " ", server->get_hostname(), "H", "0 " + user->get_realname()),
    IRCCommand(server->get_hostname(), "315", this->get_nick(), user->get_nick(), "End of WHO listing.")
  };
}

std::vector<IRCCommand> User::whois_cmd(IRCCommand cmd) {
  if (!server->has_addressable_name(cmd.get_parameter(0)))
    return {};
  
  Nameable* target = server->resolve_addressable_name(cmd.get_parameter(0));
  if (target->what_are_you() != NT_User)
    return {};
  
  User* user = (User*)target;
  return {};
}

std::vector<IRCCommand> User::topic_cmd(IRCCommand cmd) {
  std::string named_channel = cmd.get_parameter(0);
  if (!server->has_addressable_name(named_channel))
    return {};

  Nameable* target = server->resolve_addressable_name(named_channel);
  if (target->what_are_you() != NT_Channel)
    return {};

  std::string topic = cmd.get_parameter(1);

  ((Channel*)target)->set_topic(topic);;
  return {IRCCommand(server->get_hostname(), this->get_nick(), named_channel, topic)};
}

std::vector<IRCCommand> User::names_cmd(IRCCommand cmd) {
  std::string named_channel = cmd.get_parameter(0);
  if (!server->has_addressable_name(named_channel))
    return {};
  Nameable* resolved = server->resolve_addressable_name(named_channel);
  if (resolved->what_are_you() != NT_Channel)
    return {};
  Channel* chan = (Channel*)resolved;
  return {
    IRCCommand(server->get_hostname(), "353", this->get_nick(), named_channel, chan->associates_as_string()),
    IRCCommand(server->get_hostname(), "366", this->get_nick(), named_channel, "End of NAMES.")
  };
}

std::vector<IRCCommand> User::motd_cmd(IRCCommand cmd) {
  return {IRCCommand(server->get_hostname(), "422", this->get_nick(), "MOTD is not defined.")};
}

std::vector<IRCCommand> User::process_cmd(IRCCommand cmd) {
  auto command = commands.find(cmd.get_command());
  //return {};
  if (command == commands.end()) {
    return {IRCCommand(server->get_hostname(), "421",  get_nick(), cmd.get_command(), "Unknown Command.")};
  }
  
  if (cmd.get_parameter_count() < std::get<0>(command->second)) {
    return {IRCCommand(server->get_hostname(), "461", get_nick(), cmd.get_command(), "Too few parameters.")};
  }

  if (this->state == Initial && std::get<2>(command->second))
    return {};
  
  return std::get<1>(command->second)(this, cmd);
}

IRCCommand User::set_nick(std::string nick) {
  bool success;
  std::string old_nick = this->get_nick();
  if (this->has_nick())
    success = this->rename(nick);
  else
    success = this->set_addressable_name(nick);
  
  if (success) {
    if (old_nick != "") {
       return IRCCommand(old_nick, "NICK", this->get_nick());
    }
    return {};
  } else {
    return IRCCommand(server->get_hostname(), "433", "*", nick, "Nickname is already in use!");
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

void User::send_direct(std::vector<IRCCommand> messages, std::string exclude) {
  for (IRCCommand m : messages) {
    std::cout << m.to_string() << "\n";
    this->connection->send(m.to_string());
  }
}

NameableType User::what_are_you() {
  return NT_User;
}

void User::destroy() {
  Connection* conn = connection;
  delete this;
  conn->kill();
}