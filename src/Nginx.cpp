#include "nginx.hpp"

ServerConfig::ServerConfig(int port, std::string server) : _listen_port(port), \
		_server_name(server) {};

ServerConfig::ServerConfig() {};
ServerConfig::~ServerConfig() {};
