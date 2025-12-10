#pragma once

#include <string>
#include <vector>
#include <map>
#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    class Language {
        using TextMap = std::map<std::string, std::string>;
        using TextMapIter = TextMap::iterator;
		using TextMapConstIter = TextMap::const_iterator;

    private:
        struct LangHeader {
            std::string shortName;
            std::string fullName;
            u32 offset;
            u32 size;

            bool operator==(const std::string& other) const {
                return shortName == other;
            }
        };

        TextMap translations;
        std::string currentLang;
        std::string filePath;
        bool loaded = false;
        static Language *instance;

    public:
        static Language* getInstance();
        static std::vector<LangHeader> listAvailableLanguages(const std::string &filePath);
        bool loadFromBinary(const std::string& filePath, const std::string& lang);
        std::string get(const std::string& key);
        const std::string& getCurrentLang() const { return currentLang; }
    };
}