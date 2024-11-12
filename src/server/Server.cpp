#include "Server.hpp"
#include "message/Response.hpp"

Server::Server(int port): _addr_len(sizeof(_address)) {
  memset(&_address, 0, _addr_len);
  _port = port;
}

int Server::Run() {
  int server_fd = launchServer();
  while (true) {
    int new_socket = accept(server_fd, (struct sockaddr *)&_address, (socklen_t*)&_addr_len);
    if (new_socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        continue;
    }
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    Response response(1.1, Response::OK);
    response.setHeader("Content-Type", "text/html");
    response.setBody("<h1>This is a test</h1><a href=\"https://google.com\">google</a>");
    response.setContentLength();
    response.Send(new_socket);
    close(new_socket);
  }
}

int Server::launchServer(void) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0) {
    std::cerr << "Socket failed" << std::endl;
    return -1;
  }
  _address.sin_family = AF_INET;
  _address.sin_addr.s_addr = INADDR_ANY;
  _address.sin_port = htons(_port);
  if (bind(server_fd, (struct sockaddr *)&_address, _addr_len) < 0) {
    std::cerr << "Bind failed" << std::endl;
    return -1;
  }
  if (listen(server_fd, 3) < 0) {
    std::cerr << "Listen failed" << std::endl;
    return -1;
  }
  return server_fd;
}
