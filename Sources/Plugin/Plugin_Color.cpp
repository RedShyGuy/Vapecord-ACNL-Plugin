#include <CTRPluginFramework.hpp>
#include "LibCtrpfExtras/MenuFolderExtras.hpp"
#include "LibCtrpfExtras/ColorExtras.hpp"
#include "LibCtrpfExtras/PluginMenuExtras.hpp"
#include "Address/Address.hpp"
#include "Helpers/Game.hpp"

namespace CTRPluginFramework {
    static const std::string loremIpsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	void ColorChangeKeyboard(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		if (input.size() > 6) {
			input.pop_back();
		}

		u32 rgb = StringToHex<u32>(input, 0xFFFFFF);
		u8 r = (rgb >> 16) & 0xFF;
		u8 g = (rgb >> 8)  & 0xFF;
		u8 b = (rgb >> 0)  & 0xFF;
		
		Color color(r, g, b);

		keyboard.GetMessage() = color << loremIpsum + ColorExtras::RemoveColor("\n\nFormat: RRGGBB");
	}

	void EditColors() {
		std::vector<std::string> folders;
		std::string foldersStr;
		for (int i = static_cast<int>(FolderType::Save); i < static_cast<int>(FolderType::Misc) + 1; ++i) {
			FolderType folder = static_cast<FolderType>(i);

			Color color = MenuFolderExtras::GetFolderColor(folder);

			folders.push_back(color << MenuFolderExtras::GetFolderName(folder));

			std::string colorStr = Utils::Format("R:%d G:%d B:%d", color.r, color.g, color.b);
			foldersStr += color << MenuFolderExtras::GetFolderName(folder) + " : " + colorStr + "\n";
		}

		Keyboard keyboard(foldersStr, folders);
		int choice = keyboard.Open();
		if (choice < 0 || choice >= (int)folders.size()) {
			return;
		}

		FolderType selectedFolder = static_cast<FolderType>(choice);
		Color currentFolderColor = MenuFolderExtras::GetFolderColor(selectedFolder);

		Keyboard setColorKB;
		setColorKB.DisplayTopScreen = true;
		setColorKB.OnKeyboardEvent(ColorChangeKeyboard);
		setColorKB.IsHexadecimal(true);

		u32 output = (currentFolderColor.r << 16) | (currentFolderColor.g << 8) | currentFolderColor.b;
		int res = setColorKB.Open(output, output);
		if (res < 0) {
			return;
		}

		u8 r = (output >> 16) & 0xFF;
		u8 g = (output >> 8)  & 0xFF;
		u8 b = (output >> 0)  & 0xFF;

		Color newColor(r, g, b);
		std::vector<ColorEntry> colors;

		if (CustomColorsExist()) {
			colors = GetCustomColors();
		}
		else {
			colors = GetDefaultColors();
		}

		for (auto& entry : colors) {
			if (entry.folderType == selectedFolder) {
				entry.r = newColor.r;
				entry.g = newColor.g;
				entry.b = newColor.b;
			}
		}
		WriteCustomColors(colors);

		PluginMenuExtras::Update();
		
        MessageBox("Color changed successfully!").SetClear(ClearScreen::Top)();
	}

    bool CustomColorsExist() {
        return File::Exists(Utils::Format(PATH_COLOR_BIN, Address::regionName.c_str()));
    }

    std::vector<ColorEntry> GetDefaultColors() {
        return defaultColors;
    }

    std::vector<ColorEntry> GetCustomColors() {
        std::vector<ColorEntry> colors(12);
        File file;
        if (File::Open(file, Utils::Format(PATH_COLOR_BIN, Address::regionName.c_str()), File::READ) != File::SUCCESS) {
            return defaultColors; // fallback to default colors
        }

        if (file.Read(colors.data(), sizeof(ColorEntry) * colors.size()) != File::SUCCESS) {
            file.Close();
            return defaultColors; // fallback to default colors
        }

        file.Close();
        return colors;
    }

    void WriteCustomColors(const std::vector<ColorEntry>& colors) {
        File file;
        if (File::Open(file, Utils::Format(PATH_COLOR_BIN, Address::regionName.c_str()), File::CREATE | File::WRITE) != File::SUCCESS) {
            return;
        }

        file.Write(colors.data(), sizeof(ColorEntry) * colors.size());
        file.Close();
    }
}