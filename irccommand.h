#ifndef IRCCOMMAND_H
#define IRCCOMMAND_H

#include <unordered_map>
#include <string>
#include <vector>

class IRCCommand {
private:
  std::unordered_map<std::string, std::string> tags;
  std::string source, command = "";
  std::vector<std::string> payload;
public:
  IRCCommand(std::string raw_command);

  bool has_source();

  std::string get_source();

  bool has_command();

  std::string get_command();

  bool has_payload();

  std::string get_payload(int i);

  int get_payload_count();

  std::string to_string();
};

#endif