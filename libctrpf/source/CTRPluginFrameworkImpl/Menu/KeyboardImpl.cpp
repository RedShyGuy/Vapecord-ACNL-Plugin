#include "CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"

#include <cmath>
#include "3ds.h"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFramework/Graphics/CustomIcon.hpp"

namespace CTRPluginFramework
{
    #define USER_VALID  0
    #define USER_ABORT  -1
    #define SLEEP_ABORT -2
    #define KEY_ENTER 0xA
    #define KEY_BACKSPACE 0x8
    #define KEY_SYMBOLS -2
    #define KEY_CAPS -3
    #define KEY_SMILEY -4
    #define KEY_SPACE -5
    #define KEY_SYMBOLS_PAGE -6
    #define KEY_NINTENDO_PAGE -7
    #define KEY_PLUS_MINUS -8

    std::vector<TouchKey>    KeyboardImpl::_DecimalKeys;
    std::vector<TouchKey>    KeyboardImpl::_HexaDecimalKeys;
    std::vector<TouchKey>    KeyboardImpl::_QwertyKeys;

    KeyboardImpl::KeyboardImpl(const std::string &text)
    {
        _owner = nullptr;

        _text = text;
        _error = "";
        _userInput = "";
        _canChangeLayout = false;
        _canAbort = true;
        _isOpen = false;
        _userAbort = false;
        _mustRelease = false;
        _useCaps = false;
        _useSymbols = false;
        _useNintendo = false;
        _errorMessage = false;
        _askForExit = false;
        _isHex = false;
        _offset = 0.f;
        _max = 0;
        _layout = HEXADECIMAL;
        _cursorPositionInString = 0;
        _cursorPositionOnScreen = 0;
        _showCursor = true;
        _keys = nullptr;
        _convert = nullptr;
        _compare = nullptr;
        _onKeyboardEvent = nullptr;

        _customKeyboard = false;
        _displayScrollbar = false;
        _currentPosition = 0;
        _scrollbarSize = 0;
        _scrollCursorSize = 10;
        _scrollPadding = 0.f;
        _scrollJump = 0.f;
        _scrollSize = 0.f;
        _scrollPosition = 0.f;
        _inertialVelocity = 0.f;
        _scrollStart = 0.f;
        _scrollEnd = 0.f;

        _symbolsPage = 0;
        _nintendoPage = 0;

        DisplayTopScreen = true;
    }

    KeyboardImpl::KeyboardImpl(Keyboard* kb, const std::string &text)
    {
        _owner = kb;

        _text = text;
        _error = "";
        _userInput = "";
        _canChangeLayout = false;
        _canAbort = true;
        _isOpen = false;
        _userAbort = false;
        _mustRelease = false;
        _useCaps = false;
        _useSymbols = false;
        _useNintendo = false;
        _errorMessage = false;
        _askForExit = false;
        _isHex = false;
        _offset = 0.f;
        _max = 0;
        _layout = HEXADECIMAL;
        _cursorPositionInString = 0;
        _cursorPositionOnScreen = 0;
        _showCursor = true;

        _convert = nullptr;
        _compare = nullptr;
        _onKeyboardEvent = nullptr;
        _keys = nullptr;

        _customKeyboard = false;
        _displayScrollbar = false;
        _currentPosition = 0;
        _scrollbarSize = 0;
        _scrollCursorSize = 10;
        _scrollPadding = 0.f;
        _scrollJump = 0.f;
        _scrollSize = 0.f;
        _scrollPosition = 0.f;
        _inertialVelocity = 0.f;
        _scrollStart = 0.f;
        _scrollEnd = 0.f;

        _symbolsPage = 0;
        _nintendoPage = 0;

        DisplayTopScreen = true;
    }

    KeyboardImpl::~KeyboardImpl(void)
    {
      //  for (KeyIter iter = _keys.begin(); iter != _keys.end(); ++iter)
      //      iter->Clear();
        for (TouchKeyString *tks : _strKeys)
            delete tks;
    }

    void    KeyboardImpl::SetLayout(Layout layout)
    {
        _canChangeLayout = false; ///< Reset state
        _layout = layout;
        _isHex = false;
        _userInput.clear();
        if (_layout == QWERTY) _Qwerty();
        else if (_layout == DECIMAL) _Decimal();
        else if (_layout == HEXADECIMAL)
        {
            _isHex = true;
            _Hexadecimal();
        }
    }

    void    KeyboardImpl::SetHexadecimal(bool isHex)
    {
        _isHex = isHex;
    }

    bool    KeyboardImpl::IsHexadecimal(void) const
    {
        return (_isHex);
    }

    void    KeyboardImpl::SetMaxInput(u32 max)
    {
        _max = max;
    }

    void    KeyboardImpl::CanAbort(bool canAbort)
    {
        _canAbort = canAbort;
    }

    std::string &KeyboardImpl::GetInput(void)
    {
        return (_userInput);
    }

    std::string &KeyboardImpl::GetMessage(void)
    {
        return (_text);
    }

    std::string& KeyboardImpl::GetTitle(void)
    {
        return _title;
    }

    void    KeyboardImpl::SetError(std::string &error)
    {
        _errorMessage = true;
        _error = error;
    }

    void    KeyboardImpl::SetConvertCallback(ConvertCallback callback)
    {
        _convert = callback;
    }

    void    KeyboardImpl::SetCompareCallback(CompareCallback callback)
    {
        _compare = callback;
    }

    void    KeyboardImpl::OnKeyboardEvent(OnEventCallback callback)
    {
        _onKeyboardEvent = callback;
    }

    void	KeyboardImpl::ChangeSelectedEntry(int entry) {
        if (!_customKeyboard)
            return;
        if (!_strKeys.size() || entry >= static_cast<int>(_strKeys.size()) || !_strKeys[entry]->CanUse())
            entry = -1;
        _ChangeManualKey(entry, false);
        // Update the position
        if (entry != -1 && _displayScrollbar) {
            int keyRow = _manualKey / (_isIconKeyboard ? 4 : 1);
            int positionRow = _currentPosition / (_isIconKeyboard ? 4 : 1);

            if (keyRow > positionRow + 6 - 2) {
                positionRow = keyRow - 4;
                _scrollSize = ((positionRow * 36.01f) + 15) / _scrollJump - _scrollPosition;
                _manualScrollUpdate = true;
                _UpdateScroll(0.f, true);
            }
            else if (keyRow < positionRow + 1) {
                positionRow = std::max(keyRow, 0);
                _scrollSize = ((positionRow * 36.01f) - 15) / _scrollJump - _scrollPosition;
                _manualScrollUpdate = true;
                _UpdateScroll(0.f, true);
            }
        }
    }

    void    KeyboardImpl::ChangeEntrySound(int entry, SoundEngine::Event soundEvent)
    {
        if (_customKeyboard && entry >= 0 && entry < static_cast<int>(_strKeys.size()) && _strKeys[entry]->CanUse())
        {
            _strKeys[entry]->SetAcceptSoundEvent(soundEvent);
        }
    }

