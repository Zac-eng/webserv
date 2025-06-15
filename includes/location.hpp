#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "nginx.hpp"
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <sstream>
#include <set>
#include <map>


class LocationConfig
{
	public:
		void setPath(const std::string& p);
		void setRoot(const std::string& r);
		void setPathParser(const std::string& path);
		void setRedirectFlag(bool flag);
		void setRedirectMap(int status_code, const std::string& url);
		void setFastcgiIndex(const std::string& idx);
		void setFastcgiPass(const std::string& pass);
		void setIndexCount(const std::vector<int>& counts);
		void setIndexPushCount(const std::vector<int>& counts);
		void setFastcgiParam(const std::map<std::string, std::string>& params);
		void setMethodCount(const std::vector<int>& counts);
		void setautoindex(bool on_off);
		
		const std::string& getPath() const;
		const std::string& getRoot() const;
		const std::string& getFastcgiIndex() const;
		const std::string& getFastcgiPass() const;
		const std::vector<std::string>& getIndexFiles() const;
		const std::vector<int>& getIndexCount() const;
		const std::vector<int>& getIndexPushCount() const;
		const std::map<std::string, std::string>& getFastcgiParam() const;
		const std::vector<std::string>& getMethod() const;
		const std::vector<int>& getMethodCount() const;
		const std::string& getPathParser() const;
		const std::map<int, std::string>& getRedirectMap() const;
		bool getRedirectFlag() const;
		bool getautoindex() const;	
		bool check_location(std::istream& config_file, LocationConfig& location_config, const ServerConfig& config);
		void setIndex(const std::vector<std::string>& indexes);
		void setMethod(const std::vector<std::string>& methods);
		void parseReturnDirective(const std::string& line, LocationConfig& config);
		void parseConfFile(const std::string& filename, std::vector<LocationConfig>& configs);
		LocationConfig() : path(""), redirect_flag(false) {}
	private:
		std::string path;
		std::string root;
		std::string autoindex;
		bool on_off;
		std::string path_parser;
		bool redirect_flag;
		std::map<int, std::string> redirect_map;
		std::vector<std::string> allow_method;
		std::string fastcgi_index;
		std::string fastcgi_pass;
		std::vector<int> index_count;
		std::vector<int> index_push_count;
		std::vector<int> allow_method_count;
		std::map<std::string, std::string> fastcgi_param;
		std::vector<std::string> index_files;

		void reset();
		bool handle_root(std::stringstream& stream, LocationConfig& location_config);
		bool handle_index(std::stringstream& stream, LocationConfig& location_config);
		bool handle_autoindex(std::stringstream& stream, LocationConfig& location_config);
		bool handle_allow_methods(std::stringstream& stream, LocationConfig& location_config);
		bool handle_fastcgi_index(std::stringstream& stream, LocationConfig& location_config);
		bool handle_fastcgi_pass(std::stringstream& stream, LocationConfig& location_config);
		bool handle_fastcgi_param(std::stringstream& stream, LocationConfig& location_config);
		bool handle_return(std::stringstream& stream, LocationConfig& location_config);
		bool handle_path_parser(std::stringstream& stream, LocationConfig& location_config);
};



#endif