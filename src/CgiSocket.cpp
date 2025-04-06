#include "CgiSocket.hpp"

CgiSocket::CgiSocket(
  pid_t cgi_pid,
  int read_fd,
  int write_fd,
  const std::string& request_body,
  std::string& response
): _request_body(request_body), _response_body(response) {
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
) : _request_body(obj._request_body), _response_body(obj._response_body) {
  *this = obj;
}

CgiSocket& CgiSocket::operator = (const CgiSocket& obj) {
  if (this == &obj)
    return *this;
  this->_cgi_pid = obj._cgi_pid;
  this->_pipe_fds[READ] = obj._pipe_fds[READ];
  this->_pipe_fds[WRITE] = obj._pipe_fds[WRITE];
  this->_response_body = obj._response_body;
  // this->_request_body = obj._request_body;
  time(&this->_created_at);
  return *this;
}

CgiSocket* CgiSocket::createCgiSocket(
  const ServerConfig& conf,
  const Request& req,
  std::string& response_buf
) {
  int ptc_pipe[2];
  int ctp_pipe[2];

  if (pipe(ptc_pipe) != 0)
    return NULL;
  if (pipe(ctp_pipe) != 0) {
    close(ptc_pipe[READ]);
    close(ptc_pipe[WRITE]);
    return NULL;
  }
  pid_t pid = fork();
  if (pid < 0) {
    close_pipes(ptc_pipe, ctp_pipe);
    return NULL;
  }
  else if (pid == 0) {
    const char *args[] = {CMD_PATH, req.GetUri().c_str(), NULL};
    if (close(ptc_pipe[WRITE]) != 0 \
    || close(ctp_pipe[READ]) != 0 \
    || dup2(ptc_pipe[READ], 0) != 0 \
    || dup2(ctp_pipe[WRITE], 1) != 0) {
      close_pipes(ptc_pipe, ctp_pipe);
      std::exit(1);
    }
    const char **meta_vars = create_meta_vars(conf, req);
    if (execve(CMD_PATH, (char **)args, (char **)meta_vars) != 0)
      std::exit(1);
  }
  else {
    if (close(ptc_pipe[READ]) != 0 || close(ctp_pipe[WRITE]) != 0) {
      close_pipes(ptc_pipe, ctp_pipe);
      kill(pid, SIGINT);
      return NULL;
    }
    return new CgiSocket(pid, ctp_pipe[READ], ptc_pipe[WRITE], req._body, response_buf);
  }
}

bool CgiSocket::handleEpollInEvent(int epoll_fd, std::map<int, ASocket*>& _socket) {
  std::string response_body;
  char read_buf[BUFFER_SIZE];
  int read_count;
  time_t current_time;
  int status;

  while (true) {
    time(&current_time);
    if (current_time > this->_created_at + CGI_TIMEOUT) {
      kill(this->_cgi_pid, SIGINT);
      return false;
    }
    read_count = read(this->_pipe_fds[READ], read_buf, BUFFER_SIZE - 1);
    if (read_count < 0)
      return false;
    read_buf[read_count] = '\0';
    response_body += read_buf;
    if (read_count < BUFFER_SIZE - 1)
      break ;
  }
  this->_response_body = response_body;
  while (true) {
    waitpid(this->_cgi_pid, &status, WNOHANG);
    time(&current_time);
    if (current_time > this->_created_at + CGI_TIMEOUT)
      kill(this->_cgi_pid, SIGINT);
  }
}

void CgiSocket::handleEpollOutEvent() {
  if (this->_request_body.empty())
    return ;
  if (write(this->_pipe_fds[WRITE], this->_request_body.c_str(), this->_request_body.length()) < 0)
    return ;
}

