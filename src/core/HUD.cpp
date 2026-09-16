#include "core/HUD.hpp"
#include "core/libgarden/gardenex/MessageDisplay.hpp"
#include "core/libgarden/gardenex/layout.hpp"
#include "core/libgarden/gardenex/script/tag.hpp"
#include "core/libgarden/garden/ssys/ma/lyt/LayoutMgr.hpp"

#include <CTRPluginFramework.hpp>
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdarg>
#include <vector>

using namespace CTRPluginFramework;

namespace HUD {
    namespace {
        static constexpr u32 PERSIST_SLOTS = 8;
        static constexpr u32 PERSIST_BUF = 64;
        static constexpr u32 DRAW_BUF = 128;
        static constexpr u32 DRAW_SLOTS = 16;

        template<u32 Capacity>
        struct TextSlot {
            gardenex::TextDisplayMem<Capacity> mem;
            std::array<char16, Capacity> text{};
            nn::math::VEC2 pos{};
            nw::ut::Color8 rgbColor{}; // a!=0 = rgb-mode; a==0 = tag-mode (inline tags inside text)
            u16 textLen = 0;
            bool active = false;
            bool bottomScreen = false;
            bool dirty = true;

            void Set(std::u16string_view t, float x, float y, bool bottom, nw::ut::Color8 color = {}) {
                size_t len = t.size() < Capacity ? t.size() : Capacity - 1;
                std::char_traits<char16>::copy(text.data(), t.data(), len);
                textLen = static_cast<u16>(len);
                pos = {x, y};
                rgbColor = color;
                bottomScreen = bottom;
                active = true;
                dirty = true;
            }

            void Clear() {
                active = false;
                textLen = 0;
            }

            void Draw() {
                if (!active || textLen == 0) {
                    return;
                }

                nw::font::WideTextWriter writer;
                writer.SetDispStringBuffer(mem.GetBuffer());
                // rgb-mode: saved color; tag-mode: white (inline tags overwrite per-char)
                nw::ut::Color8 top = rgbColor.a ? rgbColor : nw::ut::Color8::White();
                writer.SetTextColors(top, {0x90, 0x90, 0x90, 0xff});

                if (dirty) {
                    gardenex::PrintTextScope printer{writer, 13.0f, 17.33333f};
                    using enum nw::font::WideTextWriter::Alignment;
                    writer.SetTextAlignment(LEFT, TOP, LEFT);
                    writer.Print(text.data(), static_cast<s32>(textLen));
                    dirty = false;
                }

                gardenex::DrawText(pos, writer, bottomScreen);
            }
        };

        using PersistSlot = TextSlot<PERSIST_BUF>;
        using DrawSlot = TextSlot<DRAW_BUF>;

        struct State {
            gardenex::MessageDisplay notify;
            PersistSlot slots[PERSIST_SLOTS];
            DrawSlot drawScratch;
            LightLock callbackLock{};
            std::vector<Callback> callbacks;
            std::vector<Callback> callbacksTrash;
        };

        alignas(State) static u8 s_Buf[sizeof(State)];
        static State* s_State = nullptr;

        std::u16string ToWide(const std::string& str) {
            std::u16string buf;
            if (str.empty()) {
                return buf;
            }

            std::vector<u16> tmp(str.size() + 1, 0);
            utf8_to_utf16(tmp.data(), reinterpret_cast<const u8*>(str.data()), str.size());
            buf.append(reinterpret_cast<const char16_t*>(tmp.data()));
            return buf;
        }

        void DrawImmediate(float x, float y, std::u16string_view text, bool bottomScreen, nw::ut::Color8 rgbColor = {}) {
            if (!s_State || text.empty()) {
                return;
            }

            auto& slot = s_State->drawScratch;
            slot.Set(text, x, y, bottomScreen, rgbColor);

            ssys::ma::lyt::LayoutMgr::Get()->DrawBegin(bottomScreen);
            slot.Draw();
        }

