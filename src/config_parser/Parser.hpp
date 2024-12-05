#pragma once
#ifndef PARSER_HPP
# define PARSER_HPP

# include "server/Server.hpp"
# include <fstream>

class Parser {

private:
  Parser(void);
  ~Parser();
  Parser(const Parser& object);
  Parser& operator = (const Parser& object);

  static int parseConfFirstLine(std::ifstream& ifs);
  static void skipBlank(std::string str, std::size_t& it);

public:
  static Server parseConfig(const std::string& config_path);

};

#endif
