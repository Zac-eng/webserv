#include "CgiSocket.hpp"
#include "ClientSocket.hpp"

CgiSocket::CgiSocket(
  // ServerConfig& conf,
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
      perror("child pipes");
      close_pipes(ptc_pipe, ctp_pipe);
      std::exit(500);
    }
    const char **meta_vars = create_meta_vars(conf, req, client_addr);
    std::cerr << req.getFile().c_str() << std::endl;
    if (execve(req.getFile().c_str(), (char **)args, (char **)meta_vars) != 0) {
      switch (errno) {
        case ENOENT:
          perror("noent");
          std::exit(404);
        default:
          perror("cgi error");
          std::exit(502);
      }
    }
    std::exit(0);
  }
  if (!prepareParentPipes(ptc_pipe, ctp_pipe)) {
    req.setStatusNumber(500);
    perror("parent pipes");
    close_pipes(ptc_pipe, ctp_pipe);
    kill(pid, SIGINT);
    return NULL;
  }
  return new CgiSocket(pid, ctp_pipe[READ], ptc_pipe[WRITE], req, response_buf, client_fd);
}

void CgiSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& socket) {
  struct epoll_event ev;
  std::string response_body;
  std::stringstream ss;
  char read_buf[BUFFER_SIZE];
  int read_count;

  (void)socket;
  while (true) {
    if (isTimeout()) {
      perror("timeout");
      kill(this->_cgi_pid, SIGINT);
      return ;
    }
    read_count = read(this->_pipe_fds[READ], read_buf, BUFFER_SIZE - 1);
    if (read_count < 0) {
      perror("read");
      return ;
    }
    read_buf[read_count] = '\0';
    response_body += read_buf;
    if (read_count < BUFFER_SIZE - 1)
      break;
  }
  std::string crlf = "\r\n\r\n";
  std::size_t header_end = response_body.find(crlf);

  if (header_end != std::string::npos) {
    std::string body = response_body.substr(header_end + crlf.length());
    ss << "Content-Length: ";
    ss << body.length();
    ss << "\n";
  } else {
    ss << "Content-Length: ";
    ss << response_body.length();
    ss << "\n\n";
  }
  ss << response_body;
  this->_response_body = ss.str();
  std::cout << this->_response_body << std::endl;
  // while (true) {
  //   if (waitpid(this->_cgi_pid, &status, WNOHANG) != 0) {
  //     std::cout << "wait finished: " << status << std::endl;
  //     break;
  //   }
  //   if (isTimeout())
  //     kill(this->_cgi_pid, SIGINT);
  // }
  // if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->_pipe_fds[READ], NULL) == -1) {
  //   std::cout << epoll_fd << this->_pipe_fds[READ] << std::endl;
  //   perror("epollin_ctl: del");
  // }
  ev.events = EPOLLOUT;
  ev.data.fd = this->_client_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->_client_fd, &ev) == -1) {
    perror("epoll_ctl: add");
  }
  // std::map<int, ASocket*>::iterator self_pos = socket.find(_pipe_fds[READ]);
  // if (self_pos != socket.end())
  //   socket.erase(self_pos);
  // delete this;
  std::cout << "cgi finished" << std::endl;
  return ;
}

void CgiSocket::handleEpollOutEvent(int epoll_fd, std::map<int, ASocket*>& socket) {
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = this->_pipe_fds[READ];

  if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->_pipe_fds[WRITE], NULL) == -1) {
    perror("epoll_ctl: del");
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  std::map<int, ASocket*>::iterator write_epoll = socket.find(this->_pipe_fds[WRITE]);
  if (write_epoll != socket.end()) {
    socket.erase(write_epoll);
  }
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_pipe_fds[READ], &ev) == -1) {
    perror("epoll_ctl: add");
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  if (socket.insert(std::make_pair(this->_pipe_fds[READ], this)).second == false) {
    perror("epoll_ctl: add");
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  if (this->_req.getBody().empty()) {
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  std::string cgi_input = _req.getBoundary() + "\r\n" + _req.getBody() + _req.getBoundary() + "--\r\n";
  std::cout << cgi_input << " : " << cgi_input.length() << std::endl;
  if (write(this->_pipe_fds[WRITE], cgi_input.c_str(), cgi_input.length()) < 0) {
    perror("write failed");
    close(this->_pipe_fds[WRITE]);
    return ;
  }
  close(this->_pipe_fds[WRITE]);
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

int		CgiSocket::waitChildProcess() const {
  int status = 0;
  if (waitpid(this->_cgi_pid, &status, WNOHANG) == 0) {
    waitpid(this->_cgi_pid, &status, 0);
  }
  std::cout << "wait finished: " << status << std::endl;
  return status;
}
