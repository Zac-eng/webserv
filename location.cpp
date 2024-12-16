#include "location.hpp"
#include "nginx.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm> // std::remove_if
#include <functional>


// bool	check_location(std::ifstream& config_file, LocationConfig& location_config)
// {
// 	std::cout << "location_config" << location_config.getPath() << std::endl;	
// 	std::string line;
// 	bool has_closing = false;

// 	while (std::getline(config_file, line))
// 	{
// 		std::cout << "aline" << line << std::endl;
// 		line = trim(line);
// 		if (line.empty())
// 			continue;
// 		if (line == "}")
// 		{
// 			has_closing = true;
// 			break;
// 		}
// 		std::stringstream line_stream(line);
// 		std::string keyword;
// 		line_stream >> keyword;
// 		if (keyword == "root")
// 		{
// 			std::string root_value;
// 			line_stream >> root_value;
// 			location_config.setRoot(root_value);
// 		}
// 		else if (keyword == "index")
// 		{
// 			std::string index_value;
// 			line_stream >> index_value;
// 			location_config.setIndex(index_value);
// 		}
// 		else if (keyword == "allow_methods")
// 		{
// 			std::string method;
// 			while (line_stream >> method)
// 			{
// 				location_config.addAllowedMethod(method);
// 			}
// 		}
// 		else
// 		{
// 			std::cerr << "Error: Unexpected directive in location block: " << keyword << std::endl;
// 			return false;
// 		}
// 	}
// 	if (!has_closing)
// 	{
// 		std::cerr << "Error: Missing closing '}' in location block." << std::endl;
// 		return false;
// 	}
// 	return true;
// }

bool check_location(std::istream& config_file, LocationConfig& location_config)
{
    std::string line;
    bool has_closing = false;

    while (std::getline(config_file, line))
    {
        line = trim(line);
        if (line == "}")
        {
            has_closing = true;
            break;
        }
        if (line.empty())
            continue;

        std::stringstream stream(line);
        std::string keyword;
        stream >> keyword;

        if (keyword == "root")
        {
            std::string root_path;
            stream >> root_path;
            location_config.root = trim(root_path);
        }
        else if (keyword == "index")
        {
            std::string indexes;
            std::getline(stream, indexes);
            location_config.index = trim(indexes);
        }
        else if (keyword == "allow_methods")
        {
            std::string method;
            while (stream >> method)
            {
                location_config.addAllowMethod(method);
            }
        }
        else
        {
            std::cerr << "Error: Unknown directive in location block: " << line << std::endl;
            return false;
        }
    }

    if (!has_closing)
    {
        std::cerr << "Error: Missing closing '}' in location block." << std::endl;
        return false;
    }
    return true;
}