    void    KeyboardImpl::Populate(const std::vector<std::string> &input, bool resetScroll)
    {
        bool mustReset = (_strKeys.size() != input.size()) || resetScroll || _isIconKeyboard;

        int count = input.size();

        if (mustReset)
            _ChangeManualKey(0, false);
        mustReset = (mustReset || count < 6);

        _customKeyboard = true;
        _isIconKeyboard = false;

        if (mustReset)
            _currentPosition = 0;

        std::vector<float> origPosY;

        for (TouchKeyString* tks : _strKeys)
        {
            if (!mustReset)
            {
                u16 posX; float posY;
                tks->GetPosition(posX, posY);
                origPosY.push_back(posY);
            }
            delete tks;
        }

        _strKeys.clear();


        int posY = (count < 6) ? (20 + (200 - ((30 * count) + 6 * (count - 1))) / 2) : 30;

        if (mustReset)
        {
            if (count < 6)
            {
                _displayScrollbar = false;
            }
            else
            {
                int height = 190;

                float lsize = 36.f * (float)count + 1;

                float padding = (float)height / lsize;
                int cursorSize = padding * height;
                float scrollTrackSpace = lsize - height;
                float scrollThumbSpace = height - cursorSize;

                _scrollJump = scrollTrackSpace / scrollThumbSpace;
                _scrollbarSize = height;

                if (cursorSize < 5)
                    cursorSize = 5;

                _scrollPadding = padding;
                _scrollCursorSize = cursorSize;
                _scrollPosition = 0.f;
                _scrollEnd = _scrollbarSize - _scrollCursorSize;
                _displayScrollbar = true;
            }
        }

        _scrollSize = 0;
        _inertialVelocity = 0;

        IntRect box(60, posY, 200, 30);
        int i = 0;
        for (const std::string &str : input)
        {
            TouchKeyString* tks = new TouchKeyString(str, box, true);

            if (!mustReset)
                tks->SetPosition(0xFFFF, origPosY[i++]);
            _strKeys.push_back(tks);
            if (mustReset) box.leftTop.y += 36;
        }

        origPosY.clear();
        _manualScrollUpdate = true;
        _UpdateScroll(0, true);
    }

    void    KeyboardImpl::Populate(const std::vector<CustomIcon>& input, bool resetScroll)
    {
        bool mustReset = (_strKeys.size() != input.size()) || resetScroll || _customKeyboard;

        int count = input.size() / 4;

        if (mustReset)
            _ChangeManualKey(0, false);

        mustReset = (mustReset || count < 6);

        _customKeyboard = true;
        _isIconKeyboard = true;

        if (mustReset)
            _currentPosition = 0;

        std::vector<float> origPosY;

        for (TouchKeyString* tks : _strKeys)
        {
            if (!mustReset)
            {
                u16 posX; float posY;
                tks->GetPosition(posX, posY);
                origPosY.push_back(posY);
            }
            delete tks;
        }
        _strKeys.clear();

        if (input.size() % 4 != 0) count++;

        int posY = (count < 6) ? (20 + (200 - ((30 * count) + 6 * (count - 1))) / 2) : 30;

        if (mustReset) {
            if (count < 6)
            {
                _displayScrollbar = false;
            }
            else if (mustReset)
            {
                int height = 190;


                float lsize = 36.f * (float)count + 1;

                float padding = (float)height / lsize;
                int cursorSize = padding * height;
                float scrollTrackSpace = lsize - height;
                float scrollThumbSpace = height - cursorSize;

                _scrollJump = scrollTrackSpace / scrollThumbSpace;
                _scrollbarSize = height;

                if (cursorSize < 5)
                    cursorSize = 5;

                _scrollPadding = padding;
                _scrollCursorSize = cursorSize;
                _scrollPosition = 0.f;
                _scrollEnd = _scrollbarSize - _scrollCursorSize;
                _displayScrollbar = true;
            }
        }

        _scrollSize = 0;
        _inertialVelocity = 0;

        IntRect box(91, posY, 30, 30);

        int i = 1;
        int j = 0;

        for (const CustomIcon& ico : input)
        {
            TouchKeyString* tks;

            if (ico.sizeX != 30 || ico.sizeY != 30) tks = new TouchKeyString(CustomIcon(Icon::DefaultCustomIcon.pixArray, Icon::DefaultCustomIcon.sizeX, Icon::DefaultCustomIcon.sizeY, ico.isEnabled), box, true);
            else tks = new TouchKeyString(ico, box, true);

            if (!mustReset)
                tks->SetPosition(0xFFFF, origPosY[j++]);

            _strKeys.push_back(tks);

            if (i == 0 && mustReset) box.leftTop.y += 36;
            box.leftTop.x = 91 + i * 36;
            if (i++ == 3) i = 0;
        }

        origPosY.clear();
        _manualScrollUpdate = true;
        _UpdateScroll(0, true);
    }

    void    KeyboardImpl::Clear(void)
    {
        _customKeyboard = false;
        _currentPosition = 0;
        for (TouchKeyString *tks : _strKeys)
            delete tks;
        _strKeys.clear();
    }

    int     KeyboardImpl::Run(void)
    {
        _isOpen = true;
        _userAbort = false;
        _askForExit = false;

        // Check if Process is paused
        if (!ProcessImpl::IsPaused)
        {
            _mustRelease  = true;
            ProcessImpl::Pause(false);
        }
        else
            _mustRelease = false;

        int                 ret = -1;
        Event               event;
        EventManager        manager(EventManager::EventGroups::GROUP_KEYS | EventManager::EventGroups::GROUP_TOUCH);
        Clock               clock;

        // Construct keyboard
        if (!_customKeyboard)
        {
            if (_layout == QWERTY) _Qwerty();
            else if (_layout == DECIMAL) _Decimal();
            else if (_layout == HEXADECIMAL) _Hexadecimal();
        }

        // Check start input
        _errorMessage = !_CheckInput();

        // Set cursor
        if (_showCursor)
        {
            _cursorPositionInString = _userInput.size();
            _ScrollUp();
        }

        // Loop until exit
        while (_isOpen)
        {
            while (manager.PollEvent(event))
            {
                _ProcessEvent(event);
                if (_userAbort)
                {
                    ret = USER_ABORT;
                    goto exit;
                }
            }

            // Update current keys
            _Update(clock.Restart().AsSeconds());

            // Render Top Screen
            if (DisplayTopScreen)
                _RenderTop();
            // Render Bottom Screen
            _RenderBottom();
            Renderer::EndFrame();

            // if it's a standard keyboard
            if (!_customKeyboard)
            {
                // Check keys
                bool inputChanged = _CheckKeys();

                if (_errorMessage && inputChanged)
                    _errorMessage = false;

                if (inputChanged)
                {
                    _errorMessage = !_CheckInput();
                    if (_onKeyboardEvent != nullptr && _owner != nullptr)
                        _onKeyboardEvent(*_owner, _KeyboardEvent);
                }

                // If user try to exit the keyboard
                if (_askForExit)
                {
                    // If input is invalid, user can't exit
                    if (_errorMessage)
                        _askForExit = false;
                    else
                    {
                        // Check input
                        _errorMessage = !_CheckInput();
                        if (!_errorMessage)
                        {
                            // input is valid, exit
                            _isOpen = false;
                            ret = 0;
                        }
                    }
                }
            }
            else
            {
                int  choice = -1;
                bool isSelected = _CheckButtons(choice);

                if (isSelected)
                {
                    ret = choice;
                    _isOpen = false;
                }
            }
            if (SystemImpl::IsSleeping()) {
                ret = SLEEP_ABORT;
                _isOpen = false;
            }
        }

    exit:
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if (menu && !menu->IsOpen() && ret != SLEEP_ABORT)
            ScreenImpl::Clean();
        if (_mustRelease)
            ProcessImpl::Play(false);
        return (ret);
    }

    void    KeyboardImpl::Close(void)
    {
        _isOpen = false;
    }