const char **create_meta_vars(const ServerConfig& conf, const Request& req) {
  std::vector<std::string> meta_vars;
  Auth auth_info = CgiMetaProcessors::get_auth_info(req);
  CgiPath cgi_path = CgiMetaProcessors::get_path_info(req);
  meta_vars.push_back("AUTH_TYPE=" + auth_info.auth_type);
  meta_vars.push_back("CONTENT_LENGTH=" + CgiMetaProcessors::get_content_length(req));
  meta_vars.push_back("CONTENT_TYPE=" + CgiMetaProcessors::get_content_type(req));
  meta_vars.push_back("GATEWAY_INTERFACE=" + CgiMetaProcessors::get_gateway_interface());
  // meta_vars.push_back("PATH_INFO=" + CgiMetaProcessors::get_path_info(req));
  // meta_vars.push_back("PATH_TRANSLATED=" + CgiMetaProcessors::get_path_translated(req));
  // meta_vars.push_back("QUERY_STRING=" + CgiMetaProcessors::get_query_string(req));
  meta_vars.push_back("REMOTE_ADDR=" + CgiMetaProcessors::get_remote_addr(req));
  meta_vars.push_back("REMOTE_HOST=" + CgiMetaProcessors::get_remote_host(req));
  meta_vars.push_back("REMOTE_USER=" + auth_info.remote_user);
  meta_vars.push_back("REQUEST_METHOD=" + CgiMetaProcessors::get_request_method(req));
  // meta_vars.push_back("SCRIPT_NAME=" + CgiMetaProcessors::get_script_name(req));
  meta_vars.push_back("SERVER_NAME=" + CgiMetaProcessors::get_server_name(conf));
  meta_vars.push_back("SERVER_PORT=" + CgiMetaProcessors::get_server_port(conf));
  meta_vars.push_back("SERVER_PROTOCOL=" + CgiMetaProcessors::get_server_protocol());
  meta_vars.push_back("SERVER_SOFTWARE=" + CgiMetaProcessors::get_server_software());

  int meta_var_num = meta_vars.size();
  const char **meta_var_array = new const char*[meta_var_num];
  for (int i = 0; i < meta_var_num; ++i) {
    meta_var_array[i] = meta_vars[i].c_str();
  }
  return meta_var_array;
}

Auth CgiMetaProcessors::get_auth_info(const Request& req) {
  auto auth_iter = req._header.find("Authorization");
  if (auth_iter != req._header.end()) {
    std::string auth = auth_iter->second;
    size_t colon_pos = auth.find(':');
    if (colon_pos != std::string::npos) {
      return Auth {auth.substr(0, colon_pos), auth.substr(colon_pos + 1, auth.length())};
    }
  }
  return Auth {auth_type: "", remote_user: ""};
}

CgiPath CgiMetaProcessors::get_path_info(const Request& req) {
  // if ()
}

std::string CgiMetaProcessors::get_content_length(const Request& req) {
  auto cl_iter = req._header.find("Content-Length");
  if (cl_iter != req._header.end()) {
    return cl_iter->second;
  }
  return "";
}

std::string CgiMetaProcessors::get_content_type(const Request& req) {
  auto ct_iter = req._header.find("Content-Type");
  if (ct_iter != req._header.end()) {
    return ct_iter->second;
  }
  return "";
}

std::string CgiMetaProcessors::get_gateway_interface(void) {
  return "CGI/1.1";
}

// std::string CgiMetaProcessors::get_path_info(const Request& req, const ServerConfig& conf) {

// }

// std::string CgiMetaProcessors::get_path_translated(const Request& req, const ServerConfig& conf) {

// }

// std::string CgiMetaProcessors::get_query_string(const Request& req) {

// }

std::string CgiMetaProcessors::get_remote_addr(const sockaddr_in& client_addr) {

}

std::string CgiMetaProcessors::get_remote_host(const sockaddr_in& client_addr) {
  
}

std::string CgiMetaProcessors::get_request_method(const Request& req) {
  return req._method;
}

std::string CgiMetaProcessors::get_script_name(const Request& req, const ServerConfig& conf) {
  // return 
}

std::string CgiMetaProcessors::get_server_name(const ServerConfig& conf) {
  return conf.server_name;
}

std::string CgiMetaProcessors::get_server_port(const ServerConfig& conf) {
  std::stringstream ss;
  ss << conf.listen_port;
  return ss.str();
}

std::string CgiMetaProcessors::get_server_protocol(void) {
  return "http/1.1";
}

std::string CgiMetaProcessors::get_server_software(void) {
  return "webserv/1.1";
}


void close_pipes(int ptc_pipe[], int ctp_pipe[]) {
  close(ptc_pipe[READ]);
  close(ptc_pipe[WRITE]);
  close(ctp_pipe[READ]);
  close(ctp_pipe[WRITE]);
}
