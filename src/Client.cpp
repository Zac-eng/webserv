#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"

Client::Client() {};

void ReadRequestError(void)
{
	// Error::ReadingBuffer();
	// close(_fd);
}

bool Client::CheckCarrigereturn(std::string& buffer)
{
	int pos;

	pos = buffer.find("\r\n");
	if (pos == std::string::npos)
		return (false);
	return (true);
}

void Client::RestOfBuffer(std::string& buffer, std::string::iterator& it)
{
	size_t pos;

	pos = std::distance(buffer.begin(), it);
	this->_buffer += buffer.substr(pos);
	return ;
}

void Client::LoopRequest(std::string& buffer)
{
	std::string object;
	std::string::iterator it = buffer.begin();

	//キャリッジリターンがなく、まだ読み取る必要がない場合。
	while (CheckCarrigereturn(buffer) == true)
	{
		object = SubstringObject(buffer, it);
		this->_buffer += object;
		this->_request.ParseRequest(this->_buffer);
		//RequestのParse
		this->_buffer.clear();
	}
	if (it != buffer.end())
		RestOfBuffer(buffer, it);
}

bool Client::AcceptRequest()
{
	int byte_size = 0;
	char buf[BUFFER_SIZE];
	std::string buffer;
	
	while  (1)
	{
		byte_size = read(_fd, buf, BUFFER_SIZE - 1);
		if (byte_size < 0)
			return (ReadRequestError(), false);
		else if (byte_size == 0)
		{
			if (_request._parse_flag == true)
				this->_response.ExecuteResponse(_request);
			//構文に問題がなく、reaponseの準備が整った場合
			break;
		}
		else
		{
			buf[byte_size] = '\0';
			buffer = buf;
			LoopRequest(buffer);
		}
	}
	return (true);
}