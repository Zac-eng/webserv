#include "CgiSocket.hpp"
#include "ClientSocket.hpp"

CgiSocket::CgiSocket(
  pid_t cgi_pid,
  int read_fd,
  int write_fd,
  Request& req,
  std::string& response,
  int client_fd
): _req(req), _response_body(response), _client_fd(client_fd) {
  this->_cgi_pid = cgi_pid;
  this->_pipe_fds[READ] = read_fd;
  this->_pipe_fds[WRITE] = write_fd;
  this->_response_body = "";
  time(&this->_created_at);
}

CgiSocket::~CgiSocket() {
  kill(_cgi_pid, SIGINT);
  close(this->_pipe_fds[READ]);
  close(this->_pipe_fds[WRITE]);
}

CgiSocket::CgiSocket(
  const CgiSocket& obj
) : ASocket(), _req(obj._req), _response_body(obj._response_body) {
  *this = obj;
}

CgiSocket& CgiSocket::operator = (const CgiSocket& obj) {
  if (this == &obj)
    return *this;
  this->_cgi_pid = obj._cgi_pid;
  this->_pipe_fds[READ] = obj._pipe_fds[READ];
  this->_pipe_fds[WRITE] = obj._pipe_fds[WRITE];
  time(&this->_created_at);
  return *this;
}

bool CgiSocket::createSocket(void)
{
	return (false);
}

CgiSocket* CgiSocket::createCgiSocket(
  ServerConfig& conf,
  Request& req,
  const sockaddr_in& client_addr,
  std::string& response_buf,
  int client_fd
) {
  int ptc_pipe[2];
  int ctp_pipe[2];

  if (!initPipes(ptc_pipe, ctp_pipe)) {
    req.setStatusNumber(500);
    return NULL;
  }
  pid_t pid = fork();
  if (pid < 0) {
    req.setStatusNumber(500);
    close_pipes(ptc_pipe, ctp_pipe);
    return NULL;
  }
  else if (pid == 0) {
    const char *args[] = {req.getFile().c_str(), NULL};
    if (!prepareChildPipes(ptc_pipe, ctp_pipe)) {
      close_pipes(ptc_pipe, ctp_pipe);
      std::exit(50);
    }
    const char **meta_vars = create_meta_vars(conf, req, client_addr);
    if (execve(req.getFile().c_str(), (char **)args, (char **)meta_vars) != 0) {
      switch (errno) {
        case ENOENT:
          std::exit(44);
        default:
          std::exit(52);
      }
    }
    std::exit(0);
  }
  if (!prepareParentPipes(ptc_pipe, ctp_pipe)) {
    req.setStatusNumber(500);
    close_pipes(ptc_pipe, ctp_pipe);
    kill(pid, SIGINT);
    return NULL;
  }
  return new CgiSocket(pid, ctp_pipe[READ], ptc_pipe[WRITE], req, response_buf, client_fd);
}

void CgiSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& socket) {
  std::string response_body;
  std::stringstream ss;
  char read_buf[BUFFER_SIZE];
  int read_count;

  (void)socket, (void)epoll_fd;
  read_count = read(this->_pipe_fds[READ], read_buf, BUFFER_SIZE - 1);
  if (read_count < 0) {
    return ;
  }
  read_buf[read_count] = '\0';
  response_body += read_buf;
  if (read_count == BUFFER_SIZE - 1)
    return;
  std::string crlf = "\r\n\r\n";
  std::size_t header_end = response_body.find(crlf);

  if (header_end != std::string::npos) {
    std::string body = response_body.substr(header_end + crlf.length());
    ss << "Content-Length: ";
    ss << body.length();
    ss << "\r\n";
  } else {
    ss << "Content-Length: ";
    ss << response_body.length();
    ss << "\r\n\r\n";
  }
  ss << response_body;
  std::cerr << response_body << std::endl;
  this->_response_body = ss.str();

  std::map<int, ASocket*>::iterator read_epoll = socket.find(this->_pipe_fds[READ]);
  if (read_epoll != socket.end()) {
    socket.erase(read_epoll);
  }
  if (setAddEpollEvent(epoll_fd, false, this->_client_fd) == false) {
    return ;
	}
	if (setDelEpollEvent(epoll_fd, this->_pipe_fds[READ]) == false) {
		throw RequestException(500, "Parse not finish");
  }
  close(this->_pipe_fds[READ]);
  // delete read_epoll->second;
}

void CgiSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& socket) {

  std::map<int, ASocket*>::iterator write_epoll = socket.find(this->_pipe_fds[WRITE]);
  if (write_epoll != socket.end()) {
    socket.erase(write_epoll);
  }
  if (setDelEpollEvent(epoll_fd, this->_pipe_fds[WRITE]) == false) {
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  if (setAddEpollEvent(epoll_fd, true, this->_pipe_fds[READ]) == false) {
    close(this->_pipe_fds[WRITE]);
    return ;
	}
  if (socket.insert(std::make_pair(this->_pipe_fds[READ], this)).second == false) {
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  if (this->_req.getBody().empty()) {
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  std::string cgi_input = _req.getBoundary() + "\r\n" + _req.getBody() + _req.getBoundary() + "--\r\n";
  if (write(this->_pipe_fds[WRITE], cgi_input.c_str(), cgi_input.length()) < 0) {
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  close(this->_pipe_fds[WRITE]);
  return ;
}

void	CgiSocket::handleEpollHupEvent(int epoll_fd, std::map<int, ASocket*>& _socket) {
  int target_fd = this->_pipe_fds[READ];

	waitChildProcess();
  if (setAddEpollEvent(epoll_fd, false, this->_client_fd) == false) {
    return ;
	}
	setDelEpollEvent(epoll_fd, target_fd);
	_socket.erase(target_fd);
	delete this;
}

bool CgiSocket::initPipes(int ptc[], int ctp[]) {
  if (pipe(ptc) != 0) {
    return false;
  }
  if (pipe(ctp) != 0) {
    close(ptc[READ]);
    close(ptc[WRITE]);
    return false;
  }
  return true;
}

bool CgiSocket::prepareChildPipes(int ptc[], int ctp[]) {
  if (close(ptc[WRITE]) != 0 || close(ctp[READ]) != 0 || \
    dup2(ptc[READ], 0) < 0 || dup2(ctp[WRITE], 1) < 0) {
    return false;
  }
  return true;
}

bool CgiSocket::prepareParentPipes(int ptc[], int ctp[]) {
  if (close(ptc[READ]) != 0 || close(ctp[WRITE]) != 0 ||\
      set_nonblocking(ptc[WRITE]) == false || set_nonblocking(ctp[READ]) == false)
      return false;
  return true;
}

int		CgiSocket::getReadPipe() const {
  return this->_pipe_fds[READ];
}

int		CgiSocket::getWritePipe() const {
  return this->_pipe_fds[WRITE];
}

int		CgiSocket::waitChildProcess() const {
  int status = 0;
  if (waitpid(this->_cgi_pid, &status, WNOHANG) == 0) {
    return -1;
  }
  if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
    status = WEXITSTATUS(status);
    if (status == 44 || status == 52) {
      status = status / 10 * 100 + status % 10;
    } else {
      status = 500;
    }
    this->_req.setStatusNumber(status);
  } else if (WIFSIGNALED(status)) {
    status = 500;
  }
  return status;
}

void closeAndDeleteSocket(std::map<int, ASocket*>& socket, int fd)
{
	std::map<int, ASocket*>::iterator it;

	it = socket.find(fd);
	if (it == socket.end())
		return ;
	close(it->first);
	delete (it->second);
	socket.erase(it);
	return ;
}

int CgiSocket::ctlClientEpollOut(int epoll_fd) const {
  struct epoll_event ev;

  ev.events = EPOLLOUT;
  ev.data.fd = this->_client_fd;
  return epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->_client_fd, &ev);
}

bool CgiSocket::handleTimeOut(int epoll_fd, std::map<int, ASocket*>& _socket, int fd) 
{
  	// struct epoll_event ev;
  (void)_socket;
  if (this->getTimeOut() == true)
    return (true);
  {
    kill(this->_cgi_pid, SIGINT);
    ctlClientEpollOut(epoll_fd);
    //   ev.events = EPOLLOUT;
    //   ev.data.fd = this->_client_fd;
    //   close(this->_pipe_fds[READ]);
    //  if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->_client_fd, &ev) == -1) {
    //  }
    this->_req.setStatusNumber(504);
    // if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    if (setDelEpollEvent(epoll_fd, fd) == false) {
			this->setTimeOut(true);
		}
		this->setTimeOut(true);
    closeAndDeleteSocket(_socket, fd);
    return (false);
  }
}

bool CgiSocket::setAddEpollEvent(int epoll_fd, bool event_flag, int fd)
{
	struct epoll_event ev;

	ev.data.fd = fd;
	if (event_flag == true)
		ev.events = EPOLLIN;
	else
		ev.events = EPOLLOUT;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		if (errno == EEXIST)
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1)
				return (false);
			return (true);
		}
		return (false);
	}
	return (true);
}

bool CgiSocket::setModEpollEvent(int epoll_fd, bool event_flag, int fd)
{
	struct epoll_event ev;

	(void)socket;

	ev.data.fd = fd;
	if (event_flag == true)
		ev.events = EPOLLIN;
	else
		ev.events = EPOLLOUT;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1)
	{
		if (errno == ENOENT)
		{
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
				return (false);
			return (true);
		}
		return (false);
	}
	return (true);
}

bool CgiSocket::setDelEpollEvent(int epoll_fd, int fd)
{
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		if (errno == ENOENT)
		{
			return (true);
		}
		return (false);
	}
	return (true);
}
