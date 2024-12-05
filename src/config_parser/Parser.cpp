#include "config_parser/Parser.hpp"

Server Parser::parseConfig(const std::string& config_path) {
  std::ifstream config_stream(config_path);

  if (!config_stream.is_open()) {
    std::cerr << "No such file or directory: " << config_path << std::endl;
    throw std::exception();
  }
  parseConfFirstLine(config_stream);
  
}

int Parser::parseConfFirstLine(std::ifstream& ifs) {
  std::string line;
  std::size_t pos = 0;
  std::size_t len;

  if (!std::getline(ifs, line)) {
    return 1;
  }
  len = line.length();
  skipBlank(line, pos);
  if (line.substr(pos, 4) != "http")
    return 1;
  pos += 4;
  skipBlank(line, pos);
  if (pos != len && line[pos] != '{')
    return 1;
  skipBlank(line, pos);
  if (pos != len)
    return 1;
  return 0;
}

void Parser::skipBlank(std::string str, std::size_t& pos) {
  std::size_t len = str.length();

  while (pos < len && std::isspace(str[pos])) {
    ++pos;
  }
}
