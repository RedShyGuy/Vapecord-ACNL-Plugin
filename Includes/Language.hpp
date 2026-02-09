#pragma once
#include <CTRPluginFramework.hpp>
#include "TextID.hpp"
#include <vector>
#include <string>

namespace CTRPluginFramework {
    class Language {
    private:
        struct LangHeader {
            std::string shortName;
            std::string fullName;
            u32 offset;
            u32 size;

            bool operator==(const std::string& other) const {
                return shortName == other;
            };
        };

        std::vector<u8> buffer;
        const LangHeader* currentHeader;
        std::vector<u16> offsets;
        bool loaded = false;
        std::string currentLang;

        static Language* instance;
        Language() = default;

    public:
        static Language* getInstance();

        bool verifyVersion(const std::string& filePath, u32 expectedVersion);

        static std::vector<LangHeader> listAvailableLanguages(const std::string& filePath);

        bool loadFromBinary(const std::string& filePath, const std::string& lang);

        std::string get(TextID id) const;

        const std::string& getCurrentLang() const { 
            return currentLang; 
        }
    };
}