#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ErrorPage {
	public:
		class Page {
			public:
				std::string path;
				std::string media_type;
				Page();
				Page(const std::string& path, const std::string& media_type);
		};
		ErrorPage();
		const Page& get(int statusCode) const;
		std::string createErrorResponse(int statuscode) const;
	private:
		std::map<int, Page> pages;
};

std::string getStatusMessage(int statuscode);
std::string loadFile(const std::string &path);

#endif