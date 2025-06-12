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
#include <climits>


void	ServerConfig::validate() const
{
	if (listen_port < 1 || listen_port > 65535)
		throw std::runtime_error("listen, Directive not specified ");
	if (server_name.empty()) {
		std::cout << "server_name" << server_name << std::endl;
		throw std::runtime_error("server_name, Directive not specified");
	}
	for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
		if (it->first < 400 || it->first > 599)
			throw std::runtime_error("Error: Invalid error code.");
		if (it->second.empty())
			throw std::runtime_error("Error: error_page path is missing.");
	}
}

bool	is_space(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}

struct is_not_space
{
	bool operator()(int c) const {
		return !is_space(c);
	}
};

int my_stoi(const std::string& str)
{
	if (str.empty())
		throw std::invalid_argument("Input string is empty");
	const char* s = str.c_str();
	char* end_ptr;
	errno = 0;
	long converted_value = std::strtol(s, &end_ptr, 10);
	if (end_ptr == s)
		throw std::invalid_argument("No conversion could be performed");
	char* check_after_num = end_ptr;
	while (*check_after_num != '\0') {
		if (!std::isspace(static_cast<unsigned char>(*check_after_num))) {
			throw std::invalid_argument("Invalid characters after number");
		}
		check_after_num++;
	}
	if (errno == ERANGE || converted_value < INT_MIN || converted_value > INT_MAX)
		throw std::out_of_range("Value out of range for int");
	return static_cast<int>(converted_value);
}

std::string trim(const std::string& str)
{
	std::string result = str;
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), is_not_space()));
	result.erase(std::find_if(result.rbegin(), result.rend(), is_not_space()).base(), result.end());
	return result;
}

std::string	extract_quoted_string(const std::string& str)
{
	size_t	start_quote = str.find('"');
	if (start_quote == std::string::npos) {
		size_t end_semicolon = str.find(';');
		if (end_semicolon == std::string::npos)
			return trim(str);
		else
			return trim(str.substr(0, end_semicolon));
	}
	size_t	end_quote = str.find('"', start_quote + 1);
	if (end_quote == std::string::npos) {
		std::cerr << "Error: quote is not closed." << str << std::endl;
		throw std::runtime_error("quote is not closed.");
	}
	std::string result = str.substr(start_quote + 1, end_quote - start_quote - 1);
	result = trim(result);
	return result;
}

void ServerConfig::addListenPort(int port)
{
	listen_ports.push_back(port);	
}

void ServerConfig::setIndex(const std::vector<std::string>& indexes)
{
	index_server = indexes;
}

