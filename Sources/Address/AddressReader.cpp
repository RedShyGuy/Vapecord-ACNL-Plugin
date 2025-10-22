#include "Address/AddressReader.hpp"
#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    AddressReader* AddressReader::instance = nullptr;

    AddressReader* AddressReader::getInstance() {
        if (!instance)
            instance = new AddressReader();
        return instance;
    }

    bool AddressReader::loadFromBinary(const std::string& filePath, const std::string& regionName) {
        File file;
        if (File::Open(file, filePath, File::READ) != File::SUCCESS) {
            file.Close();
            return false;
        }

        entries.clear();
        loaded = false;

        // Region Count (u16)
        u16 regionCount = 0;
        if (file.Read(&regionCount, sizeof(regionCount)) != File::SUCCESS || regionCount == 0) {
            file.Close();
            return false;
        }

        std::vector<RegionHeader> headers;
        headers.reserve(regionCount);

        // Read region headers
        for (u16 i = 0; i < regionCount; ++i) {
            u8 nameLen = 0;
            if (file.Read(&nameLen, sizeof(nameLen)) != File::SUCCESS)
                return false;

            std::string name(nameLen, '\0');
            if (file.Read(name.data(), nameLen) != File::SUCCESS)
                return false;

            u32 offset = 0, size = 0;
            if (file.Read(&offset, sizeof(offset)) != File::SUCCESS) return false;
            if (file.Read(&size, sizeof(size)) != File::SUCCESS) return false;

            headers.push_back({ name, offset, size });
        }

        // Find target region
        const RegionHeader* target = nullptr;
        for (const auto& h : headers) {
            if (h.name == regionName) {
                target = &h;
                break;
            }
        }

        if (!target || target->size == 0) {
            file.Close();
            return false;
        }

        // Jump to region data
        if (file.Seek(target->offset, File::SET) != File::SUCCESS) {
            file.Close();
            return false;
        }

        std::vector<u8> buffer(target->size);
        if (file.Read(buffer.data(), target->size) != File::SUCCESS) {
            file.Close();
            return false;
        }

        file.Close();

        const u8* ptr = buffer.data();
        const u8* end = ptr + buffer.size();

        // Entry count
        if (ptr + sizeof(u16) > end)
            return false;

        u16 entryCount = 0;
        memcpy(&entryCount, ptr, sizeof(entryCount));
        ptr += sizeof(entryCount);

        entries.clear();
        entries.reserve(entryCount);

        // Parse entries
        for (u16 i = 0; i < entryCount && ptr < end; ++i) {
            if (ptr + 1 > end) break;
            u8 keyLen = *ptr++;
            if (ptr + keyLen + sizeof(u32) > end) break;

            std::string key(reinterpret_cast<const char*>(ptr), keyLen);
            ptr += keyLen;

            u32 addr = 0;
            memcpy(&addr, ptr, sizeof(addr));
            ptr += sizeof(addr);

            entries.push_back({ key, addr });
        }

        loaded = true;
        return true;
    }

    u32 AddressReader::get(const std::string& key) const {
        if (!loaded)
            return 0;

        for (auto& e : entries) {
            if (e.key == key) {
                return e.address;
            }          
        }
        return 0;
    }
}
