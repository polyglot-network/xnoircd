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

  for (IRCCommand c : user->process_cmd(cmd))
    send(c.to_string());
}

void UserConnection::destroy() {
  this->user->destroy();
}