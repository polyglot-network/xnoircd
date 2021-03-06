#ifndef USERCONNECTION_H
#define USERCONNECTION_H

#include <string>

#include "connection.h"
#include "ircserver.h"
#include "user.h"

class User;
class UserConnection: public Connection {
private:
  User* user;
  IRCServer* server;
public:
  UserConnection(int fd, IRCServer* server);
  ~UserConnection();

  void process_cmd(std::string cmd_in);
  void destroy();
};

#endif