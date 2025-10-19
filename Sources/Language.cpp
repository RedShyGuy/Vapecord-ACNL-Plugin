#include "language.hpp"
#include <CTRPluginFramework.hpp>
#include <malloc.h>

namespace CTRPluginFramework {
    Language* Language::instance = nullptr;

    Language* Language::getInstance() {
        if (!instance) {
            instance = new Language();
        }
        return instance;
    }

    void PrintMemoryInfo() {
        s64 used = 0;
        s64 total = 0;

        // 0 = Application Memory (für Plugins relevant)
        svcGetSystemInfo(&total, 0, 0);  
        svcGetSystemInfo(&used, 0, 1);   

        OSD::Notify(Utils::Format("Memory Used: %lld KB / %lld KB", used / 1024, total / 1024));
    }

    void PrintHeapUsage() {
        struct mallinfo info = mallinfo();
        OSD::Notify(Utils::Format("Heap Used: %d KB", info.uordblks / 1024));
    }

    bool Language::loadFromBinary(const std::string &file, const std::string &lang) {
        Clock timer;
        OSD::Notify("Before:");
        PrintMemoryInfo();
        PrintHeapUsage();

        filePath = file;
        currentLang = lang;

        File f;
        if (File::Open(f, file, File::READ) != File::SUCCESS) {
            f.Close();
            return false;
        }

        // Header [uint32 langCount]
        u32 langCount = 0;
        if (f.Read(&langCount, sizeof(langCount)) != File::SUCCESS) {
            f.Close();
            return false;
        }

        struct LangHeader {
            u8 nameLen;
            std::string name;
            u32 offset;
            u32 size;
        };

        std::vector<LangHeader> headers;
        headers.reserve(langCount);

        // Read language headers
        for (u32 i = 0; i < langCount; ++i) {
            u8 nameLen = 0;
            f.Read(&nameLen, sizeof(nameLen));

            std::string langName(nameLen, '\0');
            f.Read(&langName[0], nameLen);

            u32 offset = 0, size = 0;
            f.Read(&offset, sizeof(offset));
            f.Read(&size, sizeof(size));

            headers.push_back({nameLen, langName, offset, size});
        }

        // Search for target language
        const LangHeader* target = nullptr;
        for (auto &h : headers) {
            if (h.name == lang) {
                target = &h;
                break;
            }
        }

        if (!target) {
            f.Close();
            return false;
        }

        // Read entire language block in one go
        f.Seek(target->offset, File::SET);
        std::vector<u8> buffer(target->size);
        if (f.Read(buffer.data(), target->size) != File::SUCCESS) {
            f.Close();
            return false;
        }
        f.Close();

        const u8* ptr = buffer.data();
        const u8* end = ptr + buffer.size();

        // Entry count
        if (ptr + sizeof(u32) > end) {
            return false;
        }
        u32 entryCount = *reinterpret_cast<const u32*>(ptr);
        ptr += sizeof(u32);

        translations.clear();
        translations.reserve(entryCount);

        for (u32 j = 0; j < entryCount && ptr < end; ++j) {
            if (ptr + sizeof(u16) > end) {
                break;
            }
            u16 keyLen = *reinterpret_cast<const u16*>(ptr);
            ptr += sizeof(u16);

            if (ptr + keyLen > end) {
                break;
            }
            std::string key(reinterpret_cast<const char*>(ptr), keyLen);
            ptr += keyLen;

            if (ptr + sizeof(u16) > end) {
                break;
            }
            u16 valLen = *reinterpret_cast<const u16*>(ptr);
            ptr += sizeof(u16);

            if (ptr + valLen > end) {
                break;
            }
            std::string val(reinterpret_cast<const char*>(ptr), valLen);
            ptr += valLen;

            translations.emplace_back(Entry{ key, val });
        }

        loaded = true;

        OSD::Notify("After:");
        PrintMemoryInfo();
        PrintHeapUsage();

        Time elapsed = timer.GetElapsedTime();
        OSD::Notify(Utils::Format("Dauer: %.3f Sekunden", elapsed.AsSeconds()));

        return true;
    }

    std::string Language::get(const std::string &key) const
    {
        if (!loaded) {
            return "[not loaded]";
        }
            
        for (auto &pair : translations) {
            if (pair.key == key) {
                return pair.value;
            }
        }

        return "[" + key + "]";
    }
}