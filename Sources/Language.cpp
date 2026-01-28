#include "Language.hpp"

namespace CTRPluginFramework {
    Language* Language::instance = nullptr;

    Language* Language::getInstance() {
        if (!instance) instance = new Language();
        return instance;
    }

    std::vector<Language::LangHeader> Language::listAvailableLanguages(const std::string& filePath) {
        std::vector<LangHeader> langs;
        File f;
        if (File::Open(f, filePath, File::READ) != File::SUCCESS) {
            return langs;
        }

        u32 langCount = 0;
        f.Read(&langCount, sizeof(langCount));

        for (u32 i = 0; i < langCount; ++i) {
            u8 shortLen = 0, fullLen = 0;
            f.Read(&shortLen, sizeof(shortLen));
            std::string shortName(shortLen, '\0');
            f.Read(&shortName[0], shortLen);

            f.Read(&fullLen, sizeof(fullLen));
            std::string fullName(fullLen, '\0');
            f.Read(&fullName[0], fullLen);

            u32 offset = 0, size = 0;
            f.Read(&offset, sizeof(offset));
            f.Read(&size, sizeof(size));

            langs.push_back({shortName, fullName, offset, size});
        }
        f.Close();
        return langs;
    }

    bool Language::loadFromBinary(const std::string& filePath, const std::string& lang) {
        File f;
        if (File::Open(f, filePath, File::READ) != File::SUCCESS) {
            return false;
        }

        u32 langCount = 0;
        f.Read(&langCount, sizeof(langCount));

        std::vector<LangHeader> headers;
        for (u32 i = 0; i < langCount; ++i) {
            u8 shortLen = 0, fullLen = 0;
            f.Read(&shortLen, sizeof(shortLen));
            std::string shortName(shortLen, '\0');
            f.Read(&shortName[0], shortLen);

            f.Read(&fullLen, sizeof(fullLen));
            std::string fullName(fullLen, '\0');
            f.Read(&fullName[0], fullLen);

            u32 offset = 0, size = 0;
            f.Read(&offset, sizeof(offset));
            f.Read(&size, sizeof(size));

            headers.push_back({shortName, fullName, offset, size});
        }

        const LangHeader* target = nullptr;
        for (auto& h : headers) {
            if (h.shortName == lang) {
                target = &h;
                break;
            }
        }
        if (!target) {
            f.Close();
            return false;
        }

        buffer.resize(target->size);
        f.Seek(target->offset, File::SET);
        if (f.Read(buffer.data(), target->size) != File::SUCCESS) {
            f.Close();
            return false;
        }
        f.Close();

        currentHeader = target;
        currentLang = lang;

        const u8* ptr = buffer.data();
        if (ptr + sizeof(u16) > buffer.data() + buffer.size()) {
            return false;
        }

        u16 entryCount = *reinterpret_cast<const u16*>(ptr);
        ptr += sizeof(u16);

        offsets.resize(entryCount);
        for (u16 i = 0; i < entryCount; ++i) {
            if (ptr + sizeof(u16) > buffer.data() + buffer.size()) {
                return false;
            }

            offsets[i] = *reinterpret_cast<const u16*>(ptr);
            ptr += sizeof(u16);
        }

        loaded = true;
        return true;
    }

    std::string Language::get(TextID id) const {
        if (!loaded) {
            return "[not loaded]";
        }

        if (id == TextID::NONE) {
            return "";
        }

        u16 idx = static_cast<u16>(id);
        if (idx >= offsets.size()) {
            return "[invalid id]";
        }

        const u8* base = buffer.data();
        const u8* blobStart = base + 2 + offsets.size() * 2; // skip count + offset table
        return std::string(reinterpret_cast<const char*>(blobStart + offsets[idx]));
    }
}