#include "Response.hpp"
#include "Request.hpp"

Response::Response() {};

Response::~Response()
{
	std::cout << "Response object destroyed" << std::endl;
}

// bool Response::ExistUri(std::string& uri)
// {
// 	DIR *dir;
// 	dirent *entry;

// 	dir = opendir(uri.c_str());
// 	if (dir == NULL)
// 		return (false);
// 	entry = readdir(dir);
// 	while (entry != NULL)
// 	{
// 		if (entry->d_name == uri)
// 			return (true);
// 		entry = readdir(dir);
// 	}
// 	return (false);
// }

// bool Response::ReadUri(Request& req)
// {
// 	if (ReadFile() == false)
// 		return (false);
// 	CreateResponse(req, StatusCode::OK);
// 	return (true);
// }

// void Response::CreateResponseOK(Response& req)
// {
// 	CreateResponseHeader(req);
// }

// void Response::HandlePost(Request& req)
// {
// 	CreateFile(req);
// 	return ;
// }

// void Rsaponse::HandleGet(Request& req)
// {
// 	ReadFile();
// 	return (StatusMessage::OK())
// }

// void Rsaponse::HandleMethod(Request& req)
// {
// 	if (req.GetMethod() == "GET")
// 		HandleGet(req);
// 	else if (req.GetMethod() == "POST")
// 		HandlePost(req);
// 	else if (req.GetMethod() == "DELETE")
// 		HandleDelete(req);
// 	else
// 		return ;
// }

// void Response::IsDynamicFileType(const std::string& uri)
// {
	
// }

// void Response::ExecuteAndGetStatusCode(Request& req)
// {
// 	bool type = false;

// 	type = IsDynamicFileType(req.GetUri());
// 	if (type == true)
// 		ExecuteCGI(req);
// 	else
// 		HandleMethod(req);
// 	return ;
// }

// void Response::CreateResponse(Request& req)
// {
// 	if (number == OK)
// 		CreateResponseOK(req);
// 	else if (number == BadRequest)
// 		CreateResponseBadRequest(req);
// 	else if (number == NotFound)
// 		CreateResponseNotFound(req);
// 	else
// 		return ;
// }

void Response::ExecuteResponse(Request& req)
{
	// (this->ExistUri(req.GetPath()) == false)
	// 	return (Error::InvalidUri());
	// this->ExecuteAndGetStatusCode(req);
	// Createresponse(req);
}
