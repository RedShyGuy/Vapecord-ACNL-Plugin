#include "TextFileParser.hpp"

/*All credits goes to PabloMK7*/

namespace CTRPluginFramework {
	TextFileParser* colorparser = new TextFileParser();
	TextFileParser* Language = new TextFileParser();
	TextFileParser* fwkparser = new TextFileParser();

	TextFileParser::TextFileParser() {	}

	static inline std::string& Ltrim(std::string& str) {
		auto it = std::find_if(str.begin(), str.end(), [](char ch) { return (!std::isspace(ch)); });
		str.erase(str.begin(), it);
		return str;
	}

	static inline std::string& Rtrim(std::string& str) {
		auto it = std::find_if(str.rbegin(), str.rend(), [](char ch) { return (!std::isspace(ch)); });
		str.erase(it.base(), str.end());
		return str;
	}

	std::string& Trim(std::string& str) {
		return (Ltrim(Rtrim(str)));
	}

	static u32 Str2U32(const std::string& str) {
		u32 val = 0;
		const u8* hex = (const u8*)str.c_str();

		if(str.size() < 4)
			return val;

		while(*hex) {
			u8 byte = (u8)*hex++;

			if(byte >= '0' && byte <= '9') 
				byte = byte - '0';
			else if(byte >= 'a' && byte <= 'f') 
				byte = byte - 'a' + 10;
			else if(byte >= 'A' && byte <= 'F') 
				byte = byte - 'A' + 10;
			else 
				return 0; ///< Incorrect char

			val = (val << 4) | (byte & 0xF);
		}
		return val;
	}

	static std::string ConvertToUTF8(const std::string& str) {
		u32 code = 0;
		char buffer[10] = { 0 };

		if(str[0] != '\\' && str[1] != 'u')
			return buffer;

		code = Str2U32(str.substr(2, 4));
		if(code == 0) {
			buffer[0] = 0xF; //Special character meaning \0
		}
		else {
			encode_utf8((u8*)buffer, code);
		}
		return buffer;
	}

	static void ProcessText(std::string& str) {
		if(str.empty()) 
			return;
		//Process our string
		for(u32 i = 0; i < str.size() - 1; i++) {
			const char c = str[i];

			//Check the symbol \n
			if(c == '\\' && str[i + 1] == 'n') {
				str[i] = '\n';

				str = str.erase(i + 1, 1);

				continue;
			}
			//Check the symbol \u
			if(c == '\\' && str[i + 1] == 'u') {
				std::string utf8 = ConvertToUTF8(str.substr(i));

				if(utf8.empty()) 
					continue;

				str.erase(i, 6);

				str.insert(i, utf8);

				continue;
			}

			if(c != '\\' && str[i + 1] == '#') {
				str.erase(i + 1);
				continue;
			}
		}
	}

	bool TextFileParser::Parse(std::string filename) {

		std::string line;
		File file(filename, File::READ);

		if(!file.IsOpen()) 
			return false;

		LineReader reader(file);

		u32 lineNumber = 0;
		const char utfbom[] = { 0xEF, 0xBB, 0xBF };
		std::vector<std::string> currVector;

		//Read our file until the last line
		for(; reader(line); lineNumber++) {
			currVector.clear();
			//If line is empty, skip it
			if(line.empty() || line[0] == '#') //Ignore lines with comments
				continue;

			//Skip the UTF-8 BOM indicator
			if(lineNumber == 0 && line.size() >= 3 && memcmp(line.data(), utfbom, 3) == 0)
				line = line.substr(3, line.npos);
			
			std::string key;
			std::string part;
			int i = 0;

			for(;;i++) {
				// Search our delimiter
				auto namePos = line.find("::");

				// Get key if i = 0
				if(i == 0) {
					key = line.substr(0, namePos);
					Trim(key);
				}
				else {
					part = line.substr(0, namePos);
					Trim(part);
					ProcessText(part);
					currVector.push_back(part);
				}

				// If we couldn't find the delimiter, break
				if(namePos == std::string::npos)
					break;

				line = line.substr(namePos + 2);	
			}
			if(currVector.empty()) 
				continue;
			//Add to our current map
			dataMap[key] = currVector;
		}
		return true;
	}

	static std::vector<std::string> g_empty = { "???" };
	
	const std::vector<std::string>& TextFileParser::_FindKey(const std::string& key) {
	//Search for the requested string in the current map
		TextMapIter it = dataMap.find(key);

	//If we found the requested string, returns it
		if(it != dataMap.end())
			return(it->second);

	//Since we couldn't find the requested string, returns an empty vector
		return g_empty;
	}

	const std::string& TextFileParser::Get(const std::string& key, u32 element) {
		const std::vector<std::string>& vec = _FindKey(key);
		if(element >= vec.size()) 
			return g_empty[0];
		
		return vec[element];	
	}

	TextFileParser::TextMapIter TextFileParser::begin() {
		return dataMap.begin();
	}

	TextFileParser::TextMapIter TextFileParser::end() {
		return dataMap.end();
	}

	TextFileParser::TextMapConstIter TextFileParser::cbegin() {
		return dataMap.cbegin();
	}

	TextFileParser::TextMapConstIter TextFileParser::cend() {
		return dataMap.cend();
	}

	TextFileParser::TextMapConstIter TextFileParser::erase(TextMapIter first, TextMapIter last) {
		return dataMap.erase(first, last);
	}

	TextFileParser::~TextFileParser() {	}
	bool TextFileParser::IsNumerical(const std::string& str, bool isHex) {
		int i = 0;
		for(const char c : str) {
			if(c < '0' || c > '9') {
				if(!isHex) 
					return false;
				else if(!((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) 
					return false;
			}
			i++;
		}
		if(!isHex) {
			if(i > 10) 
				return false;
			if(i == 10) {
				const char max[] = "4294967295";
				for(int i = 0; i <= 10; i++) {
					if(str[i] > max[i]) 
						return false;
				}
				return true;
			}
		}
		else {
			if(i > 8) 
				return false;
		}
		return true;
	}
}