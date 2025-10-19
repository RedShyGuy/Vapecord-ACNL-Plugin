#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <string>
#include <vector>

namespace CTRPluginFramework {
    class Language {
    private:
        struct Entry {
            std::string key;
            std::string value;
        };

        std::vector<Entry> translations;
        std::string currentLang;
        std::string filePath;
        bool loaded = false;
        static Language *instance;

    public:
        static Language* getInstance();
        bool loadFromBinary(const std::string& filePath, const std::string& lang);
        std::string get(const std::string& key) const;
        const std::string& getCurrentLang() const { return currentLang; }
    };
}
#endif