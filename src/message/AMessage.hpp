#pragma once
#ifndef A_MESSAGE_HPP
# define A_MESSAGE_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>
# include <exception>
# include <algorithm>
# include <sstream>
# include <sys/socket.h>


class AMessage {

public:
  void setHeader(const std::string& key, const std::string& value);
  void setContentLength(void);
  void setBody(const std::string& body);
  void Send(int stream_fd);

protected:
  AMessage(void);
  virtual ~AMessage();

  // necessary components
  double _version;

  // optional, can be "empty"
  std::map<std::string, std::vector<std::string> > _headers;
  std::string _message_body;

  // this exception will be used inside of setHeader
  class BadHeaderException: std::exception {};

  // internal functions
  virtual std::string flattenContents(void) const = 0;
  bool canDuplicate(const std::string header) const;

};

#endif
