#pragma once

#include <iostream>

class NginxSetting {
	private:
		unsigned int _server_count;
		std::string _default_server_number;
		std::vector<NginxServer> _server_name;

	public:
		NginxSetting();
		virtual ~NginxSetting();
		void DefaultSetting(std::string& nginx_setting_path);
		void FileReading(std::string& nginx_setting_path)
		// defaultのroot,index,locationを設定
};

class NginxServer {
	private:
		std::string _server_name;
		unsigned int _port;
		std::string _root;
	
	public:
		NginxServer();
		~NginxServer();
		void ServerNameSetting();
		// server_nameの設定をカウント
};