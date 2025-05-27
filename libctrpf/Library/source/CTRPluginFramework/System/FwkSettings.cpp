#include "CTRPluginFramework/System/FwkSettings.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"

namespace CTRPluginFramework
{
    PluginHeader *FwkSettings::Header = reinterpret_cast<PluginHeader *>(0x07000000);
    FwkSettings & FwkSettings::Get(void)
    {
        return Preferences::Settings;
    }

    void    FwkSettings::SetThemeDefault(void)
    {
        FwkSettings &settings = Get();

        // UI Colors
        settings.MenuSelectedItemColor = settings.BackgroundBorderColor = settings.WindowTitleColor = settings.MainTextColor = Color::White;
        settings.MenuUnselectedItemColor = Color::Silver;
        settings.BackgroundMainColor = Color::Black;
        settings.BackgroundSecondaryColor = Color::BlackGrey;
        settings.CursorFadeValue = 0.2f;

        // Keyboard colors
        {
            auto &kb = settings.Keyboard;

            kb.KeyBackground = kb.Background = Color::Black;
            kb.KeyBackgroundPressed = Color::Silver;
            kb.Input = kb.Cursor = kb.KeyTextPressed = kb.KeyText = Color::White;
            kb.KeyTextDisabled = Color::DimGrey;
        }

        // Custom keyboard colors
        {
            auto &kb = settings.CustomKeyboard;

            kb.BackgroundMain = Color::Black;
            kb.BackgroundSecondary = Color::BlackGrey;
            kb.BackgroundBorder = Color::White;
            kb.KeyBackground = Color(51, 51, 51);
            kb.KeyBackgroundPressed = Color::Gainsboro;
            kb.KeyText = Color::White;
            kb.KeyTextPressed = Color::Black;
            kb.ScrollBarBackground = Color::Silver;
            kb.ScrollBarThumb = Color::DimGrey;
        }
    }

    Result  FwkSettings::SetTopScreenBackground(void *bmpData)
    {
        BMPImage *image = new BMPImage(bmpData);

        if (!image->IsLoaded())
        {
            delete image;
            return -1;
        }

        Preferences::topBackgroundImage = image;
        Window::UpdateBackgrounds();
        return 0;
    }

    Result  FwkSettings::SetBottomScreenBackground(void *bmpData)
    {
        BMPImage *image = new BMPImage(bmpData);

        if (!image->IsLoaded())
        {
            delete image;
            return -1;
        }

        Preferences::bottomBackgroundImage = image;
        Window::UpdateBackgrounds();
        return 0;
    }
}
