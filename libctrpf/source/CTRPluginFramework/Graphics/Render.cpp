#include "CTRPluginFramework/Graphics/Render.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"

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
 }
