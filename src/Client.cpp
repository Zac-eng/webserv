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

bool Client::LoopRequest(std::string& buffer)
{
	std::string object;
	std::string::iterator it = buffer.begin();

	//キャリッジリターンがなく、まだ読み取る必要がない場合。
	while (CheckCarrigereturn(buffer) == true)
	{
		// std::cout<<"Header   "<<buffer<<std::endl;
		if (SubstringObject(buffer, it, object) == false)
			return (false);
		it = buffer.begin();
		this->_buffer += object;
		if (this->_request.ParseRequest(this->_buffer) == false)
			return (false);
		//RequestのParse
		this->_buffer.clear();
	}
	if (!buffer.empty())
		RestOfBuffer(buffer, it);
	return (true);
}

bool Client::AcceptRequest()
{
	int byte_size = 0;
	char buf[BUFFER_SIZE];
	std::string buffer;
	
	while (true)
	{
			// std::cout << "+++++++++++" << std::endl;
		byte_size = read(_fd, buf, BUFFER_SIZE - 1);
			// std::cout << "---------ssss-0--------" << std::endl;
		if (byte_size < 0)
		{
			// std::cout << "+++++++++++" << std::endl;
			return (ReadRequestError(), false);
		}
		else if (byte_size == 0)
		{
			// std::cout << "---------ssss-0--------" << std::endl;
			if (_request._parse_flag == true)
				this->_response.ExecuteResponse(_request);
			//構文に問題がなく、reaponseの準備が整った場合
			return (true);
		}
		else
		{
			buf[byte_size] = '\0';
			buffer = buf;
			// std::cout << "----------" <<byte_size<< buffer<< std::endl;
			if (LoopRequest(buffer) == false)
			{
				return (false);
			}
			if (_request._parse_flag == true)
				this->_response.ExecuteResponse(_request);
			if (buffer.empty())
				return (true);
			// std::cout << "---------sqaaaaa-------" << std::endl;
		}
	}
	return (true);
}