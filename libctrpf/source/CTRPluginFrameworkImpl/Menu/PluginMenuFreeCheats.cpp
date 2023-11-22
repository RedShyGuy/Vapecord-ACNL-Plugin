#include "CTRPluginFrameworkImpl/Menu/PluginMenuFreeCheats.hpp"

#if FREECHEATS

#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include <string>
#include "CTRPluginFramework/Menu/MessageBox.hpp"

namespace CTRPluginFramework
{
    FreeCheats      *FreeCheats::_instance = nullptr;

    static bool   GetName(std::string &name)
    {
        Keyboard    keyboard;

        keyboard.DisplayTopScreen = false;
        //keyboard.CanAbort(false);
        keyboard.SetCompareCallback([](const void *in, std::string &error)
        {
            bool isValid = true;
            const std::string *input = reinterpret_cast<const std::string*>(in);

            if (input->empty())
            {
                isValid = false;
                error = "Error, name can't be empty.";
            }

            return (isValid);
        });

        return (keyboard.Open(name) == 0);
    }

    FreeCheats::FreeCheats(HexEditor &hexEditor) :
        _hexEditor(hexEditor),
        _inEditor(false),
        _mustSave(false),
        _nameChanged(false),
        _menu("Free cheats"),
        _addressTextBox(160, 60, 130, 15),
        _valueTextBox(160, 80, 130, 15),
        _hexBtn("Hex", *this, nullptr, IntRect(120, 80, 38, 15), nullptr),
        _saveBtn("Save", *this, &FreeCheats::_SaveBtn_OnClick, IntRect(40, 180, 73, 15)),
        _cancelBtn("Cancel", *this, &FreeCheats::_CancelBtn_OnClick, IntRect(123, 180, 73, 15)),
        _deleteBtn("Delete", *this, &FreeCheats::_DeleteBtn_OnClick, IntRect(206, 180, 73, 15)),
        _openInEditorBtn("Open in HexEditor", *this, &FreeCheats::_OpenInEditorBtn_OnClick, IntRect(40, 120, 240, 15)),
        _changeNameBtn("Change name", *this, &FreeCheats::_ChangeNameBtn_OnClick, IntRect(40, 140, 240, 15)),
        _duplicateBtn("Duplicate", *this, &FreeCheats::_DuplicateBtn_OnClick, IntRect(40, 160, 240, 15)),
        _u8CheckBox(60, 100),
        _u16CheckBox(140, 100),
        _u32CheckBox(220, 100)
    {
        if (_instance == nullptr)
            _instance = this;
        else
        {
            delete _instance;
            _instance = this;
        }

        _hexBtn.UseSysFont(false);
        _saveBtn.UseSysFont(false);
        _cancelBtn.UseSysFont(false);
        _deleteBtn.UseSysFont(false);
        _openInEditorBtn.UseSysFont(false);
        _changeNameBtn.UseSysFont(false);
        _duplicateBtn.UseSysFont(false);

        _valueTextBox.UseHexadecimal(false);

        _saveBtn.IsLocked = true;
        _cancelBtn.IsLocked = true;

        _uiContainer += &_addressTextBox;
        _uiContainer += &_valueTextBox;
        _uiContainer += &_hexBtn;
        _uiContainer += &_duplicateBtn;
        _uiContainer += &_saveBtn;
        _uiContainer += &_cancelBtn;
        _uiContainer += &_deleteBtn;
        _uiContainer += &_changeNameBtn;
        _uiContainer += &_openInEditorBtn;
        _uiContainer += &_u8CheckBox;
        _uiContainer += &_u16CheckBox;
        _uiContainer += &_u32CheckBox;
    }

    void    FreeCheats::Create(u32 address, u8 value) const
    {
        std::string name;

        if(GetName(name))
            _menu.Append(new MenuEntryFreeCheat(name, address, value));
    }

    void    FreeCheats::Create(u32 address, u16 value) const
    {
        std::string name;

        if (GetName(name))
            _menu.Append(new MenuEntryFreeCheat(name, address, value));
    }

