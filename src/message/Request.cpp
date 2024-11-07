#include "Request.hpp"

Request::Request(double version, Method method, const std::string& path) {
  _version = version;
  _method = method;
  _path = path;
}

Request::~Request() {}

std::string Request::flattenContents(void) const {
  std::stringstream ss;
  std::map<std::string, std::vector<std::string> >::const_iterator header_iter;

  ss << resolveMethod(_method) << " " << _path << " HTTP/" << _version << "\r\n";
  for (header_iter = _headers.begin(); header_iter != _headers.end(); ++header_iter) {
    for (std::vector<std::string>::const_iterator it = header_iter->second.begin(); it != header_iter->second.end(); ++it) {
      ss << header_iter->first << ": " << *it << "\r\n";
    }
  }
  ss << "\r\n";
  ss << _message_body;
  return ss.str();
}

std::string Request::resolveMethod(Method method) const {
  switch (method)
  {
    case GET:
      return "GET";
    case POST:
      return "POST";
    case DELETE:
      return "DELETE";
    default:
      return "";
  }
}
