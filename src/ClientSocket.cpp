#include "ClientSocket.hpp"

ClientSocket::ClientSocket() : _first_post_flag(false),  _complete_post_flag(false),  _complete_parse_flag(false), _post_body_flag(false) {};

ClientSocket::ClientSocket(ServerConfig conf) : _response_flag(false) {};

ClientSocket::~ClientSocket() {};

bool ClientSocket::CreateSocket(void)
{
	return (false);
}

bool ClientSocket::ValidRequest(std::string& buffer, std::string::iterator& it, std::string& object)
{
	// if (object.compare("\r\n") == 0)
	// {
	// 	if (this->_request.GetRequestFlag() == false)
	// 		return (true);
	// 	if (CheckCRequestFlag(buffer, it, object) == false)
	// 		return (false);
	// }
	// else
	// {
	// 	if (this->_first_post_flag == true)
	// 		this->_complete_post_flag = true;
		if (this->_request.ParseRequest(object, this->_complete_post_flag) == false)
			return (false);
	// 	if (this->_complete_post_flag == true)
	// 		this->_post_body_flag = true;
	// }
	return (true);
}

bool ClientSocket::CheckExecuteResponse(int epoll_fd)
{
	std::string buffer = this->_buffer;
	std::string object;
	std::string::iterator it;
	struct epoll_event event;

	it = buffer.begin();
	while (it != buffer.end())
	{
		if (SubstringObject(buffer, it, object) == false)
		{
			this->_buffer = std::string(it, buffer.end());
			return (true);
		}
		else
		{
			if (this->ValidRequest(buffer, it, object) == false)
				return (false);

		}
		if (this->_complete_parse_flag == true)
		{
			if (it != buffer.end())
				return (false);
			// ChangeConfUri(this->_request.GetUri());
			this->_response_flag = true;
			// event.events = EPOLLOUT;
			// event.data.fd = this->_fd;
			// if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->_fd, &event) == -1)
			// 	return (false);
			if (this->file == 'php')
				ExecuteCgi(epoll_fd, this->_request, server_conf);
			return (true);
		}
	}
	if (it == buffer.end())
		this->_buffer.clear();
	return (true);
}



bool ClientSocket::HandleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket)
{
	int byte_size = 0;
	char buf[BUFFER_SIZE];
	size_t pos = 0;
	
	byte_size = read(this->_fd, buf, BUFFER_SIZE);
	if (byte_size < 0)
	{
		close(this->_fd);
		throw std::runtime_error("close fd");
	}
	this->_buffer.append(buf, byte_size);
	pos = this->_buffer.find("\r\n");
	if (pos == std::string::npos)
		return (true);
	else
	{
		if (this->CheckExecuteResponse(epoll_fd) == false)
			return (false);
	}
	return (true);
}

void ClientSocket::HandleEpollOutEvent(void)
{
	if (this->_response_flag == false)
		return ;
	this->_response.ExecuteResponse(this->_request);
}