    void    FreeCheats::Create(u32 address, u32 value) const
    {
        std::string name;

        if (GetName(name))
            _menu.Append(new MenuEntryFreeCheat(name, address, value));
    }

  /*  void    FreeCheats::Create(u32 address, u64 value) const
    {
        std::string name;

        if(GetName(name))
            _menu.Append(new MenuEntryFreeCheat(name, address, value));
    } */

    void    FreeCheats::Create(u32 address, float value) const
    {
        std::string name;

        if (GetName(name))
            _menu.Append(new MenuEntryFreeCheat(name, address, value));
    }

    /*void    FreeCheats::Create(u32 address, double value) const
    {
        std::string name;

        if (GetName(name))
            _menu.Append(new MenuEntryFreeCheat(name, address, value));
    }*/

    void    FreeCheats::Create(SavedCheat &savedCheat) const
    {
        _menu.Append(new MenuEntryFreeCheat(savedCheat));
    }

    bool    FreeCheats::operator()(EventList& eventList)
    {
        if (_inEditor)
        {
            if (_hexEditor(eventList))
                _inEditor = false;
            return (false);
        }

        _ProcessEvent(eventList);
        _Update();

        if (_addressTextBox())
        {
            _mustSave = true;
            _saveBtn.IsLocked = false;
            _cancelBtn.IsLocked = false;
        }

        if (_valueTextBox())
        {
            _mustSave = true;
            _saveBtn.IsLocked = false;
            _cancelBtn.IsLocked = false;
        }

        _saveBtn();
        _cancelBtn();
        _deleteBtn();
        _openInEditorBtn();
        _changeNameBtn();
        _duplicateBtn();

        if (_hexBtn())
        {
            _valueTextBox.UseHexadecimal(_hexBtn.GetState());
        }

        if (_u8CheckBox() && _selectedFC != nullptr)
        {
            _selectedFC->SetType(Type_e::Bits8);
            _UpdateInfos();
        }

        if (_u16CheckBox() && _selectedFC != nullptr)
        {
            _selectedFC->SetType(Type_e::Bits16);
            _UpdateInfos();
        }

        if (_u32CheckBox() && _selectedFC != nullptr)
        {
            _selectedFC->SetType(Type_e::Bits32);
            _UpdateInfos();
        }

        // Draw Top
        Renderer::SetTarget(TOP);
        _menu.Draw();

        _DrawBottom();

        return (Window::BottomWindow.MustClose());
    }

    FreeCheats  *FreeCheats::GetInstance(void)
    {
        return (_instance);
    }

    void    FreeCheats::DisableAll(void)
    {
        if (_instance == nullptr)
            return;

        _instance->_menu.GetFolder()->DisableAll();
    }

    void    FreeCheats::LoadFromFile(Preferences::Header &header, File &file)
    {
        if (_instance == nullptr || header.freeCheatsCount == 0)
            return;

        std::vector<SavedCheat> freecheats;
        u32     count = header.freeCheatsCount;

        file.Seek(header.freeCheatsOffset, File::SET);

        while (count > 0)
        {
            u32 nb = count > 100 ? 100 : count;

            freecheats.resize(nb);

            if (file.Read(freecheats.data(), nb * sizeof(SavedCheat)) != 0)
                break;

            for (SavedCheat &fc : freecheats)
                _instance->Create(fc);

            count -= nb;
        }
    }

