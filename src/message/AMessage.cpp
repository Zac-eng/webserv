#include "AMessage.hpp"

AMessage::AMessage() {}

AMessage::~AMessage() {}

void AMessage::setHeader(const std::string& key, const std::string& value) {
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

void AMessage::setContentLength(void) {
  std::size_t content_length = _message_body.length();
  std::stringstream ss;

  ss << content_length;
  setHeader("Content-Length", ss.str());
}

void AMessage::setBody(const std::string& body) {
  _message_body = body;
}

void AMessage::Send(int stream_fd) const {
  std::string contents = flattenContents();
  if (send(stream_fd, contents.c_str(), contents.length(), 0) < 0)
    std::cerr << "failed to send http AMessage" << std::endl;
}

bool AMessage::canDuplicate(const std::string header_key) const {
  return header_key == "Set-Cookie" || header_key == "Warning" || header_key == "Link" || header_key == "WWW-Authenticate";
}
