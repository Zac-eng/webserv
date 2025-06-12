#pragma once

#include "ASocket.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "CgiMetaProcessors.hpp"
#include <ctime>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

enum NextEvent {
	CgiIn,
	CgiOut,
	ClientIn,
};

class CgiSocket : public ASocket
{

private:
	pid_t _cgi_pid;
	int _pipe_fds[2];
	time_t _created_at;
	Request& _req;
	std::string& _response_body;
	int	_client_fd;

	CgiSocket(void);
	static bool initPipes(int ptc[], int ctp[]);
	static bool prepareChildPipes(int ptc[], int ctp[]);
	static bool prepareParentPipes(int ptc[], int ctp[]);
	bool isTimeout(void);

public:
	// CgiSocket(ServerConfig& conf, pid_t cgi_pid, int read_fd, int write_fd, Request& req, std::string& response);
	CgiSocket(pid_t cgi_pid, int read_fd, int write_fd, Request& req, std::string& response, int client_fd);
	~CgiSocket();
	CgiSocket(const CgiSocket& obj);
	CgiSocket& operator = (const CgiSocket& obj);
	static CgiSocket* createCgiSocket(ServerConfig& conf, Request& req, const sockaddr_in& client_addr, std::string& response_buf, int client_fd);
	int		waitChildProcess() const;
	bool	createSocket(void);
	int		getReadPipe() const;
	int		getWritePipe() const;
	void	handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	void	handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket);

};

void close_pipes(int ptc_pipe[], int ctp_pipe[]);