#include "message/Response.hpp"
#include "message/Request.hpp"
#include "cgi/CgiClient.hpp"
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

// int main(int argc, char* argv[]) {
//   (void)argv;
//   Response dummy(1.1, Response::OK);
//   dummy.setHeader("Content-Type", "text/html");
//   dummy.setBody("<hi>Hello World!<h1>");

//   if (argc != 2) {
//     return 1;
//   }
//   int server_fd, new_socket;
//   struct sockaddr_in address;
//   int addrlen = sizeof(address);

//   // Create socket file descriptor
//   server_fd = socket(AF_INET, SOCK_STREAM, 0);
//   if (server_fd == 0) {
//       std::cerr << "Socket failed" << std::endl;
//       return -1;
//   }

//   // Set up the address and port
//   address.sin_family = AF_INET;
//   address.sin_addr.s_addr = INADDR_ANY;
//   address.sin_port = htons(PORT);

//   // Bind the socket to the port
//   if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//     std::cerr << "Bind failed" << std::endl;
//     return -1;
//   }

//     // Listen for incoming connections
//   if (listen(server_fd, 3) < 0) {
//     std::cerr << "Listen failed" << std::endl;
//       return -1;
//   }

//   std::cout << "Server is running on port " << PORT << std::endl;
//   while (true) {
//         // Accept a new connection
//     new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
//     if (new_socket < 0) {
//       std::cerr << "Accept failed" << std::endl;
//       continue;
//     }
//     dummy.Send(new_socket);
//     close(new_socket);
//   }
//   return 0;
// }

int main(void) {
  Request request(1.1, Request::GET, "index.html");
  request.setHeader("Host", "localhost:8080");
  request.setHeader("Accept", "text/html");
  request.setHeader("Connection", "keep-alive");
  request.setHeader("Content-Type", "application/json");
  CgiClient cgi("127.0.0.1", 8080);
  Response response = cgi.sendRequest(request);
  response.Send(1);
  return 0;
}
