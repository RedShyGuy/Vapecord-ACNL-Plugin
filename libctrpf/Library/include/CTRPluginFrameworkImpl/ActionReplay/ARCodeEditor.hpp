#ifndef CTRPLUGINFRAMEWORKIMPL_ACTIONREPLAY_ARCODEEDITOR
#define CTRPLUGINFRAMEWORKIMPL_ACTIONREPLAY_ARCODEEDITOR

#include "types.h"
#include "CTRPluginFramework/System/Clock.hpp"
#include "CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/Converter.hpp"
#include "CTRPluginFrameworkImpl/Menu/SubMenu.hpp"
#include "ARCode.hpp"

namespace CTRPluginFramework
{
    class ARCodeEditor
    {
        using EventList = std::vector<Event>;
    public:
        struct CodeLine
        {
            enum
            {
                Empty = 1,
                Error = 1 << 2,
                Modified = 1 << 3,
                PatchData = 1 << 4,
                PatternData = 1 << 5,
                Asm = 1 << 6
            };

            explicit CodeLine(ARCode &code);
            CodeLine(const CodeLine &right);
            CodeLine(CodeLine &&right);

            ARCode&         base;
            ARCode*         parent;
            u16             flags;
            u16             index;
            std::string     display;
            std::string     comment;

            CodeLine& operator=(CodeLine &&right);
            void    Edit(u32 index, u32 value);
            void    Update(void);
        };

        ARCodeEditor(void);
        ~ARCodeEditor(void) {}

        // Return true if the user decided to close it
        bool    operator()(EventList &eventList);

        static void Edit(ARCodeContext &ctx);
    private:

        void    _ProcessEvent(Event &event);
        void    _RenderTop(void);
        void    _RenderBottom(void);
        void    _Update(void);
        void    _ReloadCodeLines(void);
        bool                        _exit;
        int                         _line;
        int                         _index;
        u16                         _cursorPosX;
        u16                         _cursorPosY;
        Clock                       _inputClock;
        ARCode                      *_clipboard;
        ARCodeContext               *_context;
        Converter                   _converter;
        SubMenu                     _submenu;
        KeyboardImpl                _keyboard;
        std::vector<CodeLine>       _codes;
    };
}

#endif
