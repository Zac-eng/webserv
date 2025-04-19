#pragma once

#include "nginx.hpp"
#include "Request.hpp"
#include <string>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define READ 0
#define WRITE 1
#define CMD_PATH "/bin/php"
#define CGI_EXTENTION ".php"

const char **create_meta_vars(const ServerConfig& conf, const Request& req, const sockaddr_in& addr);

struct Auth {
	std::string auth_type;
	std::string remote_user;
};

struct CgiPath {
	std::string script_name;
	std::string path_info;
	std::string translated;
	std::string query_string;
};

struct RemoteInfo {
	std::string remote_addr;
	std::string remote_host;
};

namespace CgiMetaProcessors {
	Auth get_auth_info(const Request& req);
	CgiPath get_path_info(const ServerConfig& conf, const Request& req);
	RemoteInfo get_remote_info(const sockaddr_in& client_addr);
	std::string get_content_length(const Request& req);
	std::string get_content_type(const Request& req);
	std::string get_gateway_interface(void);
	std::string get_path_info(const Request& req, const ServerConfig& conf);
	std::string get_path_translated(const Request& req, const ServerConfig& conf);
	std::string get_query_string(const Request& req);
	std::string get_request_method(const Request& req);
	std::string get_script_name(const Request& req, const ServerConfig& conf);
	std::string get_server_name(const ServerConfig& conf);
	std::string get_server_port(const ServerConfig& conf);
	std::string get_server_protocol(void);
	std::string get_server_software(void);
}
