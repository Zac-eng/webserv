#include "nginx.hpp"
#include "location.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <stdexcept>

bool ServerConfig::handle_listen(std::stringstream& block_line_stream, ServerConfig& config, int& listen_number) {
	listen_number++;
	std::string port_str;
	block_line_stream >> port_str;
	if (!port_str.empty() && port_str[port_str.size() - 1] == ';')
		port_str.erase(port_str.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'listen' directive." << std::endl;
		return false;
	}
	try {
		int port = my_stoi(port_str);
		if (port < 1 || port > 65535) {
			std::cerr << "Error: Invalid port number: " << port_str << std::endl;
			return false;
		}
		config.listen_port = port;
		config.addListenPort(port);
	} catch (const std::exception& e) {
		std::cerr << "Error: Invalid port value in 'listen': " << port_str << std::endl;
		return false;
	}
	return true;
}

bool ServerConfig::handle_root(std::stringstream& block_line_stream, ServerConfig& config) {
	std::string root_path;
	block_line_stream >> root_path;
	if (!root_path.empty() && root_path[root_path.size() - 1] == ';')
    	root_path.erase(root_path.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'root' directive." << std::endl;
		return false;
	}
	if (root_path[root_path.size() - 1] == '/')
		root_path.erase(root_path.size() - 1);
	config.root_server = trim(root_path);
	return true;
}

bool ServerConfig::handle_server_name(std::stringstream& block_line_stream, ServerConfig& config) {
	std::string value;
	std::getline(block_line_stream, value);
	if (value.empty() || value[value.size() - 1] != ';') {
		std::cerr << "Error: Missing semicolon after 'server_name' directive." << std::endl;
		return false;
	}
	value.erase(value.size() - 1);
	config.server_name = extract_quoted_string(value);
	return true;
}

bool ServerConfig::handle_index(std::stringstream& block_line_stream, ServerConfig& config, size_t prev_size) {
	std::string file_index;
	while (block_line_stream >> file_index) {
		if (file_index[file_index.size() - 1] == ';') {
			file_index.erase(file_index.size() - 1);
			index_server.push_back(file_index);
			break;
		}
		index_server.push_back(file_index);
		if (index_server.size() >= prev_size)
			index_server_count.push_back(index_server.size());
		config.setIndex(index_server);
	}
	return true;
}

bool ServerConfig::handle_error_page(std::stringstream& block_line_stream, ServerConfig& config) {
	int error_code;
	std::string error_path;
	block_line_stream >> error_code;
	std::getline(block_line_stream, error_path);
	if (error_path.empty() || error_path[error_path.size() - 1] != ';') {
		std::cerr << "Error: Missing semicolon after 'error_page' directive." << std::endl;
		return false;
	}
	error_path.erase(error_path.size() - 1);
	error_path = trim(error_path);
	if (error_code < 400 || error_code > 599) {
		std::cerr << "Error: Invalid error code: " << error_code << std::endl;
		return false;
	}
	config.error_pages[error_code] = error_path;
	return true;
}

bool ServerConfig::handle_client_max_body_size(std::stringstream& block_line_stream, ServerConfig& config) {
	std::string size_str;
	block_line_stream >> size_str;
	if (!size_str.empty() && size_str[size_str.size() - 1] == ';')
		size_str.erase(size_str.size() - 1);
	else {
		std::cerr << "Error: Missing semicolon after 'client_max_body_size' directive." << std::endl;
		return false;
	}
	try {
		char unit = '\0';
		if (!size_str.empty() && std::isalpha(size_str[size_str.size() - 1])) {
			unit = std::toupper(size_str[size_str.size() - 1]);
			size_str = size_str.substr(0, size_str.size() - 1);
		}
		long int base_size = my_stoi(size_str);
		if (base_size < 0) {
			std::cerr << "Error: Negative size in 'client_max_body_size'." << std::endl;
			return false;
		}
		long int final_size = base_size;
		switch (unit) {
			case 'K': final_size *= 1024; break;
			case 'M': final_size *= 1024 * 1024; break;
			case 'G': final_size *= 1024 * 1024 * 1024; break;
			case '\0': break;
			default:
				std::cerr << "Error: Invalid unit for 'client_max_body_size': " << unit << std::endl;
				return false;
		}
		config.client_max_body_size = final_size;
	} catch (const std::exception& e) {
		std::cerr << "Error: Invalid size value in 'client_max_body_size'." << std::endl;
		return false;
	}
	return true;
}

bool ServerConfig::handle_location(std::stringstream& block_line_stream, bool& has_root, std::string& line, bool& has_open, bool& set_path, LocationConfig& location_config) {
	if (has_root != true) {
		std::cerr << "Error: 'root' directive is missing in server block, required for location '/'." << std::endl;
		return false;
	}
	int i = 0;
    std::string location_path;
	std::string rest;
	std::string next_token;
	block_line_stream >> location_path;
	if (!(location_path[i] == '/' || location_path == "~")) {
		std::cerr << "Error: Invalid location path: " << location_path << std::endl;
		return false;
	}
	if (location_path == "~") {
    	std::string line_php = block_line_stream.str();
		std::getline(block_line_stream, rest, '{');
		rest.erase(0, rest.find_first_not_of(" \t"));
		rest.erase(rest.find_last_not_of(" \t") + 1);
		location_path += " " + rest;
		if (!(location_path == "~ \\.php$")) {
			std::cerr << "Error: Invalid location path." << std::endl;
		}
		line.erase(line.find_last_not_of(" \t\n\r") + 1);
		if (!line.empty() && line[line.size() - 1] == '{'){
			next_token = "{";
		}
	}
	location_config.setPath(location_path);
	block_line_stream >> next_token;
	if (next_token == "{")
		has_open = true;
	set_path = true;
	return true;
}

bool ServerConfig::handle_server(ServerConfig& config, std::vector<ServerConfig>& configs, int& listen_number) {
	if (!listen_dupli) {
		if (config.listen_port == 0) {
			listen_number++;
			addListenPort(80);
			config.listen_port = 80;
		}
		try {
			config.validate();
			configs.push_back(config);
		} catch (const std::runtime_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return false;
		}
	}
	set_default_listen = false;
	config = ServerConfig();
	listen_dupli = false;
	return true;
}

bool ServerConfig::handle_close_brace(ServerConfig& config, int& listen_number) {
	// if (config.listen_port == 0) {
	// 	listen_number++;
	// 	addListenPort(80);
	// 	config.listen_port = 80;
	// }

	// listen_counts.push_back(listen_number);
	// listen_number = 0;

	// return true;
	const std::vector<int>& ports = config.getListenPorts();
	if (config.listen_port == 0) {
		set_default_listen = true;
		addListenPort(80);
		config.setListenPort(80);
		listen_number = 1;

	}
	else {
		config.setListenPort(ports[0]);
		listen_number = ports.size();
	}
	listen_counts.push_back(listen_number);
	listen_number = 0;
	return true;
}
