#ifndef TEXTFILEPARSER_HPP
#define TEXTFILEPARSER_HPP

/*
All credit for those functions goes to PabloMK7
*/

#pragma once

#include <CTRPluginFramework.hpp>
#include <map>
#include <string>
#include <vector>
#include <tuple>

namespace CTRPluginFramework {
	
	class TextFileParser {
	public:
		using TextMap = std::map<std::string, std::vector<std::string>>;
		using TextMapIter = TextMap::iterator;
		using TextMapConstIter = TextMap::const_iterator;

		TextFileParser();
		bool 							Parse(std::string filename);
		const std::string& 				Get(const std::string& key, u32 element = 0);
		TextMapIter 					begin();
		TextMapIter 					end();
		TextMapConstIter 				cbegin();
		TextMapConstIter 				cend();
		TextMapConstIter 				erase(TextMapIter first, TextMapIter last);

		~TextFileParser();

		static bool 		IsNumerical(const std::string& str, bool isHex);

	private:
		TextMap dataMap;
		std::vector<std::string> vecData;
		const std::vector<std::string>& _FindKey(const std::string& key);
	};

	std::string& Trim(std::string& str);
	
	extern TextFileParser* colorparser;
	extern TextFileParser* Language;
	extern TextFileParser* fwkparser;
}

#endif