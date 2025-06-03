#ifndef CTRPLUGINFRAMEWORKIMPL_SearchMenu_HPP
#define CTRPLUGINFRAMEWORKIMPL_SearchMenu_HPP


#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Search/Search32.hpp"
#include "CTRPluginFrameworkImpl/Menu/HexEditor.hpp"
#include "CTRPluginFramework/System/File.hpp"

#include <string>
#include <vector>
#include "SubMenu.hpp"

namespace CTRPluginFramework
{
    class SearchMenu
    {
        using EventList = std::vector<Event>;
    public:

        SearchMenu(Search* &curSearch, HexEditor &hexEditor, bool &inEditor, bool &useHexInput);
        ~SearchMenu(){};

        bool    ProcessEvent(EventList &eventList, Time &delta);
        void    Draw(void);
        void    Update(void);

    private:
        HexEditor                   &_hexEditor;
        Search*                     &_currentSearch;
        SubMenu                     _submenu;
        std::vector<std::string>    _resultsAddress;
        std::vector<std::string>    _resultsNewValue;
        std::vector<std::string>    _resultsOldValue;

        int                         _selector;
        u32                         _index;
        bool                        _alreadyExported;
        bool                        &_inEditor;
		bool						&_useHexInput;
        File                        _export;

        void        _OpenExportFile(void);
        void        _NewCheat(void);
        void        _Edit(void);
        void        _JumpInEditor(void);
        void        _Export(void);
        void        _ExportAll(void);
        void        _ShowGame(void);

    };
}

#endif