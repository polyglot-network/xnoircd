#include "ircserver.h"

IRCServer::IRCServer(std::string hostname) {
  this->hostname = hostname;
  this->nameable_map = {};
}

bool IRCServer::is_addressable_name_free(std::string name) {
  return nameable_map.find(name) == nameable_map.end(); 
}

bool IRCServer::has_addressable_name(std::string name) {
  return !is_addressable_name_free(name);
}

void IRCServer::remove_addressable_name(std::string name) {
  nameable_map.erase(name);
}

void IRCServer::register_addressable_name(std::string name, Nameable* bind) {
  nameable_map[name] = bind;
}

Nameable* IRCServer::resolve_addressable_name(std::string name) {
  return nameable_map[name];
}

std::string IRCServer::get_hostname() {
  return hostname;
}