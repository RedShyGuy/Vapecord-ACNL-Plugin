#ifndef MENUENTRYHELPERS_HPP
#define MENUENTRYHELPERS_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    /**
     * \brief Return the arg of an entry \n
     * If the arg doesn't exist (nullptr) a new one is created calling the default type constructor
     * \tparam T The type of the arg
     * \param entry The entry to get the arg from
     * \return A pointer to the arg (like reinterpret_cast<T *>(entry->GetArg()))
     */
    template <typename T>
    T *GetArg(MenuEntry *entry) {
        T *arg = reinterpret_cast<T *>(entry->GetArg());

        if(arg == nullptr) {
            arg = new T();
            entry->SetArg(arg);
        }

        return(arg);
    }

    /**
    * \brief Return the arg of an entry \n
    * If the arg doesn't exist (nullptr) a new one is created calling the default type constructor
    * \tparam T The type of the arg
    * \param entry The entry to get the arg from
    * \param defaultValue The value to set to a newly created arg
    * \return A pointer to the arg (like reinterpret_cast<T *>(entry->GetArg()))
    */
    template <typename T>
    T *GetArg(MenuEntry *entry, T defaultValue) {
        T *arg = reinterpret_cast<T *>(entry->GetArg());

        if(arg == nullptr) {
            arg = new T(defaultValue);
            entry->SetArg(arg);
        }

        return(arg);
    }
}

#endif
