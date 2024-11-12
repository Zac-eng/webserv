#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include "message/Request.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>

class Server {

public:
  // Server(int config_fd);
  Server(int port);
  ~Server();
  int Run(void);
  class SocketCreationException: std::exception {};

private:
  Server(void);
  struct sockaddr_in _address;
  const int _addr_len;
  int _port;
  std::map<std::string, std::string> _options;
  struct pollfd _incoming_requests[OPEN_MAX];
  int launchServer(void);

};

#endif
