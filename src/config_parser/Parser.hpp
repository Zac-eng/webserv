#pragma once
#ifndef PARSER_HPP
# define PARSER_HPP

# include "server/Server.hpp"
# include <fstream>

class Parser {

private:
  Parser(void);
  ~Parser();

public:
  Server parseConfig(const std::string& config_path);
  class DirectiveException: std::exception {
    public:
      DirectiveException(const std::string& directive, int line);
      std::string _directive;
      int _line;
  };
  class InvalidArgException: std::exception {
    public:
      InvalidArgException(const std::string& directive, int line);
      std::string _directive;
      int _line;
  };

};

#endif
