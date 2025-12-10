#pragma once

#include <CTRPluginFramework.hpp>
#include "Language.hpp"

namespace CTRPluginFramework {
    class MenuEntryExtras : public MenuEntry { 
        using FuncPointer = void(*)(MenuEntry*);
    public:
        /**
        * \brief Sets up a MenuEntry with the language key as the name
        * \param nameKey Name key
        * \param gameFunc Game function for the entry
        * \param menuFunc Menu function for the entry
        * \param noteKey Note Key for the entry (Optional)
        */
        MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, FuncPointer menuFunc, const std::string &noteKey = "");

        /**
        * \brief Sets up a MenuEntry with the language key as the name
        * \param nameKey Name key
        * \param gameFunc Game function for the entry
        * \param noteKey Note Key for the entry (Optional)
        */
        MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, const std::string &noteKey = "");
    private:
        static std::string setLanguageByKey(const std::string& langKey) {
            Language* lang = Language::getInstance();
            return lang->get(langKey);
        }
    };
}