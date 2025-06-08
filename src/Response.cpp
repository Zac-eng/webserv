#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "ClientSocket.hpp"

Response::Response() : _error_file_flag(false)
{
	std::cout << "Response object create" << std::endl;
}

Response::~Response()
{
	std::cout << "Response object destroyed" << std::endl;
}

int Response::getFd(void) const
{
	return (this->_fd);
}

void Response::setFd(const int& fd)
{
	this->_fd = fd;
	return ;
}

std::string Response::getResponse(void) const
{
	return (this->_response);
}

void Response::setResponse(const std::string& response)
{
	this->_response = response;
	return ;
}

size_t Response::getStatusCode(void) const
{
	return (this->_status_code);
}

void Response::setStatusCode(const size_t& status_code)
{
	this->_status_code = status_code;
	return ;
}

std::string Response::getDirectory(void) const
{
	return (this->_directory);
}

void Response::setDirectory(const std::string& directory)
{
	this->_directory = directory;
	return ;
}

std::string Response::getFilename(void) const
{
	return (this->_filename);
}

void Response::setFilename(const std::string& filename)
{
	this->_filename = filename;
	return ;
}

std::string Response::getPath(void) const
{
	return (this->_path);
}

void Response::setPath(const std::string& path)
{
	this->_path = path;
	return ;
}

std::string Response::getContentLength(void) const
{
	return (this->_content_length);
}

void Response::setContentLength(const std::string& content_length)
{
	this->_content_length = content_length;
	return ;
}



std::vector<std::string> Response::getHeader(void) const
{
	return (this->_header);
}

void Response::setHeader(const std::vector<std::string>& header)
{
	this->_header = header;
	return ;
}

std::string Response::getBody(void) const
{
	return (this->_body);
}

void Response::setBody(const std::string& body)
{
	this->_body = body;
	return ;
}

std::string Response::getCgiBuffer(void) const
{
	return (this->_cgi_buffer);
}

void Response::setCgiBuffer(const std::string& cgi_buffer)
{
	this->_cgi_buffer = cgi_buffer;
	return ;
}

// bool Response::ExistUri(const std::string& uri)
// {
// 	DIR *dir;
// 	dirent *entry;

// 	dir = opendir(this->_directory.c_str());
// 	if (dir == NULL)
// 		return (false);
// 	entry = readdir(dir);
// 	while (entry != NULL)
// 	{
// 		if (strcmp(entry->d_name, this->_filename.c_str()) == 0)
// 		{
// 			closedir(dir);
// 			return (true);
// 		}
// 		entry = readdir(dir);
// 	}
// 	closedir(dir);
// 	return (false);
// }


// bool Response::IsDynamicFileType(const std::string& file)
// {
// 	std::string object;
// 	std::string::const_iterator it;

// 	it = file.begin();
// 	for (;it != file.end(); it++)
// 	{
// 		if (*it == '.')
// 			break ;
// 	}
// 	if (it == file.end() || *it != '.')
// 		return (false);
// 	it++;
// 	object = file.substr(it - file.begin(), file.end() - (it - file.begin()));
// 	if (!object.empty() && object = "")
// 		return (true);
// 	return (false);
// }

// // おそらくこれもepollに含めなきゃいけない
// bool Response::ReadFile(Request& req)
// {
// 	int fd;
// 	int byte_size;
// 	char buf[BUFFER_SIZE];
	
// 	fd = open(this->_path.c_str(), O_RDONLY);
// 	if (fd  == -1)
// 		return (false);
// 	if (set_nonblocking(fd) == false)
// 		return (false);
// 	while (1)
// 	{
// 		byte_size = read(fd, buf, BUFFER_SIZE);
// 		if (byte_size < 0)
// 		{
// 			close(fd);
// 			throw std::runtime_error("close fd");
// 		}
// 		else if (byte_size == 0)
// 		{
// 			this->GetFileSize();
// 			close(fd);
// 			break;
// 		}
// 		else
// 			this->_response_body.append(buf, byte_size);
// 	}
// 	return (true);
// }

// void Response::CheckFileType(std::string& file)
// {
// 	if (this->_request.getFile() == "html")
// 		this->_header.push_back("Content-Type: text/html; charset=UTF-8\r\n");
// }

void Response::ErrorResponse(size_t code, const std::string& title)
{
	std::ostringstream response;

	this->_response.clear();
	response << "HTTP/1.1 "<<code<<" " <<title<<"\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Connection: close\r\n";
	response << "\r\n";

	if (this->_error_file_flag == true && !this->_body.empty())
	{
		response << this->_body;
	}
	else
	{
		response << "<html><body>";
		response << "<h1>" << code << " " << title << "</h1>";
		response << "</body></html>";
	}
	this->_response = response.str();
	std::cout << "aaa"<<this->_fd<<std::endl;
	write(this->_fd, this->_response.c_str(), this->_response.length());
}

void Response::executeRedirectResponse(size_t code, const std::string& title)
{
	std::ostringstream response;

	response << "HTTP/1.1 "<<code<<" " <<title<<"\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Location: " << this->_redirect_uri << "\r\n";
	response << "Connection: close\r\n";
	response << "\r\n";

		response << "<html><body>";
		response << "<h1>" << code << " " << title << "</h1>";
		response << "</body></html>";
	this->_response = response.str();
	std::cout << this->_fd<<"111"<<std::endl;
	write(this->_fd, this->_response.c_str(), this->_response.length());
}

void Response::responseRedirect(void)
{
	executeRedirectResponse(301, "Redirect");
}

void Response::ResponseBadRequest(void)
{
	ErrorResponse(400, "Bad Request");
}

void Response::ResponseFileNotFound(void)
{
	ErrorResponse(404, "Not Found");
}

