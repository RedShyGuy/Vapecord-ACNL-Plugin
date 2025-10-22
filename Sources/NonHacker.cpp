#include "NonHacker.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Inventory.hpp"
#include "Address/Address.hpp"
#include "Helpers/ItemReader.hpp"
#include "LibCtrpfExtras/UtilsExtras.hpp"

namespace CTRPluginFramework {

	bool NonHacker::Accessible[5] = { true, true, true, true, true };

	NonHacker::NonHacker(u8 playerID) {
		pID = playerID;
		Process::ReadString(GetPlayerMessageData() + 0x40C, pMessage, 0x64, StringFormat::Utf16);
		Process::ReadString(GetPlayerMessageData() + 0x3FA, pName, 0x16, StringFormat::Utf16);
	}

	NonHacker::~NonHacker(void) {
		pID = -1;
		pMessage.clear();
		pName.clear();

		animID = 6;
		emotionID = 1;
		snakeID = 1;
		musicID = 0x660;
		itemID = { 0x7FFE, 0 };
	}

	u32 NonHacker::GetPlayerMessageData() {
		u8 _pID = pID;
		//swap your index with player 0 in order to get the correct pointer
		if(_pID == GameHelper::GetOnlinePlayerIndex()) _pID = 0;
		else if(_pID == 0) _pID = GameHelper::GetOnlinePlayerIndex();
		
	    u32 PTR = *(u32 *)Address("CHATPOINTER").addr; //0x94FD84
		PTR += 0x464; //33078FA0
		PTR += (0x530 * _pID);
		return PTR;
	}

	std::string NonHacker::GetPlayerMessage() {
		return pMessage;
	}

	void NonHacker::ClearPlayerMessage() {
		std::memset((void *)(GetPlayerMessageData() + 0x40C), 0, 0x64);
	}

	std::string NonHacker::GetPlayerName() {
		return pName;
	}

	u8 NonHacker::GetPlayerIndex() {
		//return *(u8 *)(GetPlayerMessageData() + 0x3EC);
		return pID;
	}

	bool NonHacker::IsPlayerMessageOnScreen() {
		u8 Short;
		Process::Read8(GetPlayerMessageData() + 0x3F0, Short);
		return (Short != 0);
	}

	void NonHacker::Animation() {
		if(!Accessible[0])
			return;

		u32 x, y;
		if(PlayerClass::GetInstance(pID)->GetWorldCoords(&x, &y)) {
			if(!IfForceAllowed && PlayerClass::GetInstance()->Offset() == PlayerClass::GetInstance(pID)->Offset())
				return;
		
			Animation::ExecuteAnimationWrapper(pID, animID, {0, 0}, 0, 0, 0, 0, x, y, true);
			Sleep(Seconds(2));
			Animation::ExecuteAnimationWrapper(pID, 6, {0, 0}, 0, 0, 0, 0, x, y, true);
			OSD::Notify(Utils::Format("Player: %s", pName.c_str()));
			OSD::Notify(Utils::Format("Animation: %02X", animID)); 
		}
	}

	void NonHacker::Emotion() {
		if(!Accessible[1])
			return;

		u32 x, y;
		if(PlayerClass::GetInstance(pID)->GetWorldCoords(&x, &y)) {
			if(!IfForceAllowed && PlayerClass::GetInstance()->Offset() == PlayerClass::GetInstance(pID)->Offset())
				return;
			
			Animation::ExecuteAnimationWrapper(pID, 0xAF, {0, 0}, emotionID, 0, 0, 0, x, y, true);
			Sleep(Seconds(2));
			Animation::ExecuteAnimationWrapper(pID, 6, {0, 0}, 0, 0, 0, 0, x, y, true);
			OSD::Notify(Utils::Format("Player: %s", pName.c_str()));
			OSD::Notify(Utils::Format("Emotion: %02X", emotionID));
		}
	}

	void NonHacker::Snake() {
		if(!Accessible[2])
			return;

		u32 x, y;
		if(PlayerClass::GetInstance(pID)->GetWorldCoords(&x, &y)) {
			if(!IfForceAllowed && PlayerClass::GetInstance()->Offset() == PlayerClass::GetInstance(pID)->Offset())
				return;
			
			Animation::ExecuteAnimationWrapper(pID, 0xC5, {0, 0}, 0, snakeID, 0, 0, x, y, true);
			Sleep(Seconds(2));
			Animation::ExecuteAnimationWrapper(pID, 6, {0, 0}, 0, 0, 0, 0, x, y, true);
			OSD::Notify(Utils::Format("Player: %s", pName.c_str()));
			OSD::Notify(Utils::Format("Snake: %03X", snakeID)); 
		}
	}