    void    FreeCheats::WriteToFile(Preferences::Header &header, File &file)
    {
        if (_instance == nullptr)
            return;

        if (_instance->_menu.GetFolder()->ItemsCount())
        {
            u64             offset = file.Tell();
            MenuFolderImpl  *folder = _instance->_menu.GetFolder();
            u32             count = folder->ItemsCount();
            u32             index = 0;
            std::vector<SavedCheat> svcheats;

            while (count > 0)
            {
                u32 nb = count > 100 ? 100 : count;

                svcheats.resize(nb);

                for (int i = 0; i < nb && index < folder->ItemsCount(); ++i, ++index)
                {
                    reinterpret_cast<MenuEntryFreeCheat *>(folder->_items[index])->ToSavedSearch(svcheats[i]);
                }

                if (file.Write(svcheats.data(), nb * sizeof(SavedCheat)) != 0)
                    goto error;

                count -= nb;
            }
            header.freeCheatsCount = folder->ItemsCount();
            header.freeCheatsOffset = offset;
            error:
                return;
        }
    }

    void    FreeCheats::_DrawBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();
        const Color &textcolor = Preferences::Settings.MainTextColor;

        // If no cheat selected == No free cheat
        if (_selectedFC == nullptr)
        {
            static const u8 *errorstr = (u8 *)"You don't have any cheats here.\nGo in Search to start finding some !";
            int posY = 70;

            Renderer::DrawSysString("\uE010\uE010  \uE00A  \uE010\uE010", 117, posY, 300, textcolor);
            posY += 10;
            Renderer::DrawSysStringReturn(errorstr, 35, posY, 300, textcolor);
            return;
        }

        int posY = 63;

        Renderer::DrawString("Address: ", 35, posY, textcolor);
        posY = 83;
        Renderer::DrawString("Freeze val.: ", 35, posY, textcolor);

        /*
        // Draw TextBox
        _addressTextBox.Draw();
        _valueTextBox.Draw();

        // Draw checkboxes and labels
        _u8CheckBox.Draw();
        _u16CheckBox.Draw();
        _u32CheckBox.Draw(); */
        posY = 104;
        Renderer::DrawString("8Bits", 77, posY, textcolor); posY = 104;
        Renderer::DrawString("16Bits", 157, posY,textcolor); posY = 104;
        Renderer::DrawString("32Bits", 237, posY, textcolor);

