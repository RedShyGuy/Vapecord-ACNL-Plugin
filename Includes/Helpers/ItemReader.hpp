#ifndef ITEM_READER_HPP
#define ITEM_READER_HPP

#include <CTRPluginFramework.hpp>
#include <string>
#include <vector>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
    class ItemReader {
    public:
        struct Entry {
            Item item;
            std::string name;
        };

        static ItemReader* getInstance();

        bool loadFromBinary(const std::string& filePath);
        std::string get(Item item) const;
        Item* searchByName(const std::string& name) const;
        std::vector<ItemReader::Entry> searchAllByName(const std::string& name) const;

        inline void clearEntries() {
            entries.clear();
            entries.shrink_to_fit();
            loaded = false;
        }

        inline bool isLoaded() const {
            return loaded;
        }
    private:
        static ItemReader* instance;

        bool loaded = false;
        std::vector<Entry> entries;
    };
}

#endif