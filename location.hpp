#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <sstream>
#include <set>

class LocationConfig
{
	public:
		std::string path;
		std::string root;
		std::string index;
		// std::vector<std::string> allow_methods;
		std::string method;
		std::vector<int> index_count;
		std::vector<int> index_push_count;
		void setPath(const std::string& p) { path = p; }
		// void addAllowMethod(const std::string& method) { allow_methods.push_back(method); }
		bool check_location(std::istream& config_file, LocationConfig& location_config);
		void setIndex(const std::vector<std::string>& indexes);
		std::vector<std::string> index_files;
	private:

};
bool check_location(std::istream& config_file, LocationConfig& location_config);

// class LocationConfig
// {
// 	private:
// 		std::string path;
// 		std::string root;
// 		std::set<std::string> allowed_methods;
// 		std::string index;
// 		bool autoindex;
// 	public:
// 	   // コンストラクタ
// 		LocationConfig()
// 			: path(""), root(""), index(""), autoindex(false) {}

// 		// セッター
// 		void setPath(const std::string& p) { path = p; }
// 		void setRoot(const std::string& r) { root = r; }
// 		void setIndex(const std::string& i) { index = i; }
// 		void setAutoindex(bool a) { autoindex = a; }
// 		void addAllowedMethod(const std::string& method) { allowed_methods.insert(method); }

// 		// ゲッター
// 		const std::string& getPath() const { return path; }
// 		const std::string& getRoot() const { return root; }
// 		const std::string& getIndex() const { return index; }
// 		bool getAutoindex() const { return autoindex; }
// 		const std::set<std::string>& getAllowedMethods() const { return allowed_methods; }

// 		// 検証関数
// 		void validate() const
// 		{
// 			if (path.empty())
// 				throw std::runtime_error("Location path is not set.");
// 			if (root.empty())
// 				throw std::runtime_error("Root path is not set in location block.");
// 		}
// };



#endif
