#include "nginx.hpp"
#include "location.hpp"
// #include "location.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm> // std::remove_if
#include <functional>


void	ServerConfig::validate() const
{
	if (listen_port < 1 || listen_port > 65535)
		throw std::runtime_error("listen, Directive not specified ");
	if (server_name.empty())
	{
		std::cout << "server_name" << server_name << std::endl;
		throw std::runtime_error("server_name, Directive not specified");
	}
	for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it)
	{
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
	bool operator()(int c) const
	{
		return !is_space(c);
	}
};

//空白文字が削除された新しい文字列を返す 
std::string trim(const std::string& str)
{
	std::string result = str;
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), is_not_space()));
	result.erase(std::find_if(result.rbegin(), result.rend(), is_not_space()).base(), result.end());
	return result;
}

//""で囲まれた文字列を取り出す
std::string	extract_quoted_string(const std::string& str)
{
	size_t	start_quote = str.find('"');
	if (start_quote == std::string::npos)
	{
		size_t end_semicolon = str.find(';');
		if (end_semicolon == std::string::npos)
			return trim(str);
		else
			return trim(str.substr(0, end_semicolon));
	}
	size_t	end_quote = str.find('"', start_quote + 1);
	if (end_quote == std::string::npos)
	{
		std::cerr << "Error: quote is not closed." << str << std::endl;
		throw std::runtime_error("quote is not closed.");
	}
	// size_t	end_semicolon = str.find(';', end_quote + 1);
	std::string result = str.substr(start_quote + 1, end_quote - start_quote - 1);
	result = trim(result);
	return result;
}

void ServerConfig::addListenPort(int port)
{
	if (std::find(listen_ports.begin(), listen_ports.end(), port) == listen_ports.end())
		listen_ports.push_back(port);
}

void ServerConfig::setIndex(const std::vector<std::string>& indexes)
{
	index_server = indexes;
}

