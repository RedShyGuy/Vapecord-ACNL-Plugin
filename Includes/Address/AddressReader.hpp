#ifndef ADDRESS_READER_HPP
#define ADDRESS_READER_HPP

#include <CTRPluginFramework.hpp>
#include <string>
#include <vector>

namespace CTRPluginFramework {
    class AddressReader {
    public:
        struct Entry {
            std::string key;
            u32 address;
        };

        struct RegionHeader {
            std::string name;
            u32 offset;
            u32 size;
        };

        static AddressReader* getInstance();

        bool loadFromBinary(const std::string& filePath, const std::string& regionName);
        u32 get(const std::string& key) const;
    private:
        static AddressReader* instance;

        bool loaded = false;
        std::vector<Entry> entries;
    };
}
#endif