	void NonHacker::Music() {
		if(!Accessible[3])
			return;

		u32 x, y;
		if(PlayerClass::GetInstance(pID)->GetWorldCoords(&x, &y)) {
			if(!IfForceAllowed && PlayerClass::GetInstance()->Offset() == PlayerClass::GetInstance(pID)->Offset())
				return;
			
			Animation::ExecuteAnimationWrapper(pID, 0xC4, {0, 0}, 0, 0, musicID, 0, x, y, true);
			Sleep(Milliseconds(100));
			Animation::ExecuteAnimationWrapper(pID, 6, {0, 0}, 0, 0, 0, 0, x, y, true);
			OSD::Notify(Utils::Format("Player: %s", pName.c_str()));
			OSD::Notify(Utils::Format("Music: %03X", musicID)); 
		}
	}

	void NonHacker::Item() {
		if(!Accessible[4])
			return;

		u32 x, y;
		if(PlayerClass::GetInstance(pID)->GetWorldCoords(&x, &y)) {	
			Dropper::PlaceItemWrapper(0xA, ReplaceEverything, &itemID, &itemID, x, y, 0, 0, 0, 0, 0, 0x56, 0xA5, false);
			OSD::Notify(Utils::Format("Player: %s", pName.c_str()));
			OSD::Notify(Utils::Format("Item: %08X", *(u32*)&itemID));
		}	
	}

	bool NonHackerCommands(NonHacker *nHack) {
		if(!nHack->IsPlayerMessageOnScreen()) 
			return false;

		std::string PlayerText = nHack->GetPlayerMessage();

		if(PlayerText.empty())
			return false;

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
			nHack->animID = StringToHex<u8>(ID_8Bit, 6); //sets animation
			if(IDList::AnimationValid(nHack->animID)) {
				nHack->Animation();
			}
			else return false;
		}

		else if(Command == "e:") {
			nHack->emotionID = StringToHex<u8>(ID_8Bit, 1); //sets emotion
			if(IDList::EmotionValid(nHack->emotionID)) {
				nHack->Emotion();
			}
			else return false;
		}

		else if(Command == "s:") {
			nHack->snakeID = StringToHex<u16>(ID_12Bit, 1); //sets snake
			if(IDList::SnakeValid(nHack->snakeID)) {
				nHack->Snake();
			}
			else return false;
		}

		else if(Command == "m:") {
			nHack->musicID = StringToHex<u16>(ID_12Bit, 0x660); //sets music
			if(IDList::MusicValid(nHack->musicID)) {
				nHack->Music();
			}
			else return false;
		}

		else if(Command == "i:") {
			nHack->itemID.ID = StringToHex<u16>(ID_16Bit, 0x2001); //sets item
			if(IDList::ItemValid(nHack->itemID)) {
				if(SPCommand == "f:") 
					nHack->itemID.Flags = StringToHex<u16>(SPID_16Bit, 0); //sets flag

				nHack->Item();
			}
			else return false;
		}

		else if(Command == "n:") {
			if(!ItemReader::getInstance()->isLoaded()) 
				return false;

			UtilsExtras::Trim(ItemName);
			Item* match = ItemReader::getInstance()->searchByName(ItemName);
			if (!match) 
				return false;

			nHack->itemID = match->ID; //sets item
			if(!IDList::ItemValid(nHack->itemID)) //should always be true if orig file is used
				return false;

			nHack->Item();
		}
		else 
			return false;

		nHack->ClearPlayerMessage();
		return true;
	}
	
	void NonHacker_Player00(void) {
		NonHacker *nHack = new NonHacker(0);
		NonHackerCommands(nHack);
		delete nHack;
	}
	void NonHacker_Player01(void) {
		NonHacker *nHack = new NonHacker(1);
		NonHackerCommands(nHack);
		delete nHack;
	}
	void NonHacker_Player02(void) {
		NonHacker *nHack = new NonHacker(2);
		NonHackerCommands(nHack);
		delete nHack;
	}
	void NonHacker_Player03(void) {
		NonHacker *nHack = new NonHacker(3);
		NonHackerCommands(nHack);
		delete nHack;
	}
}