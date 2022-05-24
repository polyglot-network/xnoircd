#include "nameable.h"

Nameable::Nameable(IRCServer* server) {
  this->server = server;
}
Nameable::~Nameable() {
  this->associates.clear();
  this->associates.shrink_to_fit(); 
}

std::string Nameable::get_addressable_name() {
  return this->addressable_name;
}
bool Nameable::set_addressable_name(std::string name) {
  if (this->server->is_addressable_name_free(name)) {
    this->addressable_name = name;
    server->register_addressable_name(name, this);
    return true;
  } else {
    return false;
  }
}
bool Nameable::has_addressable_name() {
  return (this->addressable_name!="");
}
void Nameable::remove_addressable_name(std::string name) {
  server->remove_addressable_name(name);
}
bool Nameable::rename(std::string name) {
  if (server->is_addressable_name_free(name)) {
    server->remove_addressable_name(name);
    server->register_addressable_name(name, this);
    this->addressable_name = name;
    return true;
  } else {
    return false;
  }
}

std::vector<Nameable*> Nameable::get_associates()  {
  return this->associates;
}
void Nameable::add_associate(Nameable* associate) {
  std::cout << "Adding: " << associate->get_addressable_name() << "\n";
  this->associates.push_back(associate);
}
bool Nameable::has_associate(Nameable* associate) {
  return !(std::find(associates.begin(), associates.end(), associate) == associates.end());
}
void Nameable::remove_associate(Nameable* associate) {
  auto index = std::find(this->associates.begin(), this->associates.end(), associate);
  if (index != this->associates.end()) {
    this->associates.erase(index);
  }
}
std::string Nameable::associates_as_string() {
  std::string associate_list;
  for (Nameable* associate : associates) {
    associate_list += associate->get_addressable_name() + " ";
  }
  return associate_list;
}

void Nameable::send_direct(std::vector<IRCCommand> messages, std::string exclude) {}
void Nameable::send_relay(std::vector<IRCCommand> messages, std::string exclude) {
  for (Nameable* associate : this->associates) {
    if (associate->get_addressable_name() != exclude)
      associate->send_direct(messages);
  }
}

NameableType Nameable::what_are_you() {return NT_None;}