#include "irccommand.h"

#include <iostream>

IRCCommand::IRCCommand() {}
IRCCommand::IRCCommand(std::string raw_command) {
  if (raw_command[0] == '@') {
    int index = raw_command.find_first_of(" ");

    std::string tag_section = raw_command.substr(1, index);
    raw_command = raw_command.substr(index+1);

    // Todo: Further process tags down
  }

  if (raw_command[0] != 0)
    raw_command.substr(1);
  else 
    return;

  if (raw_command[0] == ':') {
    int index = raw_command.find_first_of(" ");
    source = raw_command.substr(1, index);
    raw_command = raw_command.substr(index+1);
  }

  if (raw_command[0] != 0)
    raw_command.substr(1);
  else 
    return;

  int index = raw_command.find_first_of(" ");
  command = raw_command.substr(0, index);
  if (index == std::string::npos)
    return;
  raw_command = raw_command.substr(index+1);

  if (raw_command[0] != 0)
    raw_command.substr(1);
  else 
    return;
  index = raw_command.find_first_of(" ");

  while (index != std::string::npos && raw_command[0] != ':') {
    this->parameters.push_back(raw_command.substr(0, index));
    raw_command = raw_command.substr(index + 1);
    index = raw_command.find_first_of(" ");
  }

  if (raw_command[0] == ':')
    this->parameters.push_back(raw_command.substr(1));
  else
    this->parameters.push_back(raw_command);
}
IRCCommand::IRCCommand(std::string source, std::string command) {
  this->source = source;
  this->command = command;
}
/*
template<typename... T>
IRCCommand::IRCCommand(std::string source, std::string command, T... params) {
  this->source = source;
  this->command = command;
  this->parameters = {params...};
}
*/

IRCCommand::~IRCCommand() {
  this->parameters.clear();
  this->parameters.shrink_to_fit();
}

bool IRCCommand::has_source() {
  return (source!="");
}

std::string IRCCommand::get_source() {
  return source;
}

bool IRCCommand::has_command() {
  return (command!="");
}

std::string IRCCommand::get_command() {
  return command;
}

bool IRCCommand::has_parameter() {
  return parameters.size() > 0;
}

std::string IRCCommand::get_parameter(int i) {
  return parameters.at(i);
}

int IRCCommand::get_parameter_count() {
  return parameters.size();
}

std::string IRCCommand::to_string() {
  std::string myself;
  if (has_source())
    myself += ":" + get_source() + " ";
  
  myself += get_command();
  
  for (auto i=parameters.begin(); i != parameters.end(); i++)
    myself += ((i==parameters.end()-1)&&(i->find(" ")!=std::string::npos)?" :":" ") + *i;

  return myself;
}