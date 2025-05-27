#include "ErrorPage.hpp"

ErrorPage::Page::Page() : path(), media_type("application/octet-stream") {}

ErrorPage::Page::Page(const std::string& path, const std::string& media_type) : path(path), media_type(media_type) {}

ErrorPage::ErrorPage() {
	pages[400] = Page("error_pages/400.html", "text/html");
	pages[404] = Page("error_pages/404.html", "text/html");
	pages[405] = Page("error_pages/405.html", "text/html");
	pages[413] = Page("error_pages/413.html", "text/html");
	pages[500] = Page("error_pages/500.html", "text/html");
	pages[504] = Page("error_pages/504.html", "text/html");
	pages[505] = Page("error_pages/505.html", "text/html");
}

const ErrorPage::Page& ErrorPage::get(int statuscode) const {
	static Page defaultpage("errors/default.html", "text/html");
	std::map<int, Page>::const_iterator it = pages.find(statuscode);
	if (it != pages.end()) {
		return it->second;
	}
	return defaultpage;
}

std::string getStatusMessage(int statuscode) {
	switch (statuscode) {
		case 400: return "Bad Request";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 413: return "Payload Too Large";
		case 500: return "Internal Server Error";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		default: return "Unknown Error";
	}
}

std::string generateDefaultErrorBody(int statuscode) {
	std::string message = getStatusMessage(statuscode);
	std::ostringstream html;
	html << "<!DOCTYPE html>\n"
		 << "<html><head><meta charset=\"UTF-8\"><title>"
		 << statuscode << " " << message
		 << "</title></head><body><h1>"
		 << statuscode << " " << message
		 << "</h1><p>The server encountered an error.</p></body></html>";
	return html.str();
}

std::string loadFile(const std::string &path, bool &success) {
	std::ifstream file(path.c_str());
	if (!file.is_open()){
		success = false;
		return "";
	}
	std::ostringstream buf;
	buf << file.rdbuf();
	success = true;
	return buf.str();
}

std::string ErrorPage::createErrorResponse(int statuscode) const {
	const Page &page = get(statuscode);
	bool file_loaded = false;
	std::string body = loadFile(page.path, file_loaded);
	if (!file_loaded) {
		body = generateDefaultErrorBody(statuscode);
	}
	std::string status_msg = getStatusMessage(statuscode);
	std::ostringstream res;
	res << "HTTP/1.1 " << statuscode << " " << status_msg << "\r\n";
	res << "Content-Type: text/html; charset=UTF-8\r\n";
	res << "Content-Length: " << body.size() << "\r\n";
	res << "Connection: close\r\n";
	res << "\r\n";
	res << body;
	return res.str();
}
