#include "nginx.hpp"
#include "location.hpp"
#include <iostream>

int ServerConfig::getListenPort() const
{
	return listen_port;
}

std::string ServerConfig::getServerName() const {
	return server_name;
}

std::string ServerConfig::getRootServer() const {
	return root_server;
}

long int ServerConfig::getClientMaxBodySize() const {
	return client_max_body_size;
}

const std::map<int, std::string> ServerConfig::getErrorPages() const {
	return error_pages;
}

const std::vector<int>& ServerConfig::getListenPorts() const {
	return this->listen_ports;
}

const std::vector<int>& ServerConfig::getListenCounts() const {
	return this->listen_counts;
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const { 
	return locations; 
}

void ServerConfig::setListenPort(int port) {
	listen_port = port;
}

void ServerConfig::setServerName(const std::string& name) {
	server_name = name;
}

void ServerConfig::setRootServer(const std::string& root) {
	root_server = root;
}

void ServerConfig::setErrorPages(const std::map<int, std::string>& errors) {
	error_pages = errors;
}

void ServerConfig::setClientMaxBodySize(long int size) {
	client_max_body_size = size;
}