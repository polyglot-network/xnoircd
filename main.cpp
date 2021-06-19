#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "ircserver.h"
#include "userconnection.h"

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == 0) {
    std::cout << "Socket Create Failed!\n" << std::flush;
    return 1;
  }
  int opt = 1;
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    std::cout << "Setting Socket Options Failed!\n" << std::flush;
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(6667);

  unsigned int addrsize = sizeof(addr);
  if (bind(socketfd, (struct sockaddr*)&addr, addrsize) < 0) {
    std::cout << "Socket Bind Failed!\n" << std::flush;
    return 1;
  }

  if (listen(socketfd, 5) < 0) {
    std::cout << "Socket Listen Failed!\n" << std::flush;
    return 1;
  }

  IRCServer* server = new IRCServer("xnoircd");

  while (true) {
    int sockfd = accept(socketfd, (struct sockaddr*)&addr, &addrsize);
    new UserConnection(sockfd, server);
  }
}