void Response::ResponseMethodNotAloowed(void)
{
	ErrorResponse(405, "Method Not Allowed");
}

void Response::ResponseInternalServerError(void)
{
	ErrorResponse(500, "Internal Server Error");
}

void Response::ResponseNotImplemented(void)
{
	ErrorResponse(501, "Not Implemented");
}

void Response::ResponseBadGateway(void)
{
	ErrorResponse(501, "Bad Gateway");
}

void Response::ResponseVersionNotSupported(void)
{
	ErrorResponse(505, "HTTP Version Not Supported");
}

void Response::ResponseRequestTimeOut(void)
{
	ErrorResponse(408, "HTTP Request TimeOut");
}

void Response::responseLargeRequestBody(void)
{
	ErrorResponse(413, "Request Entity Too Large");
}


void Response::reSetResponse(void)
{
	this->_response.clear();
	this->_status_code = 0;
	this->_directory.clear();
	this->_filename.clear();
	this->_path.clear();
	this->_content_length.clear();
	this->_header.clear();
	this->_body.clear();
	this->_cgi_buffer.clear();
	this->_error_file_flag = false;
}

void Response::closeResponse(bool flag)
{
	// if (server.errorpage() == this->_r.status_number)
	// 	serchErrorPage();
	// std::cout << this->_body<<std::endl;
	// if (flag == true && !this->_body.empty())
	// 	write(this->_fd, this->_body.c_str(), this->_body.length());
	if (flag == true)
		this->_error_file_flag = true;
	if (this->_status_code == 301)
		responseRedirect();
	else if (this->_status_code == 400)
		ResponseBadRequest();
	else if(this->_status_code == 404)
		ResponseFileNotFound();
	else if(this->_status_code == 405)
		ResponseMethodNotAloowed();
	else if(this->_status_code == 408)
		ResponseRequestTimeOut();
	else if (this->_status_code == 413)
		responseLargeRequestBody();
	else if(this->_status_code == 500)
		ResponseInternalServerError();
	else if(this->_status_code == 501)
		ResponseNotImplemented();
	else if(this->_status_code == 502)
		ResponseBadGateway();
	else if(this->_status_code == 505)
		ResponseVersionNotSupported();
	return ;
}

void Response::CheckConnectionHeader(std::map<std::string, std::string> header)
{
	std::map<std::string, std::string>::iterator it;

	it = header.begin();
	for (; it != header.end(); it++)
	{
		if (it->first == "connection")
		{
			if (it->second == "keep-alive")
				this->_header.push_back("Connection: keep-alive\r\n");
		}
	}
}

void Response::setRedirectUri(const std::string& uri)
{
	this->_redirect_uri = uri;
	return ;
}

std::string Response::getRedirectUri(void)
{
	return (this->_redirect_uri);
}

void Response::CreateResponse()
{
std::vector<std::string>::iterator it;

std::cout << "response" << std::endl;
it = this->_header.begin();
this->_response = "HTTP/1.1 200 OK\r\n";
for (; it != this->_header.end(); it++)
this->_response += *it;
if (!this->_cgi_buffer.empty())
{
this->_response += this->_cgi_buffer;
}
else
{
this->_response += "\r\n";
this->_response += this->_body;
}
write(this->_fd, this->_response.c_str(), this->_response.length());
}

void Response::createDateHeader(void)
{
time_t now;
struct tm n_time;
char buf[80];
std::string date;

now = time(0);
n_time = *gmtime(&now);
strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &n_time);
date = "Date: ";
date += buf;
date += "\r\n";
this->_header.push_back(date);
}

void  Response::CreateResponseHeader(Request& req)
{
std::map<std::string, std::string> header;
std::string file;

file = this->_filename;
// CheckFileType(file);
this->_header.push_back("Server: webserv/1.0\r\n");
if (this->_cgi_buffer.empty())
this->_header.push_back("Content-Length: " + this->_content_length + "\r\n");
createDateHeader();
// CheckConnectionHeader(header);
if (req.getConnectionFlag() == true)
this->_header.push_back("Connection: close\r\n");
CreateResponse();
}

void Response::handleGet(Request& req)
{
	// if (ReadFile(req) == false)
	// 	throw ResponseException(404);
	CreateResponseHeader(req);
	// return (StatusMessage::OK())
}


void Response::handleDelete(void)
{
	if (remove(this->_path.c_str()) != 0)
		throw ResponseException(400);
	this->_status_code = 200;
	return ;
}

void Response::handlePost(Request& req)
{
	std::ofstream file(this->_path.c_str());

	if (!file.is_open())
		throw std::runtime_error("error");
	file << req.getBody();
	file.close();
	this->_status_code = 200;
	return ;
}

void Response::HandleMethod(Request& req)
{
	std::cout << this->_cgi_buffer << std::endl;
	// if (!this->_cgi_buffer.empty())
	// {
	// 	// ReaponseHeader(req);
	// 	write(this->_fd, this->_cgi_buffer.c_str(), this->_cgi_buffer.length());
	// 	return ;
	// }
	if (req.getMethod() == "GET")
		handleGet(req);
	else if (req.getMethod() == "POST")
		handlePost(req);
	else if (req.getMethod() == "DELETE")
		handleDelete();
	return ;
}

void Response::ExecuteAndGetStatusCode(Request& req)
{
	// bool type = false;

	// type = IsDynamicFileType(this->_filename);
	// if (type == true)
	// 	ExecuteCGI(req);
	// else
		HandleMethod(req);
	return ;
}


void Response::ExecuteResponse(Request& req)
{
	this->ExecuteAndGetStatusCode(req);
		// Createresponse(req);
}