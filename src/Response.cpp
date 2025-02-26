#include "Response.hpp"
#include "Client.hpp"

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

// bool Response::ReadUri(Request& req)
// {
// 	if (ReadFile() == false)
// 		return (false);
// 	CreateResponse(req, StatusCode::OK);
// 	return (true);
// }

// void Response::CreateResponseOK(Response& req)
// {
// 	CreateResponseHeader(req);
// }

// void Response::HandlePost(Request& req)
// {
// 	if (CreateFile(req) == false)
// 		return ;
// 	return ;
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
			this->_response.append(buf, byte_size);
	}
	return (true);
}

void Response::ExecuteHeaderResponse(Request& req)
{
	this->_response_header = 
	    "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: " + this->_content_length + "\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";
	this->_response_header += this->_response;
	write(this->_fd, this->_response_header.c_str(), this->_response_header.length());

}

void Response::HandleGet(Request& req)
{
	if (ReadFile(req) ==  false)
		return ;
	ExecuteHeaderResponse(req);
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
	// else
		return ;
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

void ExecuteCGI(Request& req)
{
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

// void Response::CreateResponse(Request& req)
// {
// 	if (number == OK)
// 		CreateResponseOK(req);
// 	else if (number == BadRequest)
// 		CreateResponseBadRequest(req);
// 	else if (number == NotFound)
// 		CreateResponseNotFound(req);
// 	else
// 		return ;
// }

void Response::ExecuteResponse(Request& req)
{
	//404 Not Foundを返す
	if (this->ExistUri(req.GetUri()) == false)
		return ;
		// return (Error::InvalidUri());
	this->ExecuteAndGetStatusCode(req);
	// Createresponse(req);
}
