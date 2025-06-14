#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <limits>
#include <cstring>

class BitcoinExchange
{
	private:
		std::map<std::string, float> _csv;

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
		void CaluculateBitcoin(std::ifstream& input, std::ifstream &csv);
		bool ParseInputfile(std::ifstream& input);
		bool SearchCsvfile(std::ifstream& csv);
		void CaluculateValue(const std::string& date, const float value);
		bool ValidValue(std::string& key, std::string& value);
		bool CheckErrorFlag(const std::string& date, const float value);
	int ChangeDateToInt(const std::string& date);
};

namespace Error {
	void OpenError(void);
	bool GetlineFlag(const std::ifstream& input);
	bool FileHead(const std::ifstream& input);
	bool FileError(const std::ifstream& input);
	bool StringStreamError(const std::istringstream& input);
	bool ValueError(const std::istringstream& input);
	void Overflow(void);
	void Negative(void);
	void BadInput(const std::string& date);
	void InputfileError(void);
}