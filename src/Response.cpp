#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "ClientSocket.hpp"


Response::Response()
{
	std::cout << "Response object create" << std::endl;
}

Response::Response(Request& request) : _request(request) {};


Response::~Response()
{
	std::cout << "Response object destroyed" << std::endl;
}

void Response::setFd(int fd)
{
	this->_fd = fd;
}

void Response::setDirectory(const std::string& dir)
{
	this->_directory = dir;
}

void Response::setFilename(const std::string& file)
{
	this->_filename = file;
}

void Response::setPath(const std::string& file)
{
	this->_path = file;
}

bool Response::ExistUri(const std::string& uri)
{
	DIR *dir;
	dirent *entry;

	dir = opendir(this->_directory.c_str());
	if (dir == NULL)
		return (false);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (strcmp(entry->d_name, this->_filename.c_str()) == 0)
		{
			closedir(dir);
			return (true);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (false);
}


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

void Response::GetFileSize()
{
	struct stat file;
	std::stringstream ss;
	
	if (stat(this->_path.c_str(), &file) == -1)
	{
		throw std::runtime_error("stat");
	}
	ss << file.st_size;
	this->_content_length = ss.str();

}

// おそらくこれもepollに含めなきゃいけない
bool Response::ReadFile(Request& req)
{
	int fd;
	int byte_size;
	char buf[BUFFER_SIZE];
	
	fd = open(this->_path.c_str(), O_RDONLY);
	if (fd  == -1)
		return (false);
	if (set_nonblocking(fd) == false)
		return (false);
	while (1)
	{
				std::cout << "---" <<std::endl;

		byte_size = read(fd, buf, BUFFER_SIZE);
		if (byte_size < 0)
		{
			close(fd);
			throw std::runtime_error("close fd");
		}
		else if (byte_size == 0)
		{
			this->GetFileSize();
			close(fd);
			break;
		}
		else
			this->_response_body.append(buf, byte_size);
	}
	return (true);
}

void Response::CheckFileType(std::string& file)
{
	if (this->_request.getFile() == "html")
		this->_header.push_back("Content-Type: text/html; charset=UTF-8\r\n");
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

void Response::CreateResponse()
{
	std::vector<std::string>::iterator it;

	it = this->_header.begin();
	this->_response = "HTTP/1.1 200 OK\r\n";
	for (; it != this->_header.end(); it++)
		this->_response += *it;
	this->_response += "\r\n";
	this->_response += this->_response_body;
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
	CheckFileType(file);
	this->_header.push_back("Server: webserv/1.0\r\n");
	this->_header.push_back("Content-Length: " + this->_content_length + "\r\n");
	createDateHeader();
	// CheckConnectionHeader(header);
	if (req._connection_flag == true)
		this->_header.push_back("Connection: close\r\n");
	CreateResponse();
}

void Response::handleGet(Request& req)
{

	if (ReadFile(req) == false)
		return ;
	CreateResponseHeader(req);
	// return (StatusMessage::OK())
}


void Response::handleDelete(Request& req)
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
	file << req._body;
	file.close();
	this->_status_code = 200;
	return ;
}

void Response::HandleMethod(Request& req)
{
	if (!this->_cgi_buffer.empty())
	{
		// ReaponseHeader(req);
		write(this->_fd, this->_cgi_buffer.c_str(), this->_cgi_buffer.length());
		return ;
	}
	if (req.GetMethod() == "GET")
		handleGet(req);
	else if (req.GetMethod() == "POST")
		handlePost(req);
	else if (req.GetMethod() == "DELETE")
		handleDelete(req);
	return ;
}

void Response::ExecuteAndGetStatusCode(Request& req)
{
	bool type = false;

	// type = IsDynamicFileType(this->_filename);
	// if (type == true)
	// 	ExecuteCGI(req);
	// else
		HandleMethod(req);
	return ;
}

void Response::ResponseError(Request& req)
{
        std::ostringstream response;
        
        // HTTP ヘッダーとエラーメッセージのフォーマット
        response << "HTTP/1.1 " << req._status_number << " Bad Request\r\n";
        response << "Content-Type: text/html\r\n";
        response << "Connection: close\r\n";
        response << "\r\n";
        
        // レスポンスボディ
        response << "<html><body>";
        response << "<h1>" << req._status_number << " Bad Request</h1>";
        response << "<p>Your request could not be understood by the server.</p>";
        response << "</body></html>";

        // レスポンスボディとヘッダーをソケットに書き込む
        std::string response_str = response.str();
        write(this->_fd, response_str.c_str(), response_str.length());
		throw ResponseException(400);
}


void Response::ExecuteResponse(Request& req)
{
	//404 Not Foundを返す
	//Uriがあるかの確認
	if (req._status_number != 0)
		return (ResponseError(req));
	if (this->ExistUri(req.getPath()) == false)
		return (ResponseError(req));
	this->ExecuteAndGetStatusCode(req);
		// Createresponse(req);
}