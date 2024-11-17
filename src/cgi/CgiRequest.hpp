#pragma once
#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

# include "message/Request.hpp"
# include "message/Response.hpp"

class CgiRequest: public Request {

public:
  CgiRequest(const Request& base);
  Response Call(void) const;

private:
  CgiRequest(void);

};

#endif
