#include "nginx.hpp"
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
		throw std::runtime_error("server_name, Directive not specified");
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
	std::cout << "extract_in" << std::endl;
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
	size_t	end_semicolon = str.find(';', end_quote + 1);
	std::string result = str.substr(start_quote + 1, end_quote - start_quote - 1);
	result = trim(result);
	return result;
}


bool	check_listen_name(std::ifstream& config_file, std::vector<ServerConfig>& configs)
{
	std::string line;
	std::string block_content;
	bool has_closing = false;

	while (std::getline(config_file, line))
	{
		line = trim(line);
		if (line == "}")
		{
			has_closing = true;
			break;
		}
		block_content += line + "\n";
	}
	if (!has_closing)
	{
		std::cerr << "Error: Missing closing '}' in server block." << std::endl;
		return false;
	}
	ServerConfig config;
	std::stringstream block_stream(block_content);
	std::string block_line;

	while (std::getline(block_stream, block_line))
	{
		std::stringstream block_line_stream(block_line);
		std::string block_keyword;
		block_line_stream >> block_keyword;
		if (block_keyword == "listen")
		{
			std::cout << "listen_in" << std::endl;
			block_line_stream >> config.listen_port;
		}
		else if (block_keyword == "server_name")
		{
			std::cout << "server_in" << std::endl;
			std::string	value;
			std::getline(block_line_stream, value, ';');				
			config.server_name = extract_quoted_string(value);
		}
	// 	else if (block_keyword == "location")
	// 	{
	// 		LocationConfig location_config;

	// 		std::string location_path;
	// 		block_line_stream >> location_path;
	// 		location_config.path = location_path;

	// 		std::string next_token;
	// 		if (!(block_line_stream >> next_token) || next_token != "{")
	// 		{
	// 			while (std::getline(config_file, line))
	// 			{
	// 				if (!line.empty())
	// 				{
	// 					if (line == "{")
	// 						break;
	// 					else
	// 					{
	// 						std::cerr << "Error: Expected '{' after 'location' directive." << std::endl;
	// 						return false;
	// 					}
	// 				}
	// 			}
	// 		}
	// 		if (!check_location(config_file, location_config))
	// 			return false;
	// 		config.locations.push_back(location_config);
	// 	}
	}
	if (config.listen_port == '\0')
		config.listen_port = 80;
	try
	{
		config.validate();
		configs.push_back(config);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error" << e.what() << std::endl;
		return false;
	}
	return true;
}


bool	check_server_block(std::ifstream& config_file, std::vector<ServerConfig>& configs)
{
	std::string	line;
	bool	has_closing = false;
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
		else if (line == "}")
		{
			has_closing = true;
			break;
		}
		else
		{
			std::cerr << "Error: Unexpected directive in http block: " << line << std::endl;
			return false;
		}

	}
	if (!has_closing)
	{
		std::cerr << "Error: Missing closing '}' in http block." << std::endl;
		return false;
	}
	return true;
}

bool	parse_config(const std::string& filename, std::vector<ServerConfig>& configs)
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
		std::cout << line << std::endl;
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
			// if (std::getline(config_file, line))
			// {
			// 	line = trim(line);
			// 	if (line != "}")
			// 	{
			// 		std::cerr << "Error: Missing closing '}' for 'http' block." << std::endl;
			// 		return false;
			// 	}
			// 	else
			// 	{
			// 		std::cerr << "Error: Config file must start with 'http' block." << std::endl;
			// 		return false;
			// 	}
			// }
		}
		// else
		// {
		// 	std::cerr << "Error: Config file must start with 'http'." << std::endl;
		// 	return false;
		// }
	}
	return true;
}
