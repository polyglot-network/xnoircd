#ifndef IRCCOMMAND_H
#define IRCCOMMAND_H

#include <unordered_map>
#include <string>
#include <vector>

class IRCCommand {
private:
  std::unordered_map<std::string, std::string> tags;
  std::string source, command = "";
  std::vector<std::string> parameters;
public:
  IRCCommand();
  IRCCommand(std::string raw_command);
  IRCCommand(std::string source, std::string command);
  template<typename... T>
  IRCCommand(std::string source, std::string command, T... params) {
    this->source = source;
    this->command = command;
    this->parameters = {params...};
  }

  ~IRCCommand();

  bool has_source();

  std::string get_source();

  bool has_command();

  std::string get_command();

  bool has_parameter();

  std::string get_parameter(int i);

  int get_parameter_count();

  std::string to_string();
};

#endif