#ifndef NGINX_HPP
#define NGINX_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <map>
#include "location.hpp"


class ServerConfig
{
	public:
		int listen_port;
		std::string server_name;
		std::string root_server;
		std::vector<std::string> index_server;
		std::vector<int> index_server_count;
		std::map<int, std::string> error_pages;
		std::vector<LocationConfig> locations;
		ServerConfig() : listen_port(0) {}
		void validate() const;
		void addListenPort(int port);
		void setIndex(const std::vector<std::string>& indexes);		bool	parse_config(const std::string& filename, std::vector<ServerConfig>& configs);
		std::vector<int> listen_counts;
		std::vector<int> listen_ports;

	private:
		bool	check_server_block(std::ifstream& config_file, std::vector<ServerConfig>& configs);
		bool check_listen_name(std::ifstream& config_file, std::vector<ServerConfig>& configs);
};

std::string trim(const std::string& str);
std::string	extract_quoted_string(const std::string& str);


#endif