#include "Address/AddressReader.hpp"
#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    AddressReader* AddressReader::instance = nullptr;

    AddressReader* AddressReader::getInstance() {
        if (!instance)
            instance = new AddressReader();
        return instance;
    }

    std::string AddressReader::ReadString(File& file) {
        u16 len = 0;
        if (file.Read(&len, sizeof(len)) != File::SUCCESS || len == 0) {
            return "";
        }

        std::string str(len, '\0');
        if (file.Read(&str[0], len) != File::SUCCESS) {
            return "";
        }

        return str;
    }

    bool AddressReader::loadFromBinary(const std::string& filePath, const std::string& regionName) {
        File f;
        if (File::Open(f, filePath, File::READ) != File::SUCCESS) {
            f.Close();
            return false;
        }

        entries.clear();
        loaded = false;

        u16 regionCount = 0;
        if (f.Read(&regionCount, sizeof(regionCount)) != File::SUCCESS || regionCount == 0) {
            f.Close();
            return false;
        }

        for (u16 i = 0; i < regionCount; ++i) {
            std::string region = ReadString(f);

            u16 entryCount = 0;
            if (f.Read(&entryCount, sizeof(entryCount)) != File::SUCCESS)
                break;

            if (region == regionName) {
                entries.reserve(entryCount);
                for (u16 j = 0; j < entryCount; ++j) {
                    std::string key = ReadString(f);
                    u32 address = 0;
                    if (f.Read(&address, sizeof(address)) != File::SUCCESS)
                        break;

                    entries.push_back({ key, address });
                }

                loaded = true;
                f.Close();
                return true;
            } else {
                for (u16 j = 0; j < entryCount; ++j) {
                    std::string skipKey = ReadString(f);
                    f.Seek(sizeof(u32), File::CUR);
                }
            }
        }

        f.Close();
        return false;
    }

    u32 AddressReader::get(const std::string& key) const {
        if (!loaded)
            return 0;

        for (auto& e : entries) {
            if (e.key == key)
                return e.address;
        }
        return 0;
    }
}
