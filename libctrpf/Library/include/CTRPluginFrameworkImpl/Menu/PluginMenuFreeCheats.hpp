#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUFREECHEATS_HPP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUFREECHEATS_HPP

#define FREECHEATS 0

#if FREECHEATS

#include "types.h"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryFreeCheat.hpp"
#include "CTRPluginFrameworkImpl/Menu/HexEditor.hpp"
#include "CTRPluginFrameworkImpl/Graphics/UIContainer.hpp"

namespace CTRPluginFramework
{
    class FreeCheats
    {
        using EventList = std::vector<Event>;
        using SavedCheat = Preferences::SavedCheats;
    public:
        FreeCheats(HexEditor &hexEditor);
        ~FreeCheats(void) { _instance = nullptr; }

        void    Create(u32 address, u8 value) const;
        void    Create(u32 address, u16 value) const;
        void    Create(u32 address, u32 value) const;
        //void    Create(u32 address, u64 value) const;
        void    Create(u32 address, float value) const;
        //void    Create(u32 address, double value) const;
        void    Create(SavedCheat &savedCheat) const;

        // Display menu
        // Return true if menu must close
        bool operator()(EventList &eventList);

        static FreeCheats   *GetInstance(void);

        static void     DisableAll(void);
        static void     LoadFromFile(Preferences::Header &header, File &file);
        static void     WriteToFile(Preferences::Header& header, File &file);
    private:
        HexEditor                           &_hexEditor;
        bool                                _inEditor;
        bool                                _mustSave;
        bool                                _nameChanged;
        std::string                         _savedName;
        Menu                                _menu;
        NumericTextBox                      _addressTextBox;
        NumericTextBox                      _valueTextBox;
        CheckedButton<FreeCheats, void>     _hexBtn;
        Button<FreeCheats, void>            _duplicateBtn;
        Button<FreeCheats, void>            _saveBtn;
        Button<FreeCheats, void>            _cancelBtn;
        Button<FreeCheats, void>            _deleteBtn;
        Button<FreeCheats, void>            _changeNameBtn;
        Button<FreeCheats, void>            _openInEditorBtn;

        CheckBox                            _u8CheckBox;
        CheckBox                            _u16CheckBox;
        CheckBox                            _u32CheckBox;

        UIContainer                         _uiContainer;

        MenuEntryFreeCheat                  *_selectedFC;

        static FreeCheats                   *_instance;

        void    _DrawBottom(void);
        void    _ProcessEvent(EventList &eventList);
        void    _UpdateInfos();
        void    _Update(void);
        void    _OpenInEditorBtn_OnClick(void);
        void    _ChangeNameBtn_OnClick(void);
        void    _DuplicateBtn_OnClick(void);
        void    _SaveBtn_OnClick(void);
        void    _CancelBtn_OnClick(void);
        void    _DeleteBtn_OnClick(void);
    };
}

#endif
#endif
