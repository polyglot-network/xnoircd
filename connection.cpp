#include "connection.h"
#include <iostream>

Connection::Connection(int fd) {
  this->fd = fd;
  std::thread(&Connection::conn_hdlr, this).detach();
}

Connection::~Connection() {
  close(this->fd);
}

void Connection::conn_hdlr() {
  buffer = new char[512];
  while (alive) {
    if (read(this->fd, buffer, 511) == 0) {
      destroy();
      return;
    }
    std::string message(buffer);
    int index;
    if ((index = message.find_first_of("\r\n")) == std::string::npos)
      continue;
    else 
      message = message.substr(0, index);
    process_cmd(message);
  }
}

void Connection::process_cmd(std::string command) {}
void Connection::destroy() {}

void Connection::kill() {
  this->alive = false;
}

void Connection::send(std::string to_write) {
  to_write = to_write + "\r\n";
  const char* buffer = to_write.c_str();
  int len = strlen(buffer);
  write(this->fd, buffer, len);
}