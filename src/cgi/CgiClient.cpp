#include "CgiClient.hpp"

CgiClient::CgiClient(const std::string& cgi_ip, int cgi_port) {
  _cgi_ip = cgi_ip;
  _cgi_port = cgi_port;
}

CgiClient::~CgiClient() {}

Response CgiClient::sendRequest(const Request& request) const {
  int sock_fd;
  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(_cgi_port);
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1)
    throw SocketCreationException();
  if (inet_pton(AF_INET, _cgi_ip.c_str(), &server_addr.sin_addr) <= 0) {
    close(sock_fd);
    throw std::exception();
  }
  if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    close(sock_fd);
    throw std::exception();
  }
  request.Send(sock_fd);
  close(sock_fd);
  return Response(1.1, Response::OK);
}