        void DrawCb(void*) {
            if (!s_State) {
                return;
            }

        //Transient notifications on the top screen
            ssys::ma::lyt::LayoutMgr::Get()->DrawBegin(false);
            s_State->notify.Update();
            s_State->notify.Draw();

        //Persistent text slots
            for (auto& slot : s_State->slots) {
                if (!slot.active) continue;
                ssys::ma::lyt::LayoutMgr::Get()->DrawBegin(slot.bottomScreen);
                slot.Draw();
            }

            std::vector<Callback> callbacks;
            LightLock_Lock(&s_State->callbackLock);
            if (s_State->callbacksTrash.size()) {
                s_State->callbacks.erase(std::remove_if(s_State->callbacks.begin(), s_State->callbacks.end(),
                    [](Callback cb) {
                        auto& trash = s_State->callbacksTrash;
                        return std::find(trash.begin(), trash.end(), cb) != trash.end();
                    }),
                    s_State->callbacks.end());
                s_State->callbacksTrash.clear();
            }
            callbacks = s_State->callbacks;
            LightLock_Unlock(&s_State->callbackLock);

            for (Callback cb : callbacks) {
                if (cb) {
                    cb();
                }
            }
        }

        std::u16string_view ToTag(const Color& c) {
            using namespace gardenex::script::tag;
            const bool r = c.r > 90, g = c.g > 90, b = c.b > 90;
            if (r && !g && !b) {
                return red;
            }
            if (g && !r && !b) {
                return green;
            }
            if (b && !r && !g) {
                return blue;
            }
            if (r && g && !b) {
                return orange;
            }
            if (r && !g && b) {
                return pink;
            }
            if (g && b && !r) {
                return blue;
            }
            if (!r && !g && !b) {
                return grey;
            }
            return white;
        }

        std::u16string Tagged(std::u16string_view tag, const std::string& str) {
            std::u16string buf;
            std::u16string wide = ToWide(str);
            buf.reserve(tag.size() + wide.size());
            buf.append(tag);
            buf.append(wide);
            return buf;
        }