    void    KeyboardImpl::_RenderTop(void)
    {
        const Color     &red = Color::Red;
        static IntRect  background1(30, 20, 340, 200);
        static IntRect  background2(50, 30, 300, 180);

        IntRect &background = _mustRelease ? background2 : background1;
        int     maxX = background.leftTop.x + background.size.x;
        int     maxY = background.leftTop.y + background.size.y;

        int   posY =  background.leftTop.y + 5;
        int   posX =  background.leftTop.x + 5;

        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw();

        Renderer::DrawSysStringReturn(reinterpret_cast<const u8 *>(_text.c_str()), posX, posY, maxX, Preferences::Settings.MainTextColor, maxY);

        // IF error
        if (_errorMessage && !_error.empty())
        {
            if (posY < 120)
                posY += 48;
            Renderer::DrawSysStringReturn(reinterpret_cast<const u8 *>(_error.c_str()), posX, posY, maxX, red, maxY);
        }
    }

    void    KeyboardImpl::_RenderBottom(void)
    {
        static IntRect  background(20, 20, 280, 200);
        static IntRect  background2(22, 22, 276, 196);
        static IntRect  clampArea(22, 25, 270, 190);

        Renderer::SetTarget(BOTTOM);

        // Draw "normal" keyboard
        if (!_customKeyboard)
        {
            // Pointer to settings
            static auto    &theme = Preferences::Settings.Keyboard;

            int     posY = 20;
            int     posX = 25;

            // Clean background
            Renderer::DrawRect(background, theme.Background);

            // Draw input
            Renderer::DrawSysString(_userInput.c_str(), posX, posY, 300, theme.Input, _offset);

            // Draw cursor
            if (_showCursor && _blinkingClock.GetElapsedTime() < Seconds(0.5f))
                Renderer::DrawLine(_cursorPositionOnScreen + posX, 21, 1, theme.Cursor, 16);

            // Digit layout
            if (_layout != Layout::QWERTY)
            {
                // Draw keys
                for (TouchKey &key : *_keys)
                {
                    key.Draw();
                }
            }
            // Qwerty layout
            else
            {
                // Symbols
                if (_useSymbols)
                {
                    int start;
                    int end;

                    if (!_symbolsPage)
                    {
                        start = 72;
                        end = 109;
                    }
                    else
                    {
                        start = 109;
                        end = 147;
                    }

                    for (; start < end; start++)
                    {
                        (*_keys)[start].Draw();
                    }
                }
                // Nintendo
                else if (_useNintendo)
                {
                    int start;
                    int end;

                    if (!_nintendoPage)
                    {
                        start = 147;
                        end = 182;
                    }
                    else
                    {
                        start = 182;
                        end = 217;
                    }

                    for (; start < end; start++)
                    {
                        (*_keys)[start].Draw();
                    }
                }
                // Letters
                else
                {
                    // Upper Case
                    if (_useCaps)
                    {
                        /*36 - 71*/
                        for (int i = 36; i < 72; i++)
                        {
                            (*_keys)[i].Draw();
                        }
                    }
                    // Lower Case
                    else
                    {
                        /* 0 - 35*/
                        for (int i = 0; i < 36; i++)
                        {
                            (*_keys)[i].Draw();
                        }
                    }
                }
            }
        }
        // Draw custom keyboard
        else
        {
            // Pointer to settings
            static auto    &theme = Preferences::Settings.CustomKeyboard;

            Renderer::DrawRect2(background, theme.BackgroundMain, theme.BackgroundSecondary);
            Renderer::DrawRect(background2, theme.BackgroundBorder, false);

            size_t max = _strKeys.size();
            int offset = _isIconKeyboard ? 24 : 6;
            max = std::min(static_cast<int>(max), _currentPosition + offset);

            PrivColor::UseClamp(true, clampArea);

            for (size_t i = _currentPosition; i < max && i < _strKeys.size(); i++)
            {
                _strKeys[i]->ForcePressed(static_cast<int>(i) == _manualKey);
                _strKeys[i]->Draw();
            }

            PrivColor::UseClamp(false);

            if (!_displayScrollbar)
                return;

            // Draw scroll bar
            const Color &sbBackground = theme.ScrollBarBackground;
            const Color &sbThumb = theme.ScrollBarThumb;

            // Background
            int posX = 292;
            int posY = 25;

            Renderer::DrawLine(posX, posY + 1, 1, sbBackground, _scrollbarSize - 2);
            Renderer::DrawLine(posX + 1, posY, 1, sbBackground, _scrollbarSize);
            Renderer::DrawLine(posX + 2, posY + 1, 1,sbBackground, _scrollbarSize - 2);

            posY += (int)(_scrollPosition);

            // Draw thumb
            Renderer::DrawLine(posX, posY + 1, 1, sbThumb, _scrollCursorSize - 2);
            Renderer::DrawLine(posX + 1, posY, 1, sbThumb, _scrollCursorSize);
            Renderer::DrawLine(posX + 2, posY + 1, 1, sbThumb, _scrollCursorSize - 2);
        }
    }

