#pragma once
#ifndef CGI_CLIENT_HPP
# define CGI_CLIENT_HPP

# include "message/Request.hpp"
# include "message/Response.hpp"
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>

class CgiClient {

private:
  CgiClient(void);
  std::string _cgi_ip;
  int _cgi_port;

public:
  CgiClient(const std::string& cgi_ip, int cgi_port);
  ~CgiClient();
  Response sendRequest(const Request& request) const;
  class SocketCreationException: std::string {};

};

#endif
