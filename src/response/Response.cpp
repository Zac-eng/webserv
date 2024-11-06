#include "Response.hpp"

Response::Response(double version, StatusCode status_code) {
  _version = version;
  _status_code = status_code;
}

void Response::setHeader(const std::string& key, const std::string& value) {
  std::map<std::string, std::vector<std::string> >::iterator current_header;

  current_header = _headers.find(key);
  if (_headers.find(key) == _headers.end()) {
    std::vector<std::string> vec;
    vec.push_back(value);
    _headers.insert(std::make_pair(key, vec));
  } else if (canDuplicate(key)) {
    current_header->second.push_back(value);
  } else {
    throw BadHeaderException();
  }
}

void Response::setBody(std::string body) {
  _message_body = body;
}

void Response::sendResponse(int stream_fd) {
  std::string contents = flattenContents();
  if (send(stream_fd, contents.c_str(), contents.length(), 0) < 0)
    std::cerr << "failed to send http response" << std::endl;
}

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

bool Response::canDuplicate(const std::string header_key) const {
  return header_key == "Set-Cookie" || header_key == "Warning" || header_key == "Link" || header_key == "WWW-Authenticate";
}
