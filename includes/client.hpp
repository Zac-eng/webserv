#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#define PORT 8080
#define MAX_SIZE 1024

class Client {
	private:
		int _client_fd;
		sockaddr_in address;
	public:
		Client(std::string &server_ip);
		~Client();
		void connect();
		void send(std::string &message);
		void receive();
};