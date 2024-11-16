#include "Response.hpp"

Response::Response(int stream_fd) {
  std::stringstream ss = createStream(stream_fd);
  std::string line;

  _version = parseHttpVersion(ss);
  if (_version < 0)
    throw std::exception();
  _status_code = parseStatusCode(ss);
  if (_status_code == Error)
    throw std::exception();
  getline(ss, line);
  while (!line.empty() && line != "\r") {
    parseHeader(line);
    getline(ss, line);
    if (ss.fail() || ss.bad())
      throw std::exception();
  }
  std::cout << ss.str() << std::endl;
  setBody(ss.str());
}

Response::Response(double version, StatusCode status_code) {
  _version = version;
  _status_code = status_code;
}

Response::~Response() {}

double Response::parseHttpVersion(std::stringstream& ss) {
  double version;
  std::string line;

  getline(ss, line, '/');
  if (ss.fail() || ss.bad() || ss.eof() || line != "HTTP")
    return -1.0;
  ss >> version;
  if (ss.fail() || ss.bad() || ss.eof())
    return -1.0;
  return version;
}

Response::StatusCode Response::parseStatusCode(std::stringstream& ss) {
  std::string status_message;
  StatusCode status_code;
  int tmp;

  ss >> tmp;
  if (ss.fail() || ss.bad() || ss.eof())
    return Error;
  status_code = static_cast<StatusCode>(tmp);
  ss.ignore(1);
  getline(ss, status_message);
  if (status_message.empty() || resolveStatusCode(status_code) != status_message.erase(status_message.length() - 1))
    return Error;
  return status_code;
}

void Response::parseHeader(const std::string& line) {
  std::size_t colon = line.find(':');

  setHeader(line.substr(0, colon), line.substr(colon + 1, line.length() - 1));
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
    case BadRequest:
      return "Bad Request";
    case NotFound:
      return "NotFound";
    default:
      return "";
  }
}