bool ServerConfig::check_listen_name(std::ifstream& config_file, std::vector<ServerConfig>& configs)
{
	std::string line;
	std::string block_content;
	bool has_open = false;
	bool set_path = false;
	bool close_server = false;
	int listen_number = 0;
	size_t prev_size = index_server.size();

	while (std::getline(config_file, line))
	{

		line = trim(line);
		block_content += line + "\n";
	}
	ServerConfig config;
	std::stringstream block_stream(block_content);
	LocationConfig location_config;
	while (std::getline(block_stream, line))
	{
		line = trim(line);
		if (line.empty())
			continue;
		std::stringstream block_line_stream(line);
		std::string block_keyword;
		block_line_stream >> block_keyword;
		if (block_keyword == "server")
		{
			if (close_server == false)
			{
				std::cerr << "Error: Missing closing '}' in server block." << std::endl;
				return false;
			}
			close_server = false;
			if (config.listen_port == 0)
			{
				addListenPort(80);
				config.listen_port = 80;
			}
			try
			{
				config.validate();
				configs.push_back(config);
			}
			catch(const std::runtime_error& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
				return false;
			}
			config = ServerConfig();
		}
		else if (block_keyword == "listen")
		{
			listen_number++;
			std::string port_str;
			block_line_stream >> port_str;
			if (!port_str.empty() && port_str.back() == ';')
				port_str.pop_back();
			else
			{
				std::cerr << "Error: Missing semicolon after 'listen' directive." << std::endl;
				return false;
			}
			try {
				int port = std::stoi(port_str);
				if (port < 1 || port > 65535)
				{
					std::cerr << "Error: Invalid port number: " << port_str << std::endl;
					return false;
				}
				config.listen_port = port;
				addListenPort(port);
			} catch (const std::exception& e) {
				std::cerr << "Error: Invalid port value in 'listen': " << port_str << std::endl;
				return false;
			}
		}
		else if (block_keyword == "server_name")
		{
			std::string value;
			std::getline(block_line_stream, value, ';');
			config.server_name = extract_quoted_string(value);
		}
		else if (block_keyword == "error_page")
		{
			int error_code;
			std::string error_path;
			block_line_stream >> error_code;
			std::getline(block_line_stream, error_path, ';');
			error_path = trim(error_path);
			if (error_code >= 400 && error_code <= 599)
				config.error_pages[error_code] = error_path;
			else
			{
				std::cerr << "Error: Invalid error code: " << line << std::endl;
				return false;
			}
		}
		else if (block_keyword == "root")
		{
			std::string root_path;
			block_line_stream >> root_path;
			if (!root_path.empty() && root_path.back() == ';')
				root_path.pop_back();
			else
			{
				std::cerr << "Error: Missing semicolon after 'root' directive." << std::endl;
				return false;
			}
			if (root_path.back() == '/')
				root_path.pop_back();
			config.root_server = trim(root_path);
			std::cout << "root_server: " << config.root_server << std::endl; 
		}
		else if (block_keyword == "index")
		{
			std::string file_index;
			while (block_line_stream >> file_index)
			{
				if (file_index.back() == ';')
				{
					file_index.pop_back();
					index_server.push_back(file_index);
					break;
				}
				index_server.push_back(file_index);
				if (index_server.size() >= prev_size)
					index_server_count.push_back(index_server.size());
				config.setIndex(index_server);
			}
		}
		else if (block_keyword == "location")
		{
			int i = 0;
			std::string location_path;
			std::string rest;
			std::string next_token;
			block_line_stream >> location_path;
			if (!(location_path[i] == '/' || location_path == "~"))
			{
				std::cerr << "Error: Invalid location path: " << location_path << std::endl;
				return false;
			}
			if (location_path == "~") {
				std::string line_php = block_line_stream.str();
				std::getline(block_line_stream, rest, '{');
				rest.erase(0, rest.find_first_not_of(" \t"));
				rest.erase(rest.find_last_not_of(" \t") + 1);
				location_path += " " + rest;
				if (!(location_path == "~ \\.php$"))
				{
					std::cerr << "Error: Invalid location path." << std::endl;
				}
				line.erase(line.find_last_not_of(" \t\n\r") + 1);
				if (!line.empty() && line.back() == '{'){
					std::cout << "here:" << std::endl;
					next_token = "{";
				}
			}
			location_config.setPath(location_path);
			block_line_stream >> next_token;
			std::cout << "next_token: " << next_token << std::endl;
			if (next_token == "{")
				has_open = true;
			set_path = true;

		}
		else if (block_keyword == "}")
		{
			listen_counts.push_back(listen_number);
			listen_number = 0;
			close_server = true;
		}
		if (set_path == true && block_keyword == "location")
		{
			if (has_open == false)
			{
				while (std::getline(block_stream, line))
				{
					line = trim(line);
					if (!line.empty())
					{
						if (line == "{")
							break;
						else
						{
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
	// if (count + 1 != i)
	// {
	// 	std::cerr << "Error: Missing closing '}' in server block." << std::endl;
	// 	return false;
	// }
	if (config.listen_port == 0)
	{
		config.listen_port = 80;
	}
	try
	{
		config.validate();
		configs.push_back(config);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
	return true;
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
		if (keyword == "server")
		{
			std::string next_token;
			if (!(stream >> next_token) || next_token != "{")
			{
				while (std::getline(config_file, line))
				{
					line = trim(line);
					if (!line.empty())
					{
						if (line == "{")
							break;
						else
						{
							std::cerr << "Error: Expected '{' after 'server' directive." << std::endl;
							return false;
						}
					}
				}
			}
			if (!check_listen_name(config_file, configs))
				return false;
		}
		else
		{
			std::cerr << "Error: Unexpected directive in http block: " << line << std::endl;
			return false;
		}
	}
	// if (has_closing == false)
	// {
	// 	std::cerr << "Error: Missing closing '}' in http block." << std::endl;
	// 	return false;
	// }
	return true;
}

bool	ServerConfig::parse_config(const std::string& filename, std::vector<ServerConfig>& configs)
{
	std::ifstream config_file(filename);
	if (!config_file.is_open())
	{
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
		if (keyword == "http")
		{
			if (http_found)
			{
				std::cerr << "Errpr: Multiple 'http' blocks are not allowed." << std::endl;
				return false;
			}
			std::string next_token;
			if (!(stream >> next_token) || next_token != "{")
			{
				while (std::getline(config_file, line))
				{
					line = trim(line);
					if (!line.empty())
					{
						if (line == "{")
							break;
						else
						{
							std::cerr << "Error: Expected '{' after 'http' directive." << std::endl;
							return false;
						}
					}
				}
			}
			if (!check_server_block(config_file, configs))
				return false;
		}
	}
	return true;
}
