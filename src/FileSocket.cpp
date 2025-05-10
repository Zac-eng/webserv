#include "FileSocket.hpp"
#include "ClientSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "RequestException.hpp"

FileSocket::FileSocket(int other_fd, Request& request, Response& response, const std::string directory, const std::string file) : \
	 _other_fd(other_fd), _request(request), _response(response), _directory(directory), _file(file) {}


void FileSocket::getFileSize(void)
{
	struct stat file;
	std::stringstream ss;
	std::string path;
	
	path = this->_directory;
	path += this->_file;
	if (stat(path.c_str(), &file) == -1)
	{
		throw RequestException(500, "file size Error");
	}
	ss << file.st_size;
	this->_response.setContentLength(ss.str());
}


void FileSocket::readFile()
{
	int byte_size;
	char buf[BUFFER_SIZE];
	std::string buffer;

	while (1)
	{
		byte_size = read(this->_fd, buf, BUFFER_SIZE);
		if (byte_size < 0)
		{
			throw RequestException(500, "file size Error");
		}
		else if (byte_size == 0)
		{
			this->getFileSize();
			break;
		}
		else
			buffer.append(buf, byte_size);
	}
	this->_response.setBody(buffer);
	return ;
}

bool FileSocket::clposeAndDeleteSocket(std::map<int, ASocket*>& socket)
{
	std::map<int, ASocket*>::iterator it;

	it = socket.find(this->_fd);
	if (it == socket.end())
		return (false);
	close(it->first);
	delete (it->second);
	socket.erase(it);
	return (true);
}

bool FileSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	struct epoll_event ev;


	ev.events = EPOLLOUT;
	ev.data.fd = this->_other_fd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->_fd, NULL) == -1) {
			throw RequestException(500, "file size Error");
		}
	try
	{
		this->readFile();
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD,this->_other_fd, &ev) == -1) {

			this->_request.setStatusNumber(500);
		}
	}
	catch (const RequestException& e)
	{
		std::cout << e.what() << std::endl;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_other_fd, &ev) == -1) {
			this->_request.setStatusNumber(500);
			}
		if (this->_request.getStatusNumber() != 0)
			this->_request.setStatusNumber(e.getStatus());
	}
	catch (std::exception& e)
	{
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD,this->_other_fd, &ev) == -1) {
			std::cout << "error" << std::endl;
			}
		this->_request.setStatusNumber(500);
	}
	this->_response.setFd(this->_other_fd);
	if (clposeAndDeleteSocket(_socket) == false)
		return (false);
	return (true);
}

bool FileSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	throw std::runtime_error("File Socket Epoll Out");
}

bool FileSocket::createSocket()
{
	throw std::runtime_error("File Socket Epoll Out");
}
