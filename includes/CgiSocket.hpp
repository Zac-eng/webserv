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

#define READ 0
#define WRITE 1
#define CMD_PATH "/bin/php"
#define CGI_EXTENTION ".php"
#define CGI_TIMEOUT 3

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
	size_t& _status_code;
	const std::string& _request_body;
	std::string& _response_body;

	CgiSocket(void);
	bool initPipes(int ptc[], int ctp[]);
	bool prepareChildPipes(int ptc[], int ctp[]);
	bool prepareParentPipes(int ptc[], int ctp[]);
	bool isTimeout(void);

public:
	CgiSocket(ServerConfig& conf, pid_t cgi_pid, int read_fd, int write_fd, Request& req, std::string& response);
	~CgiSocket();
	CgiSocket(const CgiSocket& obj);
	CgiSocket& operator = (const CgiSocket& obj);
	CgiSocket* createCgiSocket(ServerConfig& conf, Request& req, const sockaddr_in& client_addr, std::string& response_buf);
	bool	createSocket(void);
	bool	handleEpollInEvent();
	bool	handleEpollOutEvent();

};

void close_pipes(int ptc_pipe[], int ctp_pipe[]);