bool ServerConfig::process_server_blocks(const std::string& block_content, std::vector<ServerConfig>& configs)
{
	std::stringstream block_stream(block_content);
	std::string line;
	ServerConfig config;
	LocationConfig location_config;
	bool has_open = false;
	bool set_path = false;
	bool has_root = false;
	int listen_number = 0;
	size_t prev_size = index_server.size();

	while (std::getline(block_stream, line))
	{
		line = trim(line);
		if (line.empty()) continue;

		std::stringstream block_line_stream(line);
		std::string block_keyword;
		block_line_stream >> block_keyword;

		if (block_keyword == "server") {
			std::string next_token;
			if (!(block_line_stream >> next_token) || next_token != "{") {
				while (std::getline(block_stream, line)) {
					line = trim(line);
					if (!line.empty()) {
						if (line == "{") {
							num_open++;
							break;
						} else {
							std::cerr << "Error: Expected '{' after 'server' directive." << std::endl;
							return false;
						}
					}
				}
			} else {
				num_open++;
			}
			if (!handle_server(config, configs, listen_number))
				return false;
			has_root = false;
		} else if (block_keyword == "listen") {
			if (!handle_listen(block_line_stream, config, listen_number)) return false;
		} else if (block_keyword == "client_max_body_size") {
			if (!handle_client_max_body_size(block_line_stream, config)) return false;
		} else if (block_keyword == "server_name") {
			if (!handle_server_name(block_line_stream, config)) return false;
		} else if (block_keyword == "error_page") {
			if (!handle_error_page(block_line_stream, config)) return false;
		} else if (block_keyword == "root") {
			has_root = true;
			if (!handle_root(block_line_stream, config)) return false;
		} else if (block_keyword == "index") {
			handle_index(block_line_stream, config, prev_size);
		} else if (block_keyword == "#") {
			continue;
		} else if (block_keyword == "location") {
			if (!handle_location(block_line_stream, has_root, line, has_open, set_path, location_config)) return false;
		} else if (block_keyword == "{") {
			num_open++;
		} else if (block_keyword == "}") {
			handle_close_brace(config, listen_number);
			num_close++;
		} else {
			std::cerr << "Error: Unexpected directive in server block: " << line << std::endl;
			return false;
		}
		if (set_path == true && block_keyword == "location") {
			if (has_open == false) {
				while (std::getline(block_stream, line))
				{
					line = trim(line);
					if (!line.empty()) {
						if (line == "{")
							break;
						else {
							std::cerr << "Error: Expected '{' after 'location' directive." << std::endl;
							return false;
						}
					}
				}
			}
			if (!(location_config.check_location(block_stream, location_config)))
				return false;
			has_open = false;
			set_path = false;
			config.locations.push_back(location_config);
		}
	}
	if (config.listen_port == 0) {
		config.listen_port = 80;
	}
	try {
		config.validate();
		configs.push_back(config);
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
	return true;
}


bool ServerConfig::check_listen_name(std::ifstream& config_file, std::vector<ServerConfig>& configs)
{
	std::string block_content, line;
	while (std::getline(config_file, line))
		block_content += trim(line) + "\n";
	return process_server_blocks(block_content, configs);
}

bool	ServerConfig::check_server_block(std::ifstream& config_file, std::vector<ServerConfig>& configs)
{
	std::string	line;
	while (std::getline(config_file, line))
	{
		line = trim(line);
		if (line.empty())
			continue;
		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;
		if (keyword == "server") {
			std::string next_token;
			if (!(stream >> next_token) || next_token != "{") {
				while (std::getline(config_file, line))
				{
					line = trim(line);
					if (!line.empty()) {
						if (line == "{"){
							num_open++;
							break;
						}
						else {
							std::cerr << "Error: Expected '{' after 'server' directive." << std::endl;
							return false;
						}
					}
				}
			}
			else
				num_open++;
			if (!check_listen_name(config_file, configs))
				return false;
		}
		else {
			std::cerr << "Error: Unexpected directive in http block: " << line << std::endl;
			return false;
		}
	}
	return true;
}

bool	ServerConfig::parse_config(const std::string& filename, std::vector<ServerConfig>& configs)
{
	std::ifstream config_file(filename.c_str());
	num_open = 0;
	num_close = 0;
	if (!config_file.is_open()) {
		std::cerr << "file can't open" << filename << std::endl;
		return false;
	}
	std::string	line;
	bool	http_found = false;
	while (std::getline(config_file, line))
	{
		line = trim(line);
		if (line.empty())
			continue;
		std::stringstream stream(line);
		std::string keyword;
		stream >> keyword;
		if (keyword == "http") {
			if (http_found) {
				std::cerr << "Errpr: Multiple 'http' blocks are not allowed." << std::endl;
				return false;
			}
			std::string next_token;
			if (!(stream >> next_token) || next_token != "{") {
				while (std::getline(config_file, line))
				{
					line = trim(line);
					if (!line.empty()) {
						if (line == "{") {
							num_open++;
							break;
						}
						else {
							std::cerr << "Error: Expected '{' after 'http' directive." << std::endl;
							return false;
						}
					}
				}
			}
			else
				num_open++;
			if (!check_server_block(config_file, configs))
				return false;
		} else if (keyword == "#")
			continue;
		else {
			std::cerr << "Error: Unexpected directive outside 'http' block: " << line << std::endl;
			return false;
		}

	}
	if (num_open != num_close) {
		std::cerr << "Error: Mismatched number of opening and closing braces." << std::endl;
		return false;
	}
	return true;
}
