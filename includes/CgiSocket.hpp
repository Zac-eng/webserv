#pragma once

#include "ASocket.hpp"
#include "Request.hpp"
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <signal.h>

#define READ 0
#define WRITE 1
#define CMD_PATH "/bin/php"
#define CGI_TIMEOUT 3

struct Auth {
	std::string auth_type;
	std::string remote_user;
};

struct CgiPath {
	std::string script_name;
	std::string path_info;
	std::string query_parameter;
};

class CgiSocket : public ASocket
{

private:
	pid_t _cgi_pid;
	int _pipe_fds[2];
	time_t _created_at;
	const std::string& _request_body;
	std::string& _response_body;
	CgiSocket(void);
	
public:
	CgiSocket(pid_t cgi_pid, int read_fd, int write_fd, const std::string& request_body, std::string& response);
	~CgiSocket();
	CgiSocket(const CgiSocket& obj);
	CgiSocket& operator = (const CgiSocket& obj);
	CgiSocket* createCgiSocket(const ServerConfig& conf, const Request& req, const sockaddr_in& client_addr, std::string& response_buf);
	bool handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket);
	void handleEpollOutEvent();
	
};

namespace CgiMetaProcessors {
	Auth get_auth_info(const Request& req);
	CgiPath get_path_info(const Request& req);
	std::string get_content_length(const Request& req);
	std::string get_content_type(const Request& req);
	std::string get_gateway_interface(void);
	std::string get_path_info(const Request& req, const ServerConfig& conf);
	std::string get_path_translated(const Request& req, const ServerConfig& conf);
	std::string get_query_string(const Request& req);
	std::string get_remote_addr(const Request& req);
	std::string get_remote_host(const Request& req);
	// std::string get_remote_user(const Request& req);
	std::string get_request_method(const Request& req);
	std::string get_script_name(const Request& req, const ServerConfig& conf);
	std::string get_server_name(const ServerConfig& conf);
	std::string get_server_port(const ServerConfig& conf);
	std::string get_server_protocol(void);
	std::string get_server_software(void);
}

void close_pipes(int ptc_pipe[], int ctp_pipe[]);
const char **create_meta_vars(const Request& req);
