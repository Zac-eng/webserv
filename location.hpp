#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <sstream>
#include <set>
#include <map>

// class LocationConfig
// {
// 	public:
// 		std::string path;
// 		std::string root;
// 		// std::vector<std::string> allow_methods;
// 		std::string method;
// 		std::string fastcgi_index;
// 		std::string fastcgi_pass;
// 		std::vector<int> index_count;
// 		std::vector<int> index_push_count;
// 		std::map<std::string, std::string> fastcgi_param;
// 		void setPath(const std::string& p) { path = p; }
// 		// void addAllowMethod(const std::string& method) { allow_methods.push_back(method); }
// 		bool check_location(std::istream& config_file, LocationConfig& location_config);		
// 		void setIndex(const std::vector<std::string>& indexes);
// 		std::vector<std::string> index_files;
// 	private:

// };

class LocationConfig
{
	public:
		void setPath(const std::string& p);
		void setRoot(const std::string& r);
		void setMethod(const std::string& m);
		void setFastcgiIndex(const std::string& idx);
		void setFastcgiPass(const std::string& pass);
		void setIndexCount(const std::vector<int>& counts);
		void setIndexPushCount(const std::vector<int>& counts);
		void setFastcgiParam(const std::map<std::string, std::string>& params);

		const std::string& getPath() const;
		const std::string& getRoot() const;
		const std::string& getMethod() const;
		const std::string& getFastcgiIndex() const;
		const std::string& getFastcgiPass() const;
		const std::vector<std::string>& getIndexFiles() const;
		const std::vector<int>& getIndexCount() const;
		const std::vector<int>& getIndexPushCount() const;
		const std::map<std::string, std::string>& getFastcgiParam() const;

		bool check_location(std::istream& config_file, LocationConfig& location_config);

	private:
		std::string path;
		std::string root;
		std::string method;
		std::string fastcgi_index;
		std::string fastcgi_pass;
		std::vector<int> index_count;
		std::vector<int> index_push_count;
		std::map<std::string, std::string> fastcgi_param;
		std::vector<std::string> index_files;
};


#endif
