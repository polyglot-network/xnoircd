#include "irccommand.h"

#include <iostream>

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
  raw_command = raw_command.substr(index+1);

  if (raw_command[0] != 0)
    raw_command.substr(1);
  else 
    return;

  std::vector<std::string> payloads;
  index = raw_command.find_first_of(" ");

  while (index != std::string::npos && raw_command[0] != ':') {
    payloads.push_back(raw_command.substr(0, index));
    raw_command = raw_command.substr(index + 1);
    index = raw_command.find_first_of(" ");
  }

  if (raw_command[0] == ':')
    payloads.push_back(raw_command.substr(1));
  else
    payloads.push_back(raw_command);
  
  this->payload = payloads;
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

bool IRCCommand::has_payload() {
  return payload.size() > 0;
}

std::string IRCCommand::get_payload(int i) {
  return payload.at(i);
}

int IRCCommand::get_payload_count() {
  return payload.size();
}

std::string IRCCommand::to_string() {
  std::string myself;
  if (has_source())
    myself += "Source: " + get_source() + "\n";
  if (has_command())
    myself += "Command: " + get_command() + "\n";
  for (std::string p : payload)
    myself += "Parameters: " + p + "\n";
  return myself;
}