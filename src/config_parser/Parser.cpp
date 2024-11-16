#include "config_parser/Parser.hpp"

Server Parser::parseConfig(const std::string& config_path) {
  std::ifstream config_stream(config_path);
  std::string line;

  if (!config_stream.is_open()) {
    std::cerr << "No such file or directory: " << config_path << std::endl;
    throw std::exception();
  }
  getline(config_stream, line);
  if (line.substr(0, 4) != "http")
    throw DirectiveException(, 1);
}
