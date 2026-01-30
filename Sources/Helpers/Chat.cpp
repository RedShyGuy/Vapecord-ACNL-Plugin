#include <CTRPluginFramework.hpp>
#include "Helpers/Chat.hpp"
#include "LibCtrpfExtras/OSDExtras.hpp"

namespace CTRPluginFramework {
	u32 Chat::GetPlayerMessageData() {
		u8 _pID = Game::GetActualPlayerIndex();
		
	    u32 PTR = *(u32 *)Address(0x94FD84).addr; //0x94FD84
		PTR += 0x464; //33078FA0
		PTR += (0x530 * _pID);
		return PTR;
	}

	std::string Chat::GetPlayerMessage() {
        std::string pMessage;
        Process::ReadString(GetPlayerMessageData() + 0x40C, pMessage, 0x64, StringFormat::Utf16);
		return pMessage;
	}

	void Chat::ClearPlayerMessage() {
		std::memset((void *)(GetPlayerMessageData() + 0x40C), 0, 0x64);
	}

	std::string Chat::GetPlayerName() {
        std::string pName;
        Process::ReadString(GetPlayerMessageData() + 0x3FA, pName, 0x16, StringFormat::Utf16);
		return pName;
	}

	u8 Chat::GetPlayerIndex() {
		return *(u8 *)(GetPlayerMessageData() + 0x3EC);
	}

	bool Chat::IsPlayerMessageOnScreen() {
		u8 Short;
		Process::Read8(GetPlayerMessageData() + 0x3F0, Short);
		return (Short != 0);
	}

	void Chat::AnimationCommand() {
		u32 x, y;
		if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), animID, {0, 0}, 0, 0, 0, 0, x, y, true);
			Sleep(Seconds(2));
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 6, {0, 0}, 0, 0, 0, 0, x, y, true);

			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::CHAT_ANIMATION).c_str(), animID)); 
		}
	}

	void Chat::EmotionCommand() {
		u32 x, y;
		if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 0xAF, {0, 0}, emotionID, 0, 0, 0, x, y, true);
			Sleep(Seconds(2));
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 6, {0, 0}, 0, 0, 0, 0, x, y, true);

			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::CHAT_EMOTION).c_str(), emotionID));
		}
	}

	void Chat::SnakeCommand() {
		u32 x, y;
		if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 0xC5, {0, 0}, 0, snakeID, 0, 0, x, y, true);
			Sleep(Seconds(2));
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 6, {0, 0}, 0, 0, 0, 0, x, y, true);

			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::CHAT_SNAKE).c_str(), snakeID)); 
		}
	}

	void Chat::MusicCommand() {
		u32 x, y;
		if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 0xC4, {0, 0}, 0, 0, musicID, 0, x, y, true);
			Sleep(Milliseconds(100));
			Animation::ExecuteAnimationWrapper(GetPlayerIndex(), 6, {0, 0}, 0, 0, 0, 0, x, y, true);

			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::CHAT_MUSIC).c_str(), musicID)); 
		}
	}

	void Chat::ItemCommand() {
		u32 x, y;
		if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {	
			Dropper::PlaceItemWrapper(0xA, ReplaceEverything, &itemID, &itemID, x, y, 0, 0, 0, 0, 0, 0x56, 0xA5, false);

			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::CHAT_ITEM).c_str(), *(u32*)&itemID));
		}	
	}

	void Chat::CommandLoop() {
		if(!IsPlayerMessageOnScreen()) {
			return;
		}

		std::string PlayerText = GetPlayerMessage();

		if(PlayerText.empty()) {
			return;
		}

		PlayerText.resize(25, ' ');

	//command
		std::string Command = PlayerText.substr(0, 2);
		UtilsExtras::ConvertToLowcase(Command);
	//special case command (flag)
		std::string SPCommand = PlayerText.substr(6, 2);
		UtilsExtras::ConvertToLowcase(SPCommand);
	//ID's
		std::string ID_8Bit = PlayerText.substr(2, 2);
		std::string ID_12Bit = PlayerText.substr(2, 3);
		std::string ID_16Bit = PlayerText.substr(2, 4);

	//special case ID (flag)
		std::string SPID_16Bit = PlayerText.substr(8, 4);

	//Item Name
		std::string ItemName = PlayerText.substr(2, 23);

		if(Command == "a:") {
			animID = StringToHex<u8>(ID_8Bit, 6); //sets animation
			if(IDList::AnimationValid(animID, GetPlayerIndex())) {
				AnimationCommand();
			}
			else {
				OSDExtras::Notify(TextID::CHAT_INVALID_ANIMATION, Color::Red);
                return;
            }
		}

		else if(Command == "e:") {
			emotionID = StringToHex<u8>(ID_8Bit, 1); //sets emotion
			if(IDList::EmotionValid(emotionID)) {
				EmotionCommand();
			}
			else {
				OSDExtras::Notify(TextID::CHAT_INVALID_EMOTION, Color::Red);
                return;
            }
		}

		else if(Command == "s:") {
			snakeID = StringToHex<u16>(ID_12Bit, 1); //sets snake
			if(IDList::SnakeValid(snakeID)) {
				SnakeCommand();
			}
			else {
				OSDExtras::Notify(TextID::CHAT_INVALID_SNAKE, Color::Red);
                return;
            }
		}

		else if(Command == "m:") {
			musicID = StringToHex<u16>(ID_12Bit, 0x660); //sets music
			if(IDList::MusicValid(musicID)) {
				MusicCommand();
			}
			else {
				OSDExtras::Notify(TextID::CHAT_INVALID_MUSIC, Color::Red);
                return;
            }
		}

		else if(Command == "i:") {
			itemID.ID = StringToHex<u16>(ID_16Bit, 0x2001); //sets item
			if(itemID.isValid()) {
				if(SPCommand == "f:") {
					itemID.Flags = StringToHex<u16>(SPID_16Bit, 0); //sets flag
				}
				ItemCommand();
			}
			else {
				OSDExtras::Notify(TextID::INVALID_ITEM, Color::Red);
				return;
			}
		}

		else if(Command == "n:") {
			UtilsExtras::Trim(ItemName);
			ItemNamePack match;
			if (!Item::searchByKeyword(ItemName, match)) {
				OSDExtras::Notify(TextID::CHAT_NO_ITEM_FOUND, Color::Red);
				return;
			}

			itemID = Item(match.ID); //sets item
			if(!itemID.isValid()) { //should always be true if orig file is used
				OSDExtras::Notify(TextID::INVALID_ITEM, Color::Red);
				return;
			}

			ItemCommand();
		}
	}

    void Chat::CommandCallback(void) {
		if (!PlayerClass::GetInstance()->IsLoaded()) {
			return;
		}

        Chat chat;
        chat.CommandLoop();
		chat.ClearPlayerMessage();
    }

    void Chat::EnableCommands(void) {
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if (!menu) {
            return;
        }
        
        *menu += CommandCallback;
    }

    void Chat::DisableCommands(void) {
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if (!menu) {
            return;
        }

        *menu -= CommandCallback;
    }
}