        // Draw UIControls
        _uiContainer.Draw();
        /*
        // Draw buttons
        _hexBtn.Draw();
        _saveBtn.Draw();
        _cancelBtn.Draw();
        _deleteBtn.Draw();
        _changeNameBtn.Draw();
        _openInEditorBtn.Draw();
        _duplicateBtn.Draw();*/
    }

    void    FreeCheats::_ProcessEvent(EventList& eventList)
    {
        for (Event &event : eventList)
        {
            MenuItem *item = nullptr;
            MenuEvent menuevent = (MenuEvent)_menu.ProcessEvent(event, &item);

            if (menuevent == MenuEvent::SelectorChanged)
            {
                if (_mustSave)
                    _CancelBtn_OnClick();
                _selectedFC = reinterpret_cast<MenuEntryFreeCheat*>(item);
                _UpdateInfos();
            }
            if (menuevent == MenuEvent::MenuClose)
                Window::BottomWindow.Close();
        }
    }

    void    FreeCheats::_UpdateInfos(void)
    {
        if (_selectedFC != nullptr)
        {
            // If we deleted all cheats
            if (_menu.GetFolder()->ItemsCount() == 0)
            {
                _selectedFC = nullptr;
                return;
            }

            _addressTextBox.SetValue(_selectedFC->Address);

            if (_selectedFC->Type == Type_e::Bits8)
            {
                _valueTextBox.SetValue(_selectedFC->Value.Bits8);
                _u8CheckBox.SetState(true);
                _u16CheckBox.SetState(false);
                _u32CheckBox.SetState(false);
            }
            if (_selectedFC->Type == Type_e::Bits16)
            {
                _valueTextBox.SetValue(_selectedFC->Value.Bits16);
                _u8CheckBox.SetState(false);
                _u16CheckBox.SetState(true);
                _u32CheckBox.SetState(false);
            }
            if (_selectedFC->Type == Type_e::Bits32)
            {
                _u8CheckBox.SetState(false);
                _u16CheckBox.SetState(false);
                _u32CheckBox.SetState(true);
                _valueTextBox.SetValue(_selectedFC->Value.Bits32);
            }
            //if (_selectedFC->Type == Type_e::Bits64) _valueTextBox.SetValue(_selectedFC->Value.Bits64);
            //if (_selectedFC->Type == Type_e::Float) _valueTextBox.SetValue(_selectedFC->Value.Float);
            //if (_selectedFC->Type == Type_e::Double) _valueTextBox.SetValue(_selectedFC->Value.Double);
        }
    }

    void    FreeCheats::_Update(void)
    {
        bool        isTouchDown = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        if (_selectedFC == nullptr)
        {
            _selectedFC = reinterpret_cast<MenuEntryFreeCheat *>(_menu.GetSelectedItem());
            _UpdateInfos();
        }

        Window::BottomWindow.Update(isTouchDown, touchPos);

        if (_selectedFC != nullptr)
        {
            // Update UIControls
            _uiContainer.Update(isTouchDown, touchPos);
            /*
            _addressTextBox.Update(isTouchDown, touchPos);
            _valueTextBox.Update(isTouchDown, touchPos);
            _hexBtn.Update(isTouchDown, touchPos);
            _saveBtn.Update(isTouchDown, touchPos);
            _cancelBtn.Update(isTouchDown, touchPos);
            _deleteBtn.Update(isTouchDown, touchPos);
            _openInEditorBtn.Update(isTouchDown, touchPos);
            _changeNameBtn.Update(isTouchDown, touchPos);
            _duplicateBtn.Update(isTouchDown, touchPos);
            _u8CheckBox.Update(isTouchDown, touchPos);
            _u16CheckBox.Update(isTouchDown, touchPos);
            _u32CheckBox.Update(isTouchDown, touchPos);
            */
        }
    }

    void    FreeCheats::_OpenInEditorBtn_OnClick(void)
    {
        _hexEditor.Goto(_selectedFC->Address);
        _inEditor = true;
    }

    void    FreeCheats::_ChangeNameBtn_OnClick(void)
    {
        _savedName = _selectedFC->name;

        Keyboard    keyboard;
        std::string input;

        keyboard.DisplayTopScreen = false;

        if (keyboard.Open(input) != -1)
        {
            _selectedFC->name = input;
            _mustSave = true;
            _nameChanged = true;
            _saveBtn.IsLocked = false;
            _cancelBtn.IsLocked = false;
        }
    }

    void    FreeCheats::_SaveBtn_OnClick(void)
    {
        _mustSave = false;
        _nameChanged = false;
        _selectedFC->Address = _addressTextBox.Bits32;
        _selectedFC->Value.Bits32 = _valueTextBox.Bits32;
        _savedName.clear();
        _saveBtn.IsLocked = true;
        _cancelBtn.IsLocked = true;
    }

    void    FreeCheats::_CancelBtn_OnClick(void)
    {
        if (_nameChanged)
            _selectedFC->name = _savedName;
        _mustSave = false;
        _nameChanged = false;
        _saveBtn.IsLocked = true;
        _cancelBtn.IsLocked = true;
    }

    void    FreeCheats::_DeleteBtn_OnClick(void)
    {
        std::string msg("Warning\nDo you really want to delete:\n");

        msg += _selectedFC->name;

        if (!MessageBox(msg, DialogType::DialogYesNo)())
            return;

        _menu.Remove(_selectedFC);
        delete _selectedFC;

        _mustSave = false;
        _nameChanged = false;
        _savedName.clear();
        _saveBtn.IsLocked = true;
        _cancelBtn.IsLocked = true;

        _selectedFC = reinterpret_cast<MenuEntryFreeCheat*>(_menu.GetSelectedItem());
        _UpdateInfos();
    }

    void    FreeCheats::_DuplicateBtn_OnClick(void)
    {
        if (_selectedFC == nullptr)
            return;

        _menu.Append(new MenuEntryFreeCheat(*_selectedFC));
    }
}

#endif
