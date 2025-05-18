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

class MultipartParser
{
	private:
		std::string _boundary;
		std::string _body;
		bool _start_flag;
		bool _end_flag;


	public:
		MultipartParser();
		void executeParse(const std::string& request);

};