        std::vector<std::string> SplitNotifyLines(const std::string& text) {
            static constexpr size_t MAX_UNITS_PER_LINE = 62;
            static constexpr std::string_view CONT = "...";

            auto IsAsciiWhitespace = [](unsigned char c) {
                return std::isspace(c) != 0;
            };

            struct Decoded {
                u32 codepoint;
                size_t next;
            };

            auto DecodeUtf8 = [](const std::string& s, size_t i) -> Decoded {
                if (i >= s.size()) {
                    return {0, s.size()};
                }

                const unsigned char c0 = static_cast<unsigned char>(s[i]);
                if ((c0 & 0x80) == 0x00) {
                    return {c0, i + 1};
                }

                if ((c0 & 0xE0) == 0xC0 && i + 1 < s.size()) {
                    const unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
                    if ((c1 & 0xC0) == 0x80) {
                        u32 cp = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
                        return {cp, i + 2};
                    }
                }

                if ((c0 & 0xF0) == 0xE0 && i + 2 < s.size()) {
                    const unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
                    const unsigned char c2 = static_cast<unsigned char>(s[i + 2]);
                    if ((c1 & 0xC0) == 0x80 && (c2 & 0xC0) == 0x80) {
                        u32 cp = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
                        return {cp, i + 3};
                    }
                }

                if ((c0 & 0xF8) == 0xF0 && i + 3 < s.size()) {
                    const unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
                    const unsigned char c2 = static_cast<unsigned char>(s[i + 2]);
                    const unsigned char c3 = static_cast<unsigned char>(s[i + 3]);
                    if ((c1 & 0xC0) == 0x80 && (c2 & 0xC0) == 0x80 && (c3 & 0xC0) == 0x80) {
                        u32 cp = ((c0 & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
                        return {cp, i + 4};
                    }
                }

                return {c0, i + 1};
            };

            auto IsWideCodepoint = [](u32 cp) {
                return (cp >= 0x1100 && cp <= 0x115F)
                    || (cp >= 0x2E80 && cp <= 0xA4CF)
                    || (cp >= 0xAC00 && cp <= 0xD7A3)
                    || (cp >= 0xF900 && cp <= 0xFAFF)
                    || (cp >= 0xFE10 && cp <= 0xFE6F)
                    || (cp >= 0xFF00 && cp <= 0xFF60)
                    || (cp >= 0xFFE0 && cp <= 0xFFE6);
            };

            auto UnitWidth = [&](u32 cp) {
                if (cp < 0x80) {
                    return static_cast<size_t>(1);
                }
                return static_cast<size_t>(IsWideCodepoint(cp) ? 2 : 1);
            };

            auto WrapLine = [&](const std::string& rawLine) {
                std::vector<std::string> out;
                size_t start = 0;

                while (start < rawLine.size()) {
                    const bool firstLine = out.empty();
                    const size_t prefixLen = firstLine ? 0 : CONT.size();
                    const size_t reserved = prefixLen + CONT.size();
                    const size_t bodyLimit = (MAX_UNITS_PER_LINE > reserved) ? (MAX_UNITS_PER_LINE - reserved) : 1;

                    size_t end = start;
                    size_t units = 0;
                    while (end < rawLine.size()) {
                        Decoded d = DecodeUtf8(rawLine, end);
                        const size_t w = UnitWidth(d.codepoint);
                        if (units + w > bodyLimit) {
                            break;
                        }
                        units += w;
                        end = d.next;
                    }

                    if (end == start) {
                        end = DecodeUtf8(rawLine, start).next;
                    }

                    bool reachedEnd = (end >= rawLine.size());
                    size_t cut = reachedEnd ? rawLine.size() : end;

                    if (!reachedEnd) {
                        size_t lastSpace = std::string::npos;
                        for (size_t i = start; i < end; i++) {
                            unsigned char ch = static_cast<unsigned char>(rawLine[i]);
                            if (ch < 0x80 && IsAsciiWhitespace(ch)) {
                                lastSpace = i;
                            }
                        }
                        if (lastSpace != std::string::npos && lastSpace > start) {
                            cut = lastSpace;
                        }
                    }

                    while (cut > start) {
                        unsigned char ch = static_cast<unsigned char>(rawLine[cut - 1]);
                        if (ch < 0x80 && IsAsciiWhitespace(ch)) {
                            cut--;
                        }
                        else {
                            break;
                        }
                    }

                    if (cut == start) {
                        cut = end;
                    }

                    std::string chunk = rawLine.substr(start, cut - start);
                    start = cut;

                    while (start < rawLine.size()) {
                        unsigned char ch = static_cast<unsigned char>(rawLine[start]);
                        if (ch < 0x80 && IsAsciiWhitespace(ch)) {
                            start++;
                        }
                        else {
                            break;
                        }
                    }

                    const bool hasMore = start < rawLine.size();
                    std::string line;
                    line.reserve(prefixLen + chunk.size() + (hasMore ? CONT.size() : 0));
                    if (!firstLine) {
                        line.append(CONT.data(), CONT.size());
                    }
                    line += chunk;
                    if (hasMore) {
                        line.append(CONT.data(), CONT.size());
                    }

                    if (!line.empty()) {
                        out.push_back(std::move(line));
                    }
                }

                if (out.empty()) {
                    out.push_back("");
                }

                return out;
            };

            std::vector<std::string> lines;
            size_t lineStart = 0;
            while (lineStart <= text.size()) {
                size_t lineEnd = text.find('\n', lineStart);
                if (lineEnd == std::string::npos) {
                    lineEnd = text.size();
                }

                std::string rawLine = text.substr(lineStart, lineEnd - lineStart);
                if (!rawLine.empty() && rawLine.back() == '\r') {
                    rawLine.pop_back();
                }

                auto wrapped = WrapLine(rawLine);
                lines.insert(lines.end(), wrapped.begin(), wrapped.end());

                if (lineEnd == text.size()) {
                    break;
                }
                lineStart = lineEnd + 1;
            }

            return lines;
        }

        std::u16string WithResetTag(const Text& text) {
            std::u16string buf = text.str();
            buf.append(White.tag); // reset color so following draws are not affected
            return buf;
        }

        std::u16string WithResetTag(const std::string& str, const Color& color) {
            std::u16string buf = Tagged(ToTag(color), str);
            buf.append(White.tag); // reset color so following draws are not affected
            return buf;
        }
    }

    void Init() {
        if (s_State) {
            return;
        }
        s_State = new (s_Buf) State{};
        LightLock_Init(&s_State->callbackLock);

        static Hook hook;
        hook.Initialize(Address(0x1B7354).addr, (u32)DrawCb);
        hook.SetFlags(USE_LR_TO_RETURN);
        hook.Enable();
    }

    void Notify(const std::string& str, const Color& color) {
        if (!s_State) {
            return;
        }

        for (const auto& line : SplitNotifyLines(str)) {
        //rgb to tag approximation
            auto buf = Tagged(ToTag(color), line);
            buf.append(White.tag); //we reset the color back to white so no color leaking occurs
            s_State->notify.Show(std::u16string_view{buf});
        }
    }

    void Notify(const Text& text) {
        if (!s_State) {
            return;
        }
        std::u16string buf = WithResetTag(text);
        s_State->notify.Show(std::u16string_view{buf});
    }

    void Run(Callback cb) {
        if (!s_State || cb == nullptr) {
            return;
        }

        LightLock_Lock(&s_State->callbackLock);
        auto& trash = s_State->callbacksTrash;
        trash.erase(std::remove(trash.begin(), trash.end(), cb), trash.end());

        auto& callbacks = s_State->callbacks;
        if (std::find(callbacks.begin(), callbacks.end(), cb) == callbacks.end()) {
            callbacks.push_back(cb);
        }
        LightLock_Unlock(&s_State->callbackLock);
    }

    void Stop(Callback cb) {
        if (!s_State || cb == nullptr) {
            return;
        }

        LightLock_Lock(&s_State->callbackLock);
        auto& trash = s_State->callbacksTrash;
        if (std::find(trash.begin(), trash.end(), cb) == trash.end()) {
            trash.push_back(cb);
        }
        LightLock_Unlock(&s_State->callbackLock);
    }

    void Draw(float x, float y, const std::string& str, const Color& color, bool bottomScreen) {
        if (!s_State) {
            return;
        }

        std::u16string buf = WithResetTag(str, color);
        DrawImmediate(x, y, std::u16string_view{buf}, bottomScreen);
    }

    void Draw(float x, float y, const Text& text, bool bottomScreen) {
        if (!s_State) {
            return;
        }

        std::u16string buf = WithResetTag(text);
        DrawImmediate(x, y, std::u16string_view{buf}, bottomScreen);
    }

    Handle Show(float x, float y, const std::string& str, const Color& color, bool bottomScreen) {
        if (!s_State) {
            return INVALID;
        }

        std::u16string buf = WithResetTag(str, color);
        for (u32 i = 0; i < PERSIST_SLOTS; i++) {
            if (!s_State->slots[i].active) {
                s_State->slots[i].Set(std::u16string_view{buf}, x, y, bottomScreen);
                return i;
            }
        }
        return INVALID;
    }

    Handle Show(float x, float y, const Text& text, bool bottomScreen) {
        if (!s_State) {
            return INVALID;
        }
        std::u16string buf = WithResetTag(text);
        for (u32 i = 0; i < PERSIST_SLOTS; i++) {
            if (!s_State->slots[i].active) {
            //a==0 = tag-mode, inline tags in the text
                s_State->slots[i].Set(std::u16string_view{buf}, x, y, bottomScreen);
                return i;
            }
        }
        return INVALID;
    }

    void Hide(Handle h) {
        if (!s_State || h >= PERSIST_SLOTS) {
            return;
        }
        s_State->slots[h].Clear();
    }

    void Update(Handle h, const std::string& str, const Color& color) {
        if (!s_State || h >= PERSIST_SLOTS || !s_State->slots[h].active) {
            return;
        }

        std::u16string buf = WithResetTag(str, color);
        auto& slot = s_State->slots[h];
        slot.Set(std::u16string_view{buf}, slot.pos.x, slot.pos.y, slot.bottomScreen);
    }

    void Update(Handle h, const Text& text) {
        if (!s_State || h >= PERSIST_SLOTS || !s_State->slots[h].active) {
            return;
        }

        auto& slot = s_State->slots[h];
        std::u16string buf = WithResetTag(text);
        slot.Set(std::u16string_view{buf}, slot.pos.x, slot.pos.y, slot.bottomScreen);
    }
}