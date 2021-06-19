#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <unistd.h>
#include <thread>
#include <string.h>

class Connection {
private:
  int fd;
  int alive = true;
  char* buffer;

public:
  Connection(int fd);
  ~Connection();

  void conn_hdlr();

  virtual void process_cmd(std::string command);
  virtual void destroy();

  void kill();

  void send(std::string to_write);
};

#endif