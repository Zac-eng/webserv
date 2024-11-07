#pragma once
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

// # include <map>
// # include <vector>
// # include <string>
// # include <iostream>
// # include <exception>
// # include <algorithm>
// # include <sstream>
// # include <sys/socket.h>


// class Response {

// public:

//   enum StatusCode {
//     OK = 200,
//     NotFound = 404,
//   };

//   // construct Response from socket stream. will be used in client use
//   Response(int stream_fd);
//   // construct Response with necessary components. will be used in server use
//   Response(double version, StatusCode status_code);

//   void setHeader(const std::string& key, const std::string& value);
//   void setBody(std::string body);
//   void sendResponse(int stream_fd);

// private:
//   Response(void);

//   // necessary components
//   double _version;
//   StatusCode _status_code;

//   // optional, can be "empty"
//   std::map<std::string, std::vector<std::string> > _headers;
//   std::string _message_body;

//   // this exception will be used inside of setHeader
//   class BadHeaderException: std::exception {};

//   // internal functions
//   std::string flattenContents(void) const;
//   std::string resolveStatusCode(StatusCode status_code) const;
//   bool canDuplicate(const std::string header) const;

// };

// implementation for inheriting AMessage class
# include "AMessage.hpp"

class Response: public AMessage {

public:

  enum StatusCode {
    OK = 200,
    NotFound = 404,
  };

  // construct Response from socket stream. will be used in client use
  Response(int stream_fd);
  // construct Response with necessary components. will be used in server use
  Response(double version, StatusCode status_code);

  ~Response();

private:
  Response(void);

  // necessary components
  StatusCode _status_code;

  // internal functions
  std::string flattenContents(void) const;
  std::string resolveStatusCode(StatusCode status_code) const;

};

#endif
