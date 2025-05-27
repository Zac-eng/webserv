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
		ServerConfig() : listen_port(0) , client_max_body_size(-1) {}
		void validate() const;
		void addListenPort(int port);
		void setIndex(const std::vector<std::string>& indexes);
		bool	parse_config(const std::string& filename, std::vector<ServerConfig>& configs);
		std::vector<int> listen_counts;

		int getListenPort() const;
		std::string getServerName() const;
		std::string getRootServer() const;
		std::vector<std::string> getIndexServer() const;
		const std::map<int, std::string> getErrorPages() const;
		const std::vector<int>& getListenPorts() const;
		const std::vector<int>& getListenCounts() const;
		const std::vector<LocationConfig>& getLocations() const;
		long int getClientMaxBodySize() const;

		void setListenPort(int port);
		void setServerName(const std::string& name);
		void setRootServer(const std::string& root);
		void setErrorPages(const std::map<int, std::string>& errors);
		void setClientMaxBodySize(long int size);
	private:
		int listen_port;
		long int client_max_body_size;
		std::string server_name;
		std::string root_server;
		std::vector<std::string> index_server;
		std::vector<int> index_server_count;
		std::map<int, std::string> error_pages;
		std::vector<int> listen_ports;
		std::vector<LocationConfig> locations;

		bool	check_server_block(std::ifstream& config_file, std::vector<ServerConfig>& configs);
		bool check_listen_name(std::ifstream& config_file, std::vector<ServerConfig>& configs);
};

int my_stoi(const std::string& str);
std::string trim(const std::string& str);
std::string	extract_quoted_string(const std::string& str);


#endif