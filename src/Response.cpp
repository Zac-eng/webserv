#include "Request.hpp"
#include "Response.hpp"
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

void Response::SetDirectory(const std::string& dir)
{
	this->_directory = dir;
}

void Response::SetFilename(const std::string& file)
{
	this->_filename = file;
}

void Response::SetPath(const std::string& file)
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
			return (true);
		entry = readdir(dir);
	}
	return (false);
}


// bool Response::IsDynamicFileType(const std::string& file)
// {
// 	std::string object;
// 	for (std::string::iterator it = uri.begin(); it != uri.end(); it++)
// 	{
// 		if (*it == '.')
// 			break ;
// 	}
// 	if (it == uri.end() || *it != '.')
// 		return (false);
// 	it++;
// 	object = uri.substr(it - uri.begin(), uri.end() - (it - uri.begin()));
// 	if (!object.empty() && object = "py")
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
	while (1)
	{
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
	if (this->_request.GetFile() == "html")
		this->_header.push_back("Content-Type: text/html; charset=UTF-8\r\n");
}

void Response::CheckConnectionHeader(std::map<std::string, std::string> header)
{
	std::map<std::string, std::string>::iterator it;

	for (; it != header.end(); it++)
	{
		if (it->first == "Connection")
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

void  Response::CreateResponseHeader(Request& req)
{
	std::map<std::string, std::string> header;
	std::string file;

	file = req.GetFile();
	// header = req.Getheader();
	CheckFileType(file);
	this->_header.push_back("Content-Length: " + this->_content_length + "\r\n");
	CheckConnectionHeader(header);
	CreateResponse();
}

void Response::HandleGet(Request& req)
{
	if (ReadFile(req) ==  false)
		return ;
	CreateResponseHeader(req);
	// return (StatusMessage::OK())
}

void Response::HandleMethod(Request& req)
{
	if (req.GetMethod() == "GET")
		HandleGet(req);
	// else if (req.GetMethod() == "POST")
	// 	HandlePost(req);
	// else if (req.GetMethod() == "DELETE")
	// 	HandleDelete(req);
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


void Response::ExecuteResponse(Request& req)
{
	//404 Not Foundを返す
	//Uriがあるかの確認
	if (this->ExistUri(req.GetUri()) == false)
		return ;
		// return (Error::InvalidUri());
	this->ExecuteAndGetStatusCode(req);
	// Createresponse(req);
}