#pragma once

#include <CTRPluginFramework.hpp>
#include <string>

namespace HUD {
    using Handle = u32;
    static constexpr Handle INVALID = ~0u;
    using Callback = bool(*)();

//Stream-Syntax: HUD::Red << "Hello " << HUD::Green << "World"
    struct TagColor {
        std::u16string_view tag;
    };

    inline constexpr TagColor White { std::u16string_view(u"\x0e\x00\x03\x02\x00", 5) };
    inline constexpr TagColor Pink { std::u16string_view(u"\x0e\x00\x03\x02\x01", 5) };
    inline constexpr TagColor Blue { std::u16string_view(u"\x0e\x00\x03\x02\x02", 5) };
    inline constexpr TagColor Green { std::u16string_view(u"\x0e\x00\x03\x02\x03", 5) };
    inline constexpr TagColor Orange { std::u16string_view(u"\x0e\x00\x03\x02\x04", 5) };
    inline constexpr TagColor Grey { std::u16string_view(u"\x0e\x00\x03\x02\x05", 5) };
    inline constexpr TagColor Red { std::u16string_view(u"\x0e\x00\x03\x02\x06", 5) };

    class Text {
    public:
        Text() = default;
        Text& operator<<(TagColor c) {
            m_buf.append(c.tag);
            return *this;
        }
        Text& operator<<(const char* s) {
            while (*s) {
                m_buf.push_back(static_cast<char16_t>(static_cast<unsigned char>(*s++)));
            }
            return *this;
        }
        Text& operator<<(const std::string& s) {
            for (unsigned char ch : s) {
                m_buf.push_back(static_cast<char16_t>(ch));
            }
            return *this;
        }
        Text& operator<<(const Text& other) {
            m_buf.append(other.m_buf);
            return *this;
        }
        const std::u16string& str() const {
            return m_buf;
        }

        friend inline Text operator<<(const std::string& s, TagColor c);
        friend inline Text operator<<(const std::string& s, const Text& right);
    private:
        std::u16string m_buf;
    };

    inline Text operator<<(TagColor c, const char* s) {
        Text t;
        t << c;
        t << s;
        return t;
    }

    inline Text operator<<(TagColor c, const std::string& s) {
        Text t;
        t << c;
        t << s;
        return t;
    }

    inline Text operator<<(TagColor c, TagColor c2) {
        Text t;
        t << c;
        t << c2;
        return t;
    }

    inline Text operator<<(TagColor c, const Text& right) {
        Text t;
        t << c;
        t << right;
        return t;
    }

    inline Text operator<<(const std::string& s, TagColor c) {
        Text t;
        t << s;
        t << c;
        return t;
    }

    inline Text operator<<(const std::string& s, const Text& right) {
        Text t;
        t << s;
        t << right;
        return t;
    }

    void Init();

    /*
        Usage:
        HUD::Notify("Hello World", Color::Red);
        HUD::Notify(HUD::Red << "Hello " << HUD::Green << "World");
    */
    void Notify(const std::string& str, const CTRPluginFramework::Color& color = CTRPluginFramework::Color::White);
    void Notify(const Text& text);

    /*
        OSD-style frame callback API:
        HUD::Run(MyDrawCallback);
        HUD::Stop(MyDrawCallback);
    */
    void Run(Callback cb);
    void Stop(Callback cb);

    /*
        Draw one line for the current frame only.
        Intended for use from HUD callbacks registered with HUD::Run.
    */
    void Draw(float x, float y, const std::string& str, const CTRPluginFramework::Color& color = CTRPluginFramework::Color::White, bool bottomScreen = false);
    void Draw(float x, float y, const Text& text, bool bottomScreen = false);

    /*
        Usage:
        Handle h = HUD::Show(x, y, "Hello World", HUD::Red);
        HUD::Update(h, "New Text", HUD::Green);
        HUD::Hide(h);
    */
    Handle Show(float x, float y, const std::string& str, const CTRPluginFramework::Color& color = CTRPluginFramework::Color::White, bool bottomScreen = false);
    Handle Show(float x, float y, const Text& text, bool bottomScreen = false);

    void Hide(Handle h);

    void Update(Handle h, const std::string& str, const CTRPluginFramework::Color& color = CTRPluginFramework::Color::White);
    void Update(Handle h, const Text& text);
}
