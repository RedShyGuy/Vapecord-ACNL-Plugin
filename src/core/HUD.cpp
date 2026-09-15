#include "core/HUD.hpp"
#include "core/libgarden/gardenex/MessageDisplay.hpp"
#include "core/libgarden/gardenex/layout.hpp"
#include "core/libgarden/gardenex/script/tag.hpp"
#include "core/libgarden/garden/ssys/ma/lyt/LayoutMgr.hpp"

#include <CTRPluginFramework.hpp>
#include <algorithm>
#include <array>
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
            buf.reserve(str.size());
            for (unsigned char ch : str) {
                buf.push_back(static_cast<char16_t>(ch));
            }
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
            buf.reserve(tag.size() + str.size());
            buf.append(tag);
            for (unsigned char ch : str) {
                buf.push_back(static_cast<char16_t>(ch));
            }
            return buf;
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
    //rgb to tag approximation
        auto buf = Tagged(ToTag(color), str);
        buf.append(White.tag); //we reset the color back to white so no color leaking occurs
        s_State->notify.Show(std::u16string_view{buf});
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