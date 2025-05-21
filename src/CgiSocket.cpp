#include "CgiSocket.hpp"

CgiSocket::CgiSocket(
  // ServerConfig& conf,
  pid_t cgi_pid,
  int read_fd,
  int write_fd,
  Request& req,
  std::string& response
): _req(req), _response_body(response) {
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
  std::string& response_buf
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
    // not sure since req.getPath does not always return the "path" we need here
    const char *args[] = {CMD_PATH, req.getFile().c_str(), NULL};
    std::cout << req.getFile() << std::endl;
    if (!prepareChildPipes(ptc_pipe, ctp_pipe)) {
      close_pipes(ptc_pipe, ctp_pipe);
      std::exit(500);
    }
    const char **meta_vars = create_meta_vars(conf, req, client_addr);
    if (execve(CMD_PATH, (char **)args, (char **)meta_vars) != 0) {
      switch (errno) {
        case ENOENT:
          std::exit(404);
        default:
          std::exit(502);
      }
    }
    std::exit(0);
  }
  if (!prepareParentPipes(ptc_pipe, ctp_pipe)) {
    std::cout << "no pipes" << std::endl;
    req.setStatusNumber(500);
    close_pipes(ptc_pipe, ctp_pipe);
    kill(pid, SIGINT);
    return NULL;
  }
  // if (waitpid(pid, &status, WNOHANG))
  return new CgiSocket(pid, ctp_pipe[READ], ptc_pipe[WRITE], req, response_buf);
}

void CgiSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& socket) {
  std::string response_body;
  char read_buf[BUFFER_SIZE];
  int read_count;
  int status;

  std::cout << "cgi in event" << std::endl;
  while (true) {
    if (isTimeout()) {
      kill(this->_cgi_pid, SIGINT);
      return ;
    }
    read_count = read(this->_pipe_fds[READ], read_buf, BUFFER_SIZE - 1);
    if (read_count < 0)
      return ;
    read_buf[read_count] = '\0';
    response_body += read_buf;
    if (read_count < BUFFER_SIZE - 1)
      break;
  }
  this->_response_body = response_body;
  while (true) {
    if (waitpid(this->_cgi_pid, &status, WNOHANG) != 0) {
      break;
    }
    if (isTimeout())
      kill(this->_cgi_pid, SIGINT);
  }
  if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->_pipe_fds[READ], NULL) == -1) {
    perror("epoll_ctl: del");
  }
  std::map<int, ASocket*>::iterator self_pos = socket.find(_pipe_fds[READ]);
  if (self_pos != socket.end())
    socket.erase(self_pos);
  delete this;
  return ;
}

void CgiSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& socket) {
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = this->_pipe_fds[READ];

  std::cerr << "cgi out event" << std::endl;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->_pipe_fds[WRITE], NULL) == -1) {
    perror("epoll_ctl: del");
    return ;
  }
  std::map<int, ASocket*>::iterator write_epoll = socket.find(this->_pipe_fds[WRITE]);
  if (write_epoll != socket.end()) {
    socket.erase(write_epoll);
  }
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_pipe_fds[READ], &ev) == -1) {
    perror("epoll_ctl: add");
    return ;
  }
  socket.insert(std::make_pair(this->_pipe_fds[READ], this));
  if (this->_req.getBody().empty())
    return ;
  if (write(this->_pipe_fds[WRITE], this->_req.getBody().c_str(), this->_req.getBody().length()) < 0)
    return ;
  return ;
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
    dup2(ptc[READ], 0) != 0 || dup2(ctp[WRITE], 1) != 0) {
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

bool CgiSocket::isTimeout() {
  time_t current_time;

  time(&current_time);
  return (current_time > this->_created_at + CGI_TIMEOUT);
}

int		CgiSocket::getReadPipe() const {
  return this->_pipe_fds[READ];
}

int		CgiSocket::getWritePipe() const {
  return this->_pipe_fds[WRITE];
}
