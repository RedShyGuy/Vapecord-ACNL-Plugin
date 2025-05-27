#include "CTRPluginFramework/Graphics/Render.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"

namespace CTRPluginFramework
{
    std::unordered_map<int, string16> Render::_randomSets;

    float Render::GetTextWidth(const std::string& text)
    {
        return Renderer::GetTextSize(text.c_str());
    }

    int Render::CreateRandomSet(const std::string& set)
    {
        int selectedID = 0;
        while (selectedID < 63)
        {
            if (_randomSets.find(selectedID) == _randomSets.end())
                break;
            selectedID++;
        }
        if (selectedID >= 63)
            return -1;

        Utils::ConvertUTF8ToUTF16(_randomSets[selectedID], set);

        return selectedID;
    }

    void Render::EraseRandomSet(int setID)
    {
        _randomSets.erase(setID);
    }

    std::string Render::PullRandomCharacter(int setID)
    {
        auto it = _randomSets.find(setID);
        if (it == _randomSets.end())
            return "";
        string16& choices = it->second;
        if (choices.empty())
            return "";
        u16 newStr[2];
        newStr[0] = choices[Utils::Random(0, choices.size() - 1)];
        newStr[1] = '\0';
        std::string ret;
        Utils::ConvertUTF16ToUTF8(ret, newStr);
        return ret;
    }

    void Render::Interface::DrawPixel(int posX, int posY, const Color &color) {
        Renderer::DrawPixel(posX, posY, color);
    }

    void Render::Interface::ReadPixel(int posX, int posY, Color &outColor) {
        Renderer::ReadPixel(posX, posY, outColor);
    }

    void Render::Interface::DrawHLine(int posX, int posY, int length, const Color &color, int thickness, bool stippled) {
        if (stippled)
            Renderer::DrawStippledLine(posX, posY, length, color, thickness);
        else
            Renderer::DrawLine(posX, posY, length, color, thickness);
    }

    void Render::Interface::DrawVLine(int posX, int posY, int length, const Color &color, int thickness, bool stippled) {
        if (stippled)
            Renderer::DrawStippledLine(posX, posY, thickness, color, length);
        else
            Renderer::DrawLine(posX, posY, thickness, color, length);
    }

    void Render::Interface::DrawLine(const IntVector &start, const IntVector &end, const Color &color) {
        Renderer::DrawLine(start, end, color);
    }

    void Render::Interface::DrawRect(const IntRect& rect, const Color& color, bool fill, int thickness) {
        Renderer::DrawRect(rect.leftTop.x, rect.leftTop.y, rect.size.x, rect.size.y, color, fill, thickness);
    }

    void Render::Interface::DrawSysString(const std::string& str, int posX, int posY, Color color, int xLimit, int yLimit, bool autoWrap) {
        if (autoWrap)
            Renderer::DrawSysStringReturn(reinterpret_cast<const unsigned char*>(str.c_str()), posX, posY, xLimit, color, yLimit);
        else
            Renderer::DrawSysString(str.c_str(), posX, posY, xLimit, color);
    }

    void Render::Interface::DrawCustomIcon(const CustomIcon& icon, int posX, int posY) {
        Icon::DrawCustomIcon(icon, posX, posY);
    }
 }
