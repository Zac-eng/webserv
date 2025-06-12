#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>
#include "RequestException.hpp"
#include <cstdlib>
#include <cctype>

class ParseUtils
{
	public:
		static bool parse_object(std::string& input, const std::string& object);
		static bool check_valid_version(std::string& input, const std::string& object);
};
