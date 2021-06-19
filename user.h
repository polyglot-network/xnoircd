#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_map>
#include <tuple>
#include <functional>

#include "irccommand.h"
#include "ircserver.h"
#include "userconnection.h"
#include "nameable.h"
#include "channel.h"

class UserConnection;
class User: public Nameable {
private:
  std::string username, realname = "";
  Connection* connection;

  void nick_cmd(IRCCommand* cmd);
  void ping_cmd(IRCCommand* cmd);
  void who_cmd(IRCCommand* cmd);
  void whois_cmd(IRCCommand* cmd);
  void join_cmd(IRCCommand* cmd);
  void part_cmd(IRCCommand* cmd);
  void quit_cmd(IRCCommand* cmd);
  void privmsg_cmd(IRCCommand* cmd);
  void topic_cmd(IRCCommand* cmd);
  void names_cmd(IRCCommand* cmd);

  static std::unordered_map<std::string, std::tuple<int, std::function<void(User*, IRCCommand*)>>> commands;

  //std::string construct_string(int count, std::string source, std::string command, std::string parameters...);
  template<typename... T>
  std::string construct_string(std::string source, std::string command, T... parameters);

public:
  User(IRCServer* server, UserConnection* conn);
  ~User();

  void process_cmd(IRCCommand* cmd);

  void set_nick(std::string nick);

  void set_username(std::string username);

  void set_realname(std::string realname);

  bool has_nick();

  bool has_username();

  bool has_realname();

  std::string get_nick();

  std::string get_username();

  std::string get_realname();

  void send_direct(std::string message);

  NameableType what_are_you();

  void destroy();
};

#endif