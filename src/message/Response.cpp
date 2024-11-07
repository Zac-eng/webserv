#include "Response.hpp"

Response::Response(int stream_fd) {
  
}

Response::Response(double version, StatusCode status_code) {
  _version = version;
  _status_code = status_code;
}

Response::~Response() {}

std::string Response::flattenContents(void) const {
  std::stringstream ss;
  std::map<std::string, std::vector<std::string> >::const_iterator header_iter;

  ss << "HTTP/" << _version << " " << _status_code << " " << resolveStatusCode(_status_code) << "\r\n";
  for (header_iter = _headers.begin(); header_iter != _headers.end(); ++header_iter) {
    for (std::vector<std::string>::const_iterator it = header_iter->second.begin(); it != header_iter->second.end(); ++it) {
      ss << header_iter->first << ": " << *it << "\r\n";
    }
  }
  ss << "\r\n";
  ss << _message_body;
  return ss.str();
}

std::string Response::resolveStatusCode(StatusCode status_code) const {
  switch (status_code)
  {
    case OK:
      return "OK";
    case NotFound:
      return "NotFound";
    default:
      return "";
  }
}