    void    KeyboardImpl::_ProcessEvent(Event &event)
    {
        static Clock inputClock;
        bool inputPassedTime = inputClock.HasTimePassed(Milliseconds(200));
        bool keyPressIntended = false;

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Key::B)
            {
                keyPressIntended = true;
                if (_canAbort)
                {
                    _userAbort = true;
                    SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                }

                return;
            }
            if (!_customKeyboard && event.key.code == Y && !_userInput.empty())
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
                keyPressIntended  = true;
                _userInput.clear();
                _ClearKeyboardEvent();
                _KeyboardEvent.type = KeyboardEvent::EventType::InputWasCleared;
                _KeyboardEvent.codepoint = 0;
                _errorMessage = !_CheckInput();
                if (_onKeyboardEvent != nullptr && _owner != nullptr)
                    _onKeyboardEvent(*_owner, _KeyboardEvent);
            }
            if (event.key.code == X && !_customKeyboard && _layout != QWERTY && _canChangeLayout)
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
                keyPressIntended = true;
                _userInput.clear();
                _ClearKeyboardEvent();
                _KeyboardEvent.type = KeyboardEvent::EventType::InputWasCleared;
                _KeyboardEvent.codepoint = 0;
                _errorMessage = !_CheckInput();
                if (_onKeyboardEvent != nullptr && _owner != nullptr)
                    _onKeyboardEvent(*_owner, _KeyboardEvent);
                SetLayout(_layout == DECIMAL ? HEXADECIMAL : DECIMAL);
                _canChangeLayout = true;
            }
            if (_customKeyboard && (event.key.code & (Key::Down | Key::Up | Key::Left | Key::Right | Key::A))) {
                keyPressIntended = true;
                _HandleManualKeyPress((Key)(event.key.code & Key::A));
                inputPassedTime = true;
            }
        }

        if (event.type == Event::KeyDown)
        {
            if (_showCursor && inputPassedTime)
            {
                if (event.key.code == Key::DPadLeft) {
                    keyPressIntended = true;
                    inputClock.Restart();
                    _ScrollDown();
                }
                else if (event.key.code == Key::DPadRight) {
                    keyPressIntended = true;
                    inputClock.Restart();
                    _ScrollUp();
                }
            }
            if (_customKeyboard && inputPassedTime) {
                if (event.key.code & (Key::Down | Key::Up | Key::Left | Key::Right | Key::A)) {
                    keyPressIntended = true;
                    _HandleManualKeyPress((Key)(event.key.code & ~(u32)Key::A));
                    inputClock.Restart();
                }
            }
        }
        if (!keyPressIntended && _onKeyboardEvent != nullptr && _owner != nullptr && (event.type == Event::KeyDown || event.type == Event::KeyPressed || event.type == Event::KeyReleased)) {
            _ClearKeyboardEvent();
            if (event.type == Event::KeyPressed) _KeyboardEvent.type = KeyboardEvent::EventType::KeyPressed;
            else if (event.type == Event::KeyDown) _KeyboardEvent.type = KeyboardEvent::EventType::KeyDown;
            else if (event.type == Event::KeyReleased) _KeyboardEvent.type = KeyboardEvent::EventType::KeyReleased;
            _KeyboardEvent.affectedKey = event.key.code;
            _onKeyboardEvent(*_owner, _KeyboardEvent);
        }

        if (event.type == Event::TouchMoved || event.type == Event::TouchEnded)
        {
            _scrollSize = 0; _ChangeManualKey(-1, false);
        }

        if (!_displayScrollbar)
            return;

        static IntRect  buttons(60, 26, 200, 200);
        // Touch / Scroll
        if (event.type == Event::TouchBegan)
        {
            if (!buttons.Contains(event.touch.x, event.touch.y))
            {
                _inertialVelocity = 0;
                _lastTouch = IntVector(event.touch.x, event.touch.y);
                _touchTimer.Restart();
            }
        }

        if (event.type == Event::TouchMoved)
        {
            if (!buttons.Contains(event.touch.x, event.touch.y))
            {
                Time delta = _touchTimer.Restart();

                float moveDistance = (float)(_lastTouch.y - event.touch.y);
                _inertialVelocity = moveDistance / delta.AsSeconds();
                _lastTouch = IntVector(event.touch.x, event.touch.y);
            }
        }

        if (event.type == Event::TouchEnded)
        {
            if (!buttons.Contains(event.touch.x, event.touch.y))
            {
                if (_touchTimer.GetElapsedTime().AsSeconds() > 0.3f)
                    _inertialVelocity = 0.f;
            }
        }
    }

    #define INERTIA_SCROLL_FACTOR 0.9f
    #define INERTIA_ACCELERATION 0.75f
    #define INERTIA_THRESHOLD 1.0f

    void    KeyboardImpl::_UpdateScroll(float delta, bool ignoreTouch) {

        bool            isTouchDown = Touch::IsDown() && !ignoreTouch;
        IntVector       touchPos(ignoreTouch ? IntVector(0, 0) : (IntVector)Touch::GetPosition());

        if (_displayScrollbar)
        {
            if (!_manualScrollUpdate)
                _scrollSize = (_inertialVelocity * INERTIA_SCROLL_FACTOR * delta);

            _manualScrollUpdate = false;

            _scrollPosition += _scrollSize;

            if (_scrollPosition <= 0.f)
            {
                _scrollSize = _scrollSize - _scrollPosition;
                _scrollPosition = 0.f;
                _inertialVelocity = 0.f;
            }
            else if (_scrollPosition >= _scrollEnd)
            {
                _scrollSize -= (_scrollPosition - _scrollEnd);
                _scrollPosition = _scrollEnd;
                _inertialVelocity = 0.f;
            }


            _inertialVelocity += (0.98f) * delta;

            _inertialVelocity *= INERTIA_ACCELERATION;

            _currentPosition = (_scrollPosition * _scrollJump) / 36; //(_scrollPosition / 36);
            if (_isIconKeyboard) _currentPosition *= 4;

            if (std::abs(_inertialVelocity) < INERTIA_THRESHOLD)
                _inertialVelocity = 0.f;

            float scr = -_scrollSize * _scrollJump;
            _scrollSize = 0;

            for (TouchKeyString* tks : _strKeys)
            {
                tks->Scroll(scr);
                tks->Update(isTouchDown, touchPos);
            }
        }
        else
        {
            for (TouchKeyString* tks : _strKeys)
                tks->Update(isTouchDown, touchPos);
        }
    }

    void    KeyboardImpl::_Update(float delta)
    {
		bool			isTouchDown = Touch::IsDown();
		IntVector		touchPos(Touch::GetPosition());

        if (!_customKeyboard)
        {
            int start = 0;
            int end = _keys->size();

            if (_layout == Layout::QWERTY)
            {
                // Symbols
                if (_useSymbols)
                {
                    if (!_symbolsPage)
                    {
                        start = 72;
                        end = 109;
                    }
                    else
                    {
                        start = 109;
                        end = 147;
                    }
                }
                // Nintendo
                else if (_useNintendo)
                {
                    if (!_nintendoPage)
                    {
                        start = 147;
                        end = 182;
                    }
                    else
                    {
                        start = 182;
                        end = 217;
                    }
                }
                else
                {
                    // Upper Case
                    if (_useCaps)
                    {
                        start = 36;
                        end = 72;
                    }
                    // Lower Case
                    else
                    {
                        start = 0;
                        end = 36;
                    }
                }
            }

            for (int i = start; i < end; i++)
            {
                (*_keys)[i].Update(isTouchDown, touchPos);
            }

            if (_showCursor && _blinkingClock.HasTimePassed(Seconds(1.f)))
                _blinkingClock.Restart();

            _UpdateScrollInfos();
        }
        else ///< Custom Keyboard
        {
            _UpdateScroll(delta, false);
        }
    }

    /*
    ** _keys:
    **
    ** [0] = 'Q'
    ** [1] = 'W'
    ** [2] = 'E'
    ** [3] = 'R'
    ** [4] = 'T'
    ** [5] = 'Y'
    ** [6] = 'U'
    ** [7] = 'I'
    ** [8] = 'O'
    ** [9] = 'P'
    ** [10] = 'KEY_BACKSPACE'

    ** First Line : 11 keys **

    ** [11] = 'A'
    ** [12] = 'S'
    ** [13] = 'D'
    ** [14] = 'F'
    ** [15] = 'G'
    ** [16] = 'H'
    ** [17] = 'J'
    ** [18] = 'K'
    ** [19] = 'L'
    ** [20] = '''
    ** [21] = 'KEY_ENTER'

    ** Second Line : 11 keys **

     ** [22] = 'CAPS'
     ** [23] = 'Z'
     ** [24] = 'X'
     ** [25] = 'C'
     ** [26] = 'V'
     ** [27] = 'B'
     ** [28] = 'N'
     ** [29] = 'M'
     ** [30] = ','
     ** [31] = '.'
     ** [32] = '?'
     *
     ** Third Line : 11 keys **

     ** [33] = 'KEY_SYMBOL'
     ** [34] = 'SMILEY'
     ** [35] = 'SPACE'
     *
     ** Fourth Line : 3 keys **
    *************************/

    void    KeyboardImpl::_QwertyLowCase(void)
    {
        // Key rectangle
        IntRect pos(20, 36, 25, 40);

        /*low case*/
        _QwertyKeys.emplace_back('q', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('w', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('e', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('r', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('t', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('y', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('u', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('i', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('o', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('p', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 76;

        _QwertyKeys.emplace_back('a', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('s', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('d', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('f', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('g', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('h', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('j', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('k', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('l', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('\'', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        _QwertyKeys.back().SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);

        pos.leftTop.x = 20;
        pos.leftTop.y = 116;

        _QwertyKeys.emplace_back(KEY_CAPS, Icon::DrawCapsLockOn, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('z', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('x', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('c', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('v', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('b', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('n', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('m', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(',', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('.', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('?', pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 156;

        pos.size.x = 40;
        _QwertyKeys.emplace_back("+=@", pos, KEY_SYMBOLS); pos.leftTop.x += 40;
        _QwertyKeys.emplace_back("\uE008", pos, KEY_SMILEY); pos.leftTop.x += 40;
        pos.size.x = 120;
        _QwertyKeys.emplace_back("\uE057", pos, KEY_SPACE);
    }

    void    KeyboardImpl::_QwertyUpCase(void)
    {
        // Key rectangle
        IntRect pos(20, 36, 25, 40);

        /*low case*/
        _QwertyKeys.emplace_back('Q', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('W', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('E', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('R', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('T', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('Y', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('U', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('I', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('O', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('P', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 76;

        _QwertyKeys.emplace_back('A', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('S', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('D', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('F', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('G', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('H', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('J', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('K', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('L', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('"', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        _QwertyKeys.back().SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);

        pos.leftTop.x = 20;
        pos.leftTop.y = 116;

        _QwertyKeys.emplace_back(KEY_CAPS, Icon::DrawCapsLockOn, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('Z', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('X', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('C', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('V', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('B', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('N', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('M', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(';', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(':', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('!', pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 156;

        pos.size.x = 40;
        _QwertyKeys.emplace_back("+=@", pos, KEY_SYMBOLS); pos.leftTop.x += 40;
        _QwertyKeys.emplace_back("\uE008", pos, KEY_SMILEY); pos.leftTop.x += 40;
        pos.size.x = 120;
        _QwertyKeys.emplace_back("\uE057", pos, KEY_SPACE);
    }

    void KeyboardImpl::_QwertySymbols(void)
    {
        // Key rectangle
        IntRect pos(20, 36, 25, 40);

        /*page 1*/
        _QwertyKeys.emplace_back('?', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('!', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('@', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('#', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('$', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('%', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('&', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('1', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('2', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('3', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 76;

        _QwertyKeys.emplace_back('(', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(')', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('-', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('_', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('=', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00F7", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('+', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('4', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('5', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('6', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        _QwertyKeys.back().SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);

        pos.leftTop.x = 20;
        pos.leftTop.y = 116;

        _QwertyKeys.emplace_back("\u2192", pos, KEY_SYMBOLS_PAGE); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('\\', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(';', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(':', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('"', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('*', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('/', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('7', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('8', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('9', pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 156;

        pos.size.x = 40;
        _QwertyKeys.emplace_back("+=@", pos, KEY_SYMBOLS); pos.leftTop.x += 40;
        _QwertyKeys.emplace_back("\uE008", pos, KEY_SMILEY); pos.leftTop.x += 40;
        pos.size.x = 120;
        _QwertyKeys.emplace_back("\uE057", pos, KEY_SPACE); pos.leftTop.x += 120;
        pos.size.x = 25;
        _QwertyKeys.emplace_back('0', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('.', pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 36;

        /*page 2*/
        _QwertyKeys.emplace_back("\u2022", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00A9", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u20AC", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00A3", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00A5", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00B5", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00A7", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('1', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('2', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('3', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 76;

        _QwertyKeys.emplace_back("\u2122", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('<', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('>', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('[', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(']', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('{', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('}', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('4', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('5', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('6', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        _QwertyKeys.back().SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);

        pos.leftTop.x = 20;
        pos.leftTop.y = 116;

        _QwertyKeys.emplace_back("\u2190", pos, KEY_SYMBOLS_PAGE); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('|', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00B2", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('`', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00B0", pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('~', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('^', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('7', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('8', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('9', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\u00B1", pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 156;

        pos.size.x = 40;
        _QwertyKeys.emplace_back("+=@", pos, KEY_SYMBOLS); pos.leftTop.x += 40;
        _QwertyKeys.emplace_back("\uE008", pos, KEY_SMILEY); pos.leftTop.x += 40;
        pos.size.x = 120;
        _QwertyKeys.emplace_back("\uE057", pos, KEY_SPACE); pos.leftTop.x += 120;
        pos.size.x = 25;
        _QwertyKeys.emplace_back('0', pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back('.', pos);
    }

    void KeyboardImpl::_QwertyNintendo()
    {
        // Key rectangle
        IntRect pos(20, 36, 25, 40);

        /*page 1*/
        _QwertyKeys.emplace_back("\uE000" /* A */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE001" /* B */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE002" /* X */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE003" /* Y */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE004" /* L */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE005" /* R */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE054" /* ZL */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE055" /* ZR */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE006" /* DPAD */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE041" /* DPAD Wii*/, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 76;

        _QwertyKeys.emplace_back("\uE04C" /* a */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE04D" /* b */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE04E" /* x */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE04F" /* y */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE052" /* l */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE053" /* r */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE050" /* L Stick */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE051" /* R Stick */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE042" /* A Wii */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE043" /* B Wii */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        _QwertyKeys.back().SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);

        pos.leftTop.x = 20;
        pos.leftTop.y = 116;

        _QwertyKeys.emplace_back("\u2192", pos, KEY_NINTENDO_PAGE); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE040" /* Power */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE044" /* Home */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE045" /* + */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE046" /* - */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE047" /* 1 */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE048" /* 2 */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE049" /* Stick */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE04A" /* C */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE04B" /* Z */, pos); pos.leftTop.x += 25;

        pos.leftTop.x = 20;
        pos.leftTop.y = 156;

        pos.size.x = 40;
        _QwertyKeys.emplace_back("+=@", pos, KEY_SYMBOLS); pos.leftTop.x += 40;
        _QwertyKeys.emplace_back("\uE008", pos, KEY_SMILEY); pos.leftTop.x += 40;

        pos.size.x = 120;
        _QwertyKeys.emplace_back("\uE057", pos, KEY_SPACE);
        pos.size.x = 25;

        pos.leftTop.x = 20;
        pos.leftTop.y = 36;

        /*page 2*/
        _QwertyKeys.emplace_back("\uE079" /* DPAD UP */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE07B" /* DPAD DOWN */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE07C" /* DPAD LEFT */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE07D" /* DPAD RIGHT */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE07E" /* DPAD UP&DOWN */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE07F" /* DPAD LEFT&RIGHT */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE077" /* Wii Stick */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE078" /* Wii Power */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE056" /* Enter */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE057" /* Space*/, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);

        pos.leftTop.x = 20;
        pos.leftTop.y = 76;

        _QwertyKeys.emplace_back("\uE007" /* Clock */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE008" /* Happy */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE009" /* Angry */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE00A" /* Sad */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE00B" /* ExpressionLess */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE00C" /* Sun */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE00D" /* Cloud */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE00E" /* Umbrella */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE00F" /* Snowman */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE06B" /* ? */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        _QwertyKeys.back().SetAcceptSoundEvent(SoundEngine::Event::ACCEPT);

        pos.leftTop.x = 20;
        pos.leftTop.y = 116;

        _QwertyKeys.emplace_back("\u2190", pos, KEY_NINTENDO_PAGE); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE015" /*  */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE016" /*  */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE017" /*  */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE018" /*  */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE019" /* Arrow Right */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE01A" /* Arrow Left */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE01B" /* Arrow Up */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE01C" /* Arow Down */, pos); pos.leftTop.x += 25;
        _QwertyKeys.emplace_back("\uE01E" /* Camera */, pos); pos.leftTop.x += 25;

        pos.leftTop.x = 20;
        pos.leftTop.y = 156;

        pos.size.x = 40;
        _QwertyKeys.emplace_back("+=@", pos, KEY_SYMBOLS); pos.leftTop.x += 40;
        _QwertyKeys.emplace_back("\uE008", pos, KEY_SMILEY); pos.leftTop.x += 40;
        pos.size.x = 120;
        _QwertyKeys.emplace_back("\uE057", pos, KEY_SPACE);
    }


    void    KeyboardImpl::_Qwerty(void)
    {
        _keys = &_QwertyKeys;

        if (!_QwertyKeys.empty())
            return;

        /* 0 - 35*/
        _QwertyLowCase();
        /*36 - 71*/
        _QwertyUpCase();
        /* 72 - 108 Page 1*/
        /* 109 - 146 Page 2*/
        _QwertySymbols();
        /* 147 - 181 Page 1*/
        /* 182 - 216 Page 2*/
        _QwertyNintendo();
    }

    /*
    ** _keys:
    **
    ** [0] = 'A'
    ** [1] = 'B'
    ** [2] = 'C'
    ** [3] = 'D'
    ** [4] = 'E'
    ** [5] = 'F'
    ** [6] = '1'
    ** [7] = '2'
    ** [8] = '3'
    ** [9] = '4'
    ** [10] = '5'
    ** [11] = '6'
    ** [12] = '7'
    ** [13] = '8'
    ** [14] = '9'
    ** [15] = KEY_BACKSPACE
    ** [16] = KEY_ENTER
    ** [17] = '.'
    ** [18] = '0'
    *************************/

    void    KeyboardImpl::_DigitKeyboard(std::vector<TouchKey> &keys)
    {
        //Start
        IntRect pos(20, 36, 46, 46);

        // A - F
        char c = 'A';
        for (int i = 0; i < 6; i++, c++)
        {
            keys.emplace_back(c, pos);
            pos.leftTop.x += 46;

            if (i % 2)
            {
                pos.leftTop.x = 20;
                pos.leftTop.y += 46;
            }
        }

        pos.leftTop.y = 36;
        pos.leftTop.x = 112;

        // 1 - 9
        c = '1';
        for (int i = 0; i < 9; i++, c++)
        {
            keys.emplace_back(c, pos);
            pos.leftTop.x += 46;

            if (i % 3 == 2)
            {
                pos.leftTop.x = 112;
                pos.leftTop.y += 46;
            }
        }

        // Special keys
        pos.leftTop.y = 36;
        pos.leftTop.x = 250;

        // Clear key
        keys.emplace_back(KEY_BACKSPACE, Icon::DrawClearSymbol, pos);
        pos.leftTop.y += 46;

        // Enter key
        keys.emplace_back(KEY_ENTER, Icon::DrawEnterKey, pos);
        pos.leftTop.y += 46;

        // Dot key
        keys.emplace_back('.', pos);
        pos.leftTop.y += 46;

        // Plus-Minus key
        keys.emplace_back("\u00B1", pos);

        // 0 key
        pos.leftTop.y = 174;
        pos.leftTop.x = 112;
        pos.size.x = 138;
        keys.emplace_back('0', pos);
    }

    void    KeyboardImpl::_Decimal(void)
    {
        _keys = &_DecimalKeys;

        if (!_DecimalKeys.empty())
            return;

        _DigitKeyboard(_DecimalKeys);

        // Disable Hex keys
        KeyIter  iter = _DecimalKeys.begin();
        KeyIter  end = iter;
        std::advance(end, 6);

        for (; iter != end; ++iter)
            (*iter).Enable(false);
    }

    void    KeyboardImpl::_Hexadecimal(void)
    {
        _keys = &_HexaDecimalKeys;

        if (!_HexaDecimalKeys.empty())
            return;

        _DigitKeyboard(_HexaDecimalKeys);

        // Disable dot key
        _HexaDecimalKeys.at(17).Enable(false);

        // Disable pllus-minus key
        _HexaDecimalKeys.at(18).Enable(false);

        // Disable Hex keys if users asks so
        /*if (!_isHex)
        {
            KeyIter  iter = _HexaDecimalKeys.begin();
            KeyIter  end = iter;
            std::advance(end, 6);

            for (; iter != end; ++iter)
                (*iter).Enable(false);
        } */
    }

    static int UnitsToAdvance(const char *str, u32 target)
    {
        const u8 *s = reinterpret_cast<const u8 *>(str);
        int units = 0;

        // Skip UTF8 sig
        if (s[0] == 0xEF && s[1] == 0xBB && s[2] == 0xBF)
            s += 3;

        while (*s && target > 0)
        {
            if (*s == 0x18)
            {
                ++s;
                ++units;
                --target;
                continue;
            }

            if (*s == 0x1B)
            {
                s += 4;
                units += 4;
                target -= 4;
                continue;
            }

            u32 code;
            int unit = decode_utf8(&code, s);

            if (code == 0)
                break;
            if (unit == -1)
                return -1;

            s += unit;
            units += unit;
            target -= unit;
        }
        return units;
    }

    int     UnitsToNextChar(const char *cstr, int left)
    {
        int units = 0;

        while (*cstr && left > 0)
        {
            if (*cstr == 0x1B)
            {
                units += 4;
                left -= 4;
                cstr += 4;
                continue;
            }

            if (*cstr == 0x18)
            {
                ++units;
                --left;
                ++cstr;
                continue;
            }

            u32 code;
            int u = decode_utf8(&code, (u8 *)cstr);

           /* if (code == 0)
                break;*/
            if (u == -1)
                return u;

            units += u;
            break;
        }
        return units;
    }

    int     UnitsToPreviousChar(const char *cstr, int cursor)
    {
        int units = 0;

        --cstr;
        while (*cstr && cursor > 0)
        {
            --cursor;
            if (cursor > 4 && *(cstr - 3) == 0x1B)
            {
                units += 4;
                cursor -= 4;
                cstr -= 3;
                continue;
            }

            if (*cstr == 0x18)
            {
                ++units;
                --cursor;
                --cstr;
                continue;
            }

            u32 code;
            int u = decode_utf8(&code, (u8 *)cstr);

            if (u == -1 && !cursor)
                return u;

            if (u != -1)
            {
                units += u;
                break;
            }
            --cstr;
        }
        return units;
    }

    void    KeyboardImpl::_ScrollUp(void)
    {
        const u32   strLength = _userInput.size();
        const char  *cstr = _userInput.c_str();

        // If input is empty
        if (strLength == 0)
            goto empty;

        {
            // Get units to advance to go to the next char
            const int units = UnitsToNextChar(cstr + _cursorPositionInString, strLength - _cursorPositionInString);

            if (units < 0)
            {
                ///< Weird char being found, better clear the input than abort/crash later
                goto error;
            }

            // Increase cursor position
            _cursorPositionInString += units;
        }
        // cursor must be at most, after the last char of input
        if (_cursorPositionInString > static_cast<int>(strLength))
            _cursorPositionInString = strLength;

        _blinkingClock.Restart();
        return;

    error:
        _userInput.clear();
    empty:
        _offset = 0.f;
        _cursorPositionInString = _cursorPositionOnScreen = 0;
        return;

    }

    void    KeyboardImpl::_ScrollDown(void)
    {
        const u32   strLength = _userInput.size();
        const char  *cstr = _userInput.c_str();

        // If input is empty
        if (strLength == 0)
            goto empty;

        // If cursor is beyond the string, fix it (can happen when deleting a unicode)
        if (_cursorPositionInString > static_cast<int>(strLength))
            _cursorPositionInString = strLength;

        {
            // Get units to advance to go to the previous char
            const int units = UnitsToPreviousChar(cstr + _cursorPositionInString, _cursorPositionInString);

            if (units < 0)
            {
                ///< Weird char being found, better clear the input than abort/crash later
                goto error;
            }

            // Increase cursor position
            _cursorPositionInString -= units;
        }

        // cursor must be at most, at the begining of the string
        if (_cursorPositionInString < 0)
            _cursorPositionInString = 0;

        _blinkingClock.Restart();
        return;

    error:
        _userInput.clear();
    empty:
        _offset = 0.f;
        _cursorPositionInString = _cursorPositionOnScreen = 0;
        return;
    }

    void    KeyboardImpl::_UpdateScrollInfos(void)
    {
        const char *cstr = _userInput.c_str();
        const float strWidth = Renderer::GetTextSize(cstr);
        // Get units untils current cursor position
        const int unitsToCursor = UnitsToAdvance(cstr, _cursorPositionInString);

        // Weird character, might as well purge the string
        if (unitsToCursor < 0)
        {
            _offset = 0.f;
            _cursorPositionInString = _cursorPositionOnScreen = 0;
            _userInput.clear();
            return;
        }

        // Get width of both before and after cursor
        const float after = Renderer::GetTextSize(cstr + unitsToCursor);
        const float before = strWidth - after;

        // Compute offsets
        if (strWidth > 260.f)
        {
            if (before > 140.f)
                _offset = before + std::min(after, 140.f) - 260.f;
            else
                _offset = 0.f;
            _cursorPositionOnScreen = before - _offset;
        }
        else
        {
            _offset = 0;
            _cursorPositionOnScreen = before;
        }
    }

    enum
    {
        CLEAR_NOT_PRESSED = 0,
        CLEAR_JUST_PRESSED,
        CLEAR_QUICK_MODE
    };
    bool    KeyboardImpl::_CheckKeys(void)
    {
        static Clock    backspacetimer;
        static u32      backspaceFastMode = CLEAR_NOT_PRESSED;
        static Time     FastModeWaitTime = Seconds(0.5f);
        static Time     FastClearingFrame = Seconds(0.1f);

        int start = 0;
        int end = _keys->size();

        if (_layout == Layout::QWERTY)
        {
            // Symbols
            if (_useSymbols)
            {
                if (!_symbolsPage)
                {
                    start = 72;
                    end = 109;
                }
                else
                {
                    start = 109;
                    end = 147;
                }
            }
            // Nintendo
            else if (_useNintendo)
            {
                if (!_nintendoPage)
                {
                    start = 147;
                    end = 182;
                }
                else
                {
                    start = 182;
                    end = 217;
                }
            }
            else
            {
                // Upper Case
                if (_useCaps)
                {
                    start = 36;
                    end = 72;
                }
                // Lower Case
                else
                {
                    start = 0;
                    end = 36;
                }
            }
        }

        // Check cursor position, just in case
        if (_cursorPositionInString > static_cast<int>(_userInput.size()))
            _ScrollUp();

        for (int i = start; i < end; i++)
        {
            std::string  temp;

            int ret = (*_keys)[i](temp);

            if (ret != -1)
            {
                if (ret == ~KEY_BACKSPACE)
                {
                    backspaceFastMode = CLEAR_NOT_PRESSED;
                    goto _backspacePressed;
                }
                if (ret == KEY_BACKSPACE)
                {
                    if (backspaceFastMode == CLEAR_NOT_PRESSED)
                    {
                        backspaceFastMode = CLEAR_JUST_PRESSED;
                        backspacetimer.Restart();
                        return (false);
                    }
                    if (backspaceFastMode == CLEAR_JUST_PRESSED)
                    {
                        if (!backspacetimer.HasTimePassed(FastModeWaitTime))
                            return (false);
                        backspaceFastMode = CLEAR_QUICK_MODE;
                    }
                    if (backspaceFastMode == CLEAR_QUICK_MODE)
                    {
                        if (!backspacetimer.HasTimePassed(FastClearingFrame))
                            return (false);
                    }
                    backspacetimer.Restart();
                    goto _backspacePressed;
                }

                // Reset backspace state if any other key is pressed
                backspaceFastMode = CLEAR_NOT_PRESSED;

                if (ret == 0x12345678)
                {
                    if ((_layout == DECIMAL && _userInput.size() >= 18)
                        || (_layout == QWERTY && _max && Utils::GetSize(_userInput) >= _max))
                        return (false);

                    _ClearKeyboardEvent();
                    _KeyboardEvent.type = KeyboardEvent::CharacterAdded;
                    decode_utf8(&_KeyboardEvent.codepoint, (const u8 *)temp.c_str());

                    if (_KeyboardEvent.codepoint == 0x00B1) // ± key
                    {
                        if (_userInput[0] == '-')
                        {
                            _ScrollDown();
                            _userInput.erase(0, 1);
                        }
                        else
                        {
                            _userInput.insert(_userInput.begin(), '-');
                            _ScrollUp();
                        }
                    }
                    else
                    {
                        _userInput.insert(_cursorPositionInString, temp);
                        _ScrollUp();
                    }
                    return (true);
                }
                if (ret == KEY_ENTER)
                {
                    _askForExit = true;
                    return (false);
                }
                else if (ret == KEY_SPACE && (!_max || Utils::GetSize(_userInput) < _max))
                {
                    _userInput.insert(_cursorPositionInString, " ");
                    _ScrollUp();
                    return (true);
                }
                else if (ret == KEY_CAPS)
                {
                    _useCaps = !_useCaps;
                }
                else if (ret == KEY_SMILEY)
                {
                    _useNintendo = !_useNintendo;
                    _useSymbols = false;
                    _useCaps = false;
                    if (!_useNintendo)
                        _nintendoPage = 0;
                    _symbolsPage = 0;
                }
                else if (ret == KEY_SYMBOLS)
                {
                    _useSymbols = !_useSymbols;
                    _useNintendo = false;
                    _useCaps = false;
                    if (!_useSymbols)
                        _symbolsPage = 0;
                    _nintendoPage = 0;
                }
                else if (ret == KEY_SYMBOLS_PAGE)
                {
                    _symbolsPage = !_symbolsPage;
                }
                else if (ret == KEY_NINTENDO_PAGE)
                {
                    _nintendoPage = !_nintendoPage;
                }
                else
                {
                    if (_layout == DECIMAL && _userInput.length() >= 18)
                        return (false);

                    if (_layout != Layout::QWERTY &&_cursorPositionInString == 0 && ret == '.')
                    {
                        _userInput.insert(0, "0.");
                        _ScrollUp();
                        _ScrollUp(); ///< Yeah I know, I'm f*cking lazy
                    }
                    else if (_max == 0 || Utils::GetSize(_userInput) < _max)
                    {
                        temp.clear();
                        temp += ret;
                        _userInput.insert(_cursorPositionInString, temp);
                        _ClearKeyboardEvent();
                        _KeyboardEvent.type = KeyboardEvent::CharacterAdded;
                        _KeyboardEvent.codepoint = ret;
                        _ScrollUp();
                    }
                    return (true);
                }
            }
        }

        backspaceFastMode = CLEAR_NOT_PRESSED;
        return (false);

    _backspacePressed:
        std::string &&right = _userInput.substr(_cursorPositionInString);
        _userInput.erase(_cursorPositionInString);
        _ScrollDown(); ///< Scroll down before removing the char
        _ClearKeyboardEvent();
        _KeyboardEvent.codepoint = Utils::RemoveLastChar(_userInput);
        _userInput += right;

        if (_KeyboardEvent.codepoint != 0)
        {
            _KeyboardEvent.type = KeyboardEvent::CharacterRemoved;
            return (true);
        }
        return (false);
    }

    bool    KeyboardImpl::_CheckInput(void)
    {
        if (_layout == QWERTY)
        {
            if (_compare != nullptr)
                return (_compare((void *)&_userInput, _error));
            return (true);
        }

        // In case there's no convert function, always consider input as valid
        if (_convert == nullptr)
            return (true);

        // Always call convert function, can avoid overflow
        void *convertedInput = _convert(_userInput, _isHex);

        // In case there's no callback, always consider input as valid
        if (_compare == nullptr)
            return (true);

        return (_compare(convertedInput, _error));
    }

    bool    KeyboardImpl::_CheckButtons(int &ret)
    {
        bool res = false;
        for (int i = 0; i < static_cast<int>(_strKeys.size()); i++)
        {
            ret = (*_strKeys[i])();
            if (ret != -1)
            {
                ret = i;
                res = true;
                break;
            }
        }
        if (_userSelectedKey && _manualKey != -1)
        {
            ret = _manualKey; res = true; _userSelectedKey = false;
        }
        return res;
    }

    void    KeyboardImpl::_HandleManualKeyPress(Key key)
    {
        _inertialVelocity = 0;
        if (_isIconKeyboard)
        {
            if (_manualKey == -1)
            {
                if (key & (Key::Down | Key::Left))
                {
                    int tempKey = _displayScrollbar ? _currentPosition : 0;
                    if (!_strKeys[tempKey]->CanUse())
                        tempKey = 0;
                    _ChangeManualKey(tempKey);
                }
                else if (key & (Key::Up | Key::Right))
                {
                    int tempKey = _displayScrollbar ? std::min((int)_strKeys.size() - 1, _currentPosition + 20) : _strKeys.size() - 1;
                    if (!_strKeys[tempKey]->CanUse())
                        tempKey = (int)_strKeys.size() - 1;
                    _ChangeManualKey(tempKey);
                }
                else return;
            }
            if (key & (Key::Down | Key::Up | Key::Left | Key::Right))
            {
                if (key & Key::Down)
                {
                    int orig = _manualKey;
                    int tempKey = _manualKey;

                    do
                    {
                        tempKey += 4;
                    } while (tempKey < static_cast<int>(_strKeys.size()) && !_strKeys[tempKey]->CanUse() && tempKey - orig < 16);

                    if (tempKey >= static_cast<int>(_strKeys.size()) || tempKey - orig >= 16)
                        tempKey = orig;

                    _ChangeManualKey(tempKey);
                }
                else if (key & Key::Up)
                {
                    int orig = _manualKey;
                    int tempKey = _manualKey;

                    do
                    {
                        tempKey -= 4;
                    } while (tempKey > 0 && !_strKeys[tempKey]->CanUse() && orig - tempKey < 16);

                    if (tempKey < 0 || orig - tempKey >= 16)
                        tempKey = orig;

                    _ChangeManualKey(tempKey);
                }
                else if (key & Key::Right)
                {
                    int orig = _manualKey;
                    int tempKey = _manualKey;

                    do
                    {
                        tempKey++;
                    } while (tempKey < static_cast<int>(_strKeys.size()) && ((u32)tempKey & 3) != 0 && !_strKeys[tempKey]->CanUse());

                    if (tempKey >= static_cast<int>(_strKeys.size()) || ((u32)tempKey & 3) == 0)
                        tempKey = orig;

                    _ChangeManualKey(tempKey);
                }
                else if (key & Key::Left)
                {
                    int orig = _manualKey;
                    int tempKey = _manualKey;

                    do
                    {
                        tempKey--;
                    } while (tempKey > 0 && ((u32)tempKey & 3) != 3 && !_strKeys[tempKey]->CanUse());

                    if (tempKey < 0 || ((u32)tempKey & 3) == 3)
                        tempKey = orig;

                    _ChangeManualKey(tempKey);
                }

                if (_displayScrollbar)
                {
                    int keyRow = _manualKey / 4;
                    int positionRow = _currentPosition / 4;

                    if (keyRow > positionRow + 6 - 2)
                    {
                        positionRow = keyRow - 4;
                        _scrollSize = ((positionRow * 36.01f) + 15) / _scrollJump - _scrollPosition;
                        _manualScrollUpdate = true;
                        _UpdateScroll(0.f, true);
                    }
                    else if (keyRow < positionRow + 1)
                    {
                        positionRow = std::max(keyRow, 0);
                        _scrollSize = ((positionRow * 36.01f) - 15) / _scrollJump - _scrollPosition;
                        _manualScrollUpdate = true;
                        _UpdateScroll(0.f, true);
                    }
                }
            }
        }
        else
        {
            if (_manualKey == -1)
            {
                if (key & Key::Down)
                {
                    int tempKey = _displayScrollbar ? _currentPosition : 0;
                    if (!_strKeys[tempKey]->CanUse())
                        tempKey = 0;

                    _ChangeManualKey(tempKey);
                } else if (key & Key::Up)
                {
                    int tempKey = _displayScrollbar ? std::min((int)_strKeys.size() - 1, _currentPosition + 5) : _strKeys.size() - 1;
                    if (!_strKeys[tempKey]->CanUse())
                        tempKey = (int)_strKeys.size() - 1;

                    _ChangeManualKey(tempKey);
                }
                else return;
            }
            if (key & (Key::Down | Key::Up))
            {
                if (key & Key::Down)
                {
                    int orig = _manualKey;
                    int tempKey = _manualKey;

                    do {
                        tempKey++;
                    } while (tempKey < static_cast<int>(_strKeys.size()) && !_strKeys[tempKey]->CanUse() && tempKey - orig < 4);

                    if (tempKey >= static_cast<int>(_strKeys.size()) || tempKey - orig >= 4)
                        tempKey = orig;

                    _ChangeManualKey(tempKey);
                }
                else if (key & Key::Up)
                {
                    int orig = _manualKey;
                    int tempKey = _manualKey;

                    do {
                        tempKey--;
                    } while (tempKey > 0 && !_strKeys[tempKey]->CanUse() && orig - tempKey < 4);

                    if (tempKey < 0 || orig - tempKey >= 4)
                        tempKey = orig;

                    _ChangeManualKey(tempKey);
                }

                if (_displayScrollbar)
                {
                    int keyRow = _manualKey;
                    int positionRow = _currentPosition;

                    if (keyRow > positionRow + 6 - 2)
                    {
                        positionRow = keyRow - 4;
                        _scrollSize = ((positionRow * 36.01f) + 15) / _scrollJump - _scrollPosition;
                        _manualScrollUpdate = true;
                        _UpdateScroll(0.f, true);
                    }
                    else if (keyRow < positionRow + 1)
                    {
                        positionRow = std::max(keyRow, 0);
                        _scrollSize = ((positionRow * 36.01f) - 15) / _scrollJump - _scrollPosition;
                        _manualScrollUpdate = true;
                        _UpdateScroll(0.f, true);
                    }
                }
            }
        }

        if (key == A) {
            if (_manualKey != -1)
                SoundEngine::PlayMenuSound(_strKeys[_manualKey]->GetAcceptSoundEvent());
            _userSelectedKey = true;
        }
    }

    void KeyboardImpl::_ClearKeyboardEvent()
    {
        _KeyboardEvent.selectedIndex = 0;
        _KeyboardEvent.codepoint = 0;
        _KeyboardEvent.affectedKey = (Key)0;
    }

    void    KeyboardImpl::_ChangeManualKey(int newVal, bool playSound)
    {
        _manualKey = newVal;
        static bool preventRecursion = false;
        if (_manualKey != _prevManualKey && !preventRecursion && playSound)
            SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
        if (_onKeyboardEvent != nullptr && _owner != nullptr && _manualKey != _prevManualKey && !preventRecursion) {
            preventRecursion = true;
            _ClearKeyboardEvent();
            _KeyboardEvent.type = KeyboardEvent::EventType::SelectionChanged;
            _KeyboardEvent.selectedIndex = _manualKey;
            _onKeyboardEvent(*_owner, _KeyboardEvent);
            preventRecursion = false;
        }
        _prevManualKey = _manualKey;
    }

    // WIll only be used in the hex editor, so no need to do a full implementation
    bool    KeyboardImpl::operator()(int &out)
    {
        _Update(0.f);

        if (!_CheckKeys())
            return (false);

        if (_userInput.size())
        {
            out = (int)_userInput[0];
            if (out >= '0' && out <= '9')
                out -= '0';
            if (out >= 'A' && out <= 'F')
                out = 10 + out - 'A';
            _userInput.pop_back();
            return (true);
        }
        return (false);
    }

    void    KeyboardImpl::CanChangeLayout(bool canChange)
    {
        _canChangeLayout = canChange;
    }


}
