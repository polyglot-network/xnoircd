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

enum UCState {
  Initial,
  Registered
};

class UserConnection;
class User: public Nameable {
private:
  std::string username, realname = "";
  Connection* connection;
  UCState state;

  std::vector<IRCCommand> nick_cmd(IRCCommand cmd);
  std::vector<IRCCommand> user_cmd(IRCCommand cmd);
  std::vector<IRCCommand> ping_cmd(IRCCommand cmd);
  std::vector<IRCCommand> who_cmd(IRCCommand cmd);
  std::vector<IRCCommand> whois_cmd(IRCCommand cmd);
  std::vector<IRCCommand> join_cmd(IRCCommand cmd);
  std::vector<IRCCommand> part_cmd(IRCCommand cmd);
  std::vector<IRCCommand> quit_cmd(IRCCommand cmd);
  std::vector<IRCCommand> privmsg_cmd(IRCCommand cmd);
  std::vector<IRCCommand> topic_cmd(IRCCommand cmd);
  std::vector<IRCCommand> names_cmd(IRCCommand cmd);
  std::vector<IRCCommand> motd_cmd(IRCCommand cmd);

  static std::unordered_map<std::string, std::tuple<int, std::function<std::vector<IRCCommand>(User*, IRCCommand)>, bool>> commands;

  //std::string construct_string(int count, std::string source, std::string command, std::string parameters...);
  template<typename... T>
  std::string construct_string(std::string source, std::string command, T... parameters);

public:
  User(IRCServer* server, UserConnection* conn);
  ~User();

  std::vector<IRCCommand> process_cmd(IRCCommand cmd);

  IRCCommand set_nick(std::string nick);

  void set_username(std::string username);

  void set_realname(std::string realname);

  bool has_nick();

  bool has_username();

  bool has_realname();

  std::string get_nick();

  std::string get_username();

  std::string get_realname();

  void send_direct(std::vector<IRCCommand> messages, std::string exclude="");

  NameableType what_are_you() override;

  void destroy();
};

#endif