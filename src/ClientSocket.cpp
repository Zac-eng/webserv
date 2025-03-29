#include "ClientSocket.hpp"


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

bool ClientSocket::CheckExecuteResponse(void)
{
	std::string buffer = this->_buffer;
	std::string object;
	std::string::iterator it;

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
			ChangeConfUri(this->_request.GetUri());
			this->_response._fd = this->_fd;
			this->_response.ExecuteResponse(this->_request);
			return (true);
		}
	}
	if (it == buffer.end())
		this->_buffer.clear();
	return (true);
}


void ClientSocket::HandleEpollInEvent(int epoll_fd)
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
		if (this->CheckExecuteResponse() == false)
			return (this->CloseClientFd());
	}
	return (true);
}