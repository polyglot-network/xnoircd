#include "userconnection.h"

#include <iostream>

UserConnection::UserConnection(int fd, IRCServer* server): Connection(fd) {
  this->server = server;
  this->user = new User(server, this);
}

UserConnection::~UserConnection() {}

void UserConnection::process_cmd(std::string cmd_in) {
  if (cmd_in == "")
    return;

  IRCCommand cmd = IRCCommand(cmd_in);
  std::cout << cmd.to_string() << std::endl;

/* if (state == Registered) {
    user->process_cmd(cmd);
    delete cmd;
    return;
  }

  std::string command = cmd.get_command();

  if (command == "NICK" && cmd.has_parameter()) {
    user->set_nick(cmd.get_parameter(0));
  }

  if (command == "USER" && cmd.has_parameter()) {
    user->set_username(cmd.get_parameter(0));
    user->set_realname(cmd.get_parameter(3));
  }
  
  if (user->has_nick() && user->has_username()) {
    state = Registered;
    send(":xnoircd 001 " + user->get_nick() + " :Welcome");
    send(":xnoircd 002 " + user->get_nick() + " :Welcome");
    send(":xnoircd 003 " + user->get_nick() + " :Welcome");
    send(":xnoircd 004 " + user->get_nick() + " xnoircd 0 ABCabc ABCabc");
    send(":xnoircd 422 " + user->get_nick() + " :No MOTD");
  }
*/

  for (IRCCommand c : user->process_cmd(cmd))
    send(c.to_string());
}

void UserConnection::destroy() {
  this->user->destroy();
}