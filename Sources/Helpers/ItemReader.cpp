#include "CTRPluginFramework.hpp"
#include "Helpers/ItemReader.hpp"
#include "LibCtrpfExtras/UtilsExtras.hpp"

namespace CTRPluginFramework {
    ItemReader* ItemReader::instance = nullptr;

    ItemReader* ItemReader::getInstance() {
        if (!instance)
            instance = new ItemReader();
        return instance;
    }

    bool ItemReader::loadFromBinary(const std::string &path) {
        File file;
        if (File::Open(file, path, File::READ) < 0 || !file.IsOpen()) {
            return false;
        }

        u64 size = file.GetSize();
        if (size == 0) {
            file.Close();
            return false;
        }

        // Read the entire file at once
        std::vector<char> buffer(size);
        if (file.Read(buffer.data(), size) < 0) {
            file.Close();
            return false;
        }
        file.Close();

        entries.clear();
        entries.reserve(5500);

        u64 pos = 0;
        while (pos + sizeof(u16) * 2 <= size) { // ensure we can read id + len
            u16 id, len;

            // Safe copy
            std::memcpy(&id, &buffer[pos], sizeof(u16));
            pos += sizeof(u16);

            std::memcpy(&len, &buffer[pos], sizeof(u16));
            pos += sizeof(u16);

            if (pos + len > size) {
                break; // check for out-of-bounds
            }

            // Create string directly from buffer
            std::string text(&buffer[pos], len);
            UtilsExtras::ConvertToLowcase(text);
            pos += len;

            entries.push_back({Item(id), std::move(text)});
        }

        loaded = true;
        return true;
    }

    std::string ItemReader::get(Item item) const {
        for(Entry entry : entries) {
			if(entry.item.ID == item.ID) {
                return entry.name;
			}
		}
        return "???";
    }

    Item* ItemReader::searchByName(const std::string& name) const {
        std::vector<ItemReader::Entry> items = searchAllByName(name);
        if (items.empty()) {
            return nullptr;
        }

        // look for exact match among the results
        int pos = 0;
        for(Entry item : items) {
            if(item.name == name) {
                return new Item(item.item);
            }
        }

        // get first found item if no exact match
        return new Item(items[0].item);
    }

    std::vector<ItemReader::Entry> ItemReader::searchAllByName(const std::string& name) const {
        std::vector<Entry> items;

        std::string nameCopy = name;
        UtilsExtras::ConvertToLowcase(nameCopy);

		for(Entry entry : entries) {
            std::string entryName = entry.name;
			if(entryName.find(nameCopy) != std::string::npos) {
                items.push_back(entry);
			}
		}

		return items;
    }
}