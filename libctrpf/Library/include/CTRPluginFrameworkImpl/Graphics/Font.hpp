#ifndef CTRPLUGINFRAMEWORKIMPL_FONT_HPP
#define CTRPLUGINFRAMEWORKIMPL_FONT_HPP

#include "types.h"
#include "CTRPluginFramework/System/Mutex.hpp"

namespace CTRPluginFramework
{
    struct Glyph
    {
        float   xOffset; ///< Horizontal offset to draw the glyph width.
        float   xAdvance; ///< Horizontal distance to advance after drawing the glyph.
        //int     width; ///< Glyph width.
        u8      *glyph; //< Glyph data to draw

        float   Width(void) const; // Return the glyph's width
    };

    class Font
    {
    public:

        static Glyph    *GetGlyph(u8* &c);
        static Glyph    *GetGlyph(char c);
        //static float    GetStringSize(const std::string &str);
    private:
        friend void     Initialize(void);

        // Set-up the pointers array
        static void     Initialize(void);

        // Cache a new glyph
        static Glyph    *CacheGlyph(u32 glyphIndex);
        static Mutex    _mutex;
    };
}

#endif
