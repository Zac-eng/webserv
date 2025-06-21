#include "CgiMetaProcessors.hpp"

const char **create_meta_vars(const ServerConfig& conf, const Request& req, const sockaddr_in& addr) {
  (void)addr;
  std::vector<std::string> meta_vars;
  Auth auth_info = CgiMetaProcessors::get_auth_info(req);
  CgiPath cgi_path = CgiMetaProcessors::get_path_info(conf, req);

  // RemoteInfo remote_info = CgiMetaProcessors::get_remote_info(addr);
  meta_vars.push_back("AUTH_TYPE=" + auth_info.auth_type);
  meta_vars.push_back("CONTENT_LENGTH=" + CgiMetaProcessors::get_content_length(req));
  meta_vars.push_back("CONTENT_TYPE=" + CgiMetaProcessors::get_content_type(req));
  meta_vars.push_back("GATEWAY_INTERFACE=" + CgiMetaProcessors::get_gateway_interface());
  meta_vars.push_back("PATH_INFO=" + req.getPathInfo());
  meta_vars.push_back("PATH_TRANSLATED=" + cgi_path.translated);
  meta_vars.push_back("QUERY_STRING=" + cgi_path.query_string);
  // meta_vars.push_back("REMOTE_ADDR=" + remote_info.remote_addr);
  // meta_vars.push_back("REMOTE_HOST=" + remote_info.remote_host);
  meta_vars.push_back("REMOTE_USER=" + auth_info.remote_user);
  meta_vars.push_back("REQUEST_METHOD=" + CgiMetaProcessors::get_request_method(req));
  meta_vars.push_back("SCRIPT_FILENAME=" + req.getFile());
  meta_vars.push_back("REDIRECT_STATUS=200");
  meta_vars.push_back("SCRIPT_NAME=" + cgi_path.script_name);
  meta_vars.push_back("SERVER_NAME=" + CgiMetaProcessors::get_server_name(conf));
  meta_vars.push_back("SERVER_PORT=" + CgiMetaProcessors::get_server_port(conf));
  meta_vars.push_back("SERVER_PROTOCOL=" + CgiMetaProcessors::get_server_protocol());
  meta_vars.push_back("SERVER_SOFTWARE=" + CgiMetaProcessors::get_server_software());

  int meta_var_num = meta_vars.size();
  const char **meta_var_array = new const char*[meta_var_num + 1];
  meta_var_array[meta_var_num] = NULL;
  for (int i = 0; i < meta_var_num; ++i) {
    meta_var_array[i] = new char[meta_vars[i].length() + 1];
    std::memset((char*)meta_var_array[i], '\0', meta_vars[i].length() + 1);
    std::strncpy((char*)meta_var_array[i], meta_vars[i].c_str(), meta_vars[i].length() + 1);
  }
  std::cerr << "meta vars: " << meta_var_array[4] << meta_var_array[5] << meta_var_array[1] << std::endl;
  return meta_var_array;
}

Auth CgiMetaProcessors::get_auth_info(const Request& req) {
  Auth ret_val;
  ret_val.auth_type = "";
  ret_val.remote_user = "";

  std::map<std::string, std::string>::const_iterator auth_iter = req.getHeader().find("Authorization");
  if (auth_iter != req.getHeader().end()) {
    std::string auth = auth_iter->second;
    size_t colon_pos = auth.find(':');
    if (colon_pos != std::string::npos) {
      ret_val.auth_type = auth.substr(0, colon_pos);
      ret_val.remote_user = auth.substr(colon_pos + 1, auth.length());
    }
  }
  return ret_val;
}

CgiPath CgiMetaProcessors::get_path_info(const ServerConfig& conf, const Request& req) {
  CgiPath ret_val;
  ret_val.script_name = "";
  ret_val.path_info = "";
  ret_val.translated = "";
  ret_val.query_string = "";

  std::string path = req.getPath();

  std::vector<LocationConfig>::const_iterator it = conf.getLocations().begin();
  std::vector<LocationConfig>::const_iterator matched_location = conf.getLocations().end();
  size_t matching_prefix_len = 0;

  if (!path.empty()) {
    size_t question_pos = path.find('?');
    if (question_pos != std::string::npos) {
      ret_val.query_string = path.substr(question_pos + 1, path.length());
    }
    std::string filepath = path.substr(0, question_pos);
    size_t script_path_pos = filepath.find(CGI_EXTENTION, 0);
    if (script_path_pos != std::string::npos) {
      size_t border_pos = script_path_pos + sizeof(CGI_EXTENTION) / sizeof(char);
      ret_val.script_name = filepath.substr(0, border_pos);
      // ret_val.path_info = filepath.substr(border_pos + 1, filepath.length());
    }
    for (; it != conf.getLocations().end(); ++it) {
      if (it->getPath().length() < matching_prefix_len)
        continue;
      if (filepath.find(it->getPath(), 0) == 0) {
        matched_location = it;
        matching_prefix_len = it->getPath().length();
      }
    }
    if (matched_location != conf.getLocations().end()) {
      ret_val.translated = ret_val.script_name;
      ret_val.translated.replace(0, matching_prefix_len, matched_location->getRoot());
    }
  }
  return ret_val;
}

RemoteInfo CgiMetaProcessors::get_remote_info(const sockaddr_in& client_addr) {
  RemoteInfo ret_val;
  ret_val.remote_addr = "";
  ret_val.remote_host = "";

  char hostname[NI_MAXHOST];
  const char* ip_addr = inet_ntoa(client_addr.sin_addr);
  ret_val.remote_addr = ip_addr;
  int status = getnameinfo((struct sockaddr*)&client_addr, sizeof(client_addr),
                             hostname, sizeof(hostname),
                             NULL, 0, NI_NAMEREQD);
  if (status == 0)
    ret_val.remote_host = hostname;
  return ret_val;
}

std::string CgiMetaProcessors::get_content_length(const Request& req) {
  std::map<std::string, std::string>::const_iterator cl_iter = req.getHeader().find("content-length");
  if (cl_iter != req.getHeader().end()) {
    std::cerr << "length" << cl_iter->second << std::endl;
    return cl_iter->second;
  }
  return "";
}

std::string CgiMetaProcessors::get_content_type(const Request& req) {
  std::map<std::string, std::string>::const_iterator ct_iter = req.getHeader().find("content-type");
  if (ct_iter != req.getHeader().end()) {
    std::cerr << "content type" << ct_iter->second << std::endl;
    return ct_iter->second;
  }
  return "";
}

std::string CgiMetaProcessors::get_gateway_interface(void) {
  return "CGI/1.1";
}

std::string CgiMetaProcessors::get_request_method(const Request& req) {
  return req.getMethod();
}

std::string CgiMetaProcessors::get_server_name(const ServerConfig& conf) {
  // return conf.getServerName();
  std::cerr << conf.getServerName() << std::endl;
  return "localhost";
}

std::string CgiMetaProcessors::get_server_port(const ServerConfig& conf) {
  std::stringstream ss;
  ss << conf.getListenPort();
  return ss.str();
}

std::string CgiMetaProcessors::get_server_protocol(void) {
  return "HTTP/1.1";
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
