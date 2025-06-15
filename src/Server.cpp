#include "Server.hpp"
#include "Signal.hpp"
#include "CgiSocket.hpp"

Server::Server(std::vector<ServerConfig>& conf) : _conf(conf)
{
	std::cout << "server created" << std::endl;
};

Server::Server() {};

Server::~Server()
{
	std::cout << "server destructor" << std::endl;
	std::map<int, ASocket*>::iterator it;

	it = this->_socket.begin();
	for (; it != this->_socket.end(); it++)
	{
		close(it->first);
		delete it->second;
	}
	close(this->_epoll_fd);
}

std::vector<ServerConfig> Server::getConf(void)
{
	return (this->_conf);
}

void Server::setConf(std::vector<ServerConfig>& conf)
{
	this->_conf = conf;
	return ;
}

bool set_nonblocking(int fd)
{
	int flag;

	flag = fcntl(fd, F_GETFL);
	if (flag == -1)
		throw std::runtime_error("NonBlocking Fd");
	// 現在の性能にNON_BLOCKを追加。3BIT目がNON_BLOCKのフラグ。
	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
		throw std::runtime_error("NonBlocking Fd");
	return (true);
}

bool Server::setMonitoringFd(ASocket* socket)
{
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = socket->getFd();
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, socket->getFd(), &event) < 0)
		return (false);
	return (true);
}

bool Server::epollCreate(void)
{
	this->_epoll_fd = epoll_create1(0);

	if (this->_epoll_fd < 0)
		return (false);
	for (std::map<int, ASocket*>::iterator it = this->_socket.begin(); it != this->_socket.end(); it++)
	{
		if (setMonitoringFd(it->second) ==  false)
			return (false);
	}
	return (true);
}

// todo、falseが返ってきた時にsocketを全てfreeして終了。
void Server::createListenServer(void)
{
	std::vector<ServerConfig>::iterator it;

	it = this->_conf.begin();
	for (; this->_conf.end() != it; it++)
	{
		ASocket *socket = new ListenSocket(*it);
		if (socket->createSocket() == false)
		{
			delete socket;
			throw ServerException();
		}
		if (set_nonblocking(socket->getFd()) == false)
		{
			delete socket;
			throw ServerException();
		}
		socket->setStartTime(-1);
		this->_socket.insert(std::make_pair(socket->getFd(), socket));
	}
	if (epollCreate() == false)
		throw ServerException();
	return ;
}

void Server::checkTimeOut(void)
{
	std::map<int, ASocket*>::iterator it;
	time_t start;
	time_t end;


	it = this->_socket.begin();
	while (it != this->_socket.end())
	{
		end = time(NULL);
		start = it->second->getStartTime();
		// std::cout << end - start << std::endl;
		if (start != -1 && (end - start) > TIMEOUT)
		{
			if (it->second->handleTimeOut(this->_epoll_fd, this->_socket, it->first) == false)
				return ;
		}
		it++;
	}
}

void Server::executeServer(void)
{
	int event_counts;
	struct epoll_event event[MAX_EVENTS];
	int fd;

	while (g_stop)
	{
		event_counts = epoll_wait(this->_epoll_fd, event, MAX_EVENTS, EPOLL_TIME);
		// this->checkTimeOut();
		if (event_counts == -1)
		{
			if (g_stop == 1)
				throw ServerException();
			else
				return ;
		}
		for (int i = 0; i < event_counts; i++)
		{
			fd = event[i].data.fd;
			if (event[i].events == EPOLLIN)
			{
				std::cout << "thsi->fd:" << event[i].data.fd<<std::endl;
				if (this->_socket.find(fd) != this->_socket.end())
				{
					this->_socket[event[i].data.fd]->handleEpollInEvent(this->_epoll_fd, this->_socket);
				}
			}
			else if (event[i].events == EPOLLOUT)
			{
				if (this->_socket.find(fd) != this->_socket.end())
				{

					this->_socket[event[i].data.fd]->handleEpollOutEvent(this->_epoll_fd, this->_socket);
				}
			}
			else if (event[i].events == EPOLLHUP)
			{
				std::cout << "epoll hup" << std::endl;
				CgiSocket* sock = dynamic_cast<CgiSocket*>(this->_socket[event[i].data.fd]);
				if (sock != NULL) {
					sock->waitChildProcess();
				}
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, event[i].data.fd, NULL) != 0) {
					perror("epoll delete, cgi");
				}
				delete this->_socket[event[i].data.fd];
				this->_socket.erase(event[i].data.fd);
			}
		}
	}
}
