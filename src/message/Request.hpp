#pragma once
#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "AMessage.hpp"

class Request: public AMessage {

public:
  enum Method {
    GET,
    POST,
    DELETE,
  };
  Request(int stream_fd);
  // construct Request with necessary components. will be used in server use
  Request(double version, Method method, const std::string& path);

  ~Request();

private:
  Request(void);

  Method _method;
  std::string _path;

  std::string flattenContents(void) const;
  std::string resolveMethod(Method method) const;
};

#endif
