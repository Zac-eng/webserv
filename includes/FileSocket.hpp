#pragma once

#include "ASocket.hpp"
#include "Request.hpp"
#include "Response.hpp"


class FileSocket : public ASocket
{
	private:
		int _other_fd;
		Request _request;
		Response& _response;
		std::string _directory;
		std::string _file;

	public:
		FileSocket(int other_fd, Request& resquest, Response& response, std::string directory, std::string file);
		void handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
		void handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
		bool createSocket();
		void readFile();
		void getFileSize(void);
		void clposeAndDeleteSocket(std::map<int, ASocket*>& socket);
};