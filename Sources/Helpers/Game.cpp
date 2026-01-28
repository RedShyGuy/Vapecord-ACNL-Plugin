#include "Helpers/Game.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/Town.hpp"
#include "Address/Address.hpp"
#include "RuntimeContext.hpp"
#include "Language.hpp"
#include "TextID.hpp"

namespace CTRPluginFramework {
	namespace Game {
		ACNL_Date GetCurrentDate(void) {
			static Address getDateData(0x2FB394);
			static Address convertToDate(0x30F6DC);

			ACNL_Date date;
			convertToDate.Call<void>(&date, getDateData.Call<u32>());

			return date;
		}

		Item_Category GetItemCategory(Item itemID) {
			static Address getCategorie(0x2FCBC4);
			return getCategorie.Call<Item_Category>(&itemID);
		}

		void PlaySound(u16 soundID) {
			static Address soundfunc(0x58DE9C);
			soundfunc.Call<void>(0x1000000 + soundID);
		}

		void ResetValueDisplay(void) {
			static Address disp(0x2912B8); 
			static Address reset = disp.MoveOffset(0x28);

			Process::Write8(*(u32 *)(reset.addr) + 0xA, 0);
		}

		bool SetValueDisplay(u8 type) {
			static Address disp(0x2912B8); 
			return disp.Call<bool>(type);
		}

		u8 GetGrassStateAtWorldCoords(u8 wX, u8 wY) {	
			static Address gState(0x6C92FC); 
			return gState.Call<u8>(wX, wY);
		}

		bool WaterFlowerAtWorldCoords(u8 wX, u8 wY) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				return false;
			}
			
			static Address WFlower(0x765B14); 
			WFlower.Call<void>(PlayerClass::GetInstance()->Offset(), wX, wY);
			return true;
		}

		void ReloadRoom(float *coords) {
			static const Address u0Data(0x976C0E);

			static Address ReloadFunc(0x5B6660); 
			ReloadFunc.Call<void>(GetRoomData(), GetRoom(), coords, (u32 *)u0Data.addr, 6, 0, 0, 1, 1);
		}
		
		u32 GetRoomData() {
			static Address RoomDat(0x308154); 
			return RoomDat.Call<u32>();
		}

	//check if save screen is active 
		bool IsGameSaving() {
			static Address saving(0x126568); 
			return saving.Call<bool>();
		}
		
		/*u32 GameHelper::GetExhibition() {
			static const Address d_exhibition(0x9B4AC0);
			return *(u32 *)d_exhibition.addr;
		}

		u32 GameHelper::GetFriend() {
			static const Address d_friend(0x983038);
			return *(u32 *)d_friend.addr;
		}

		u32 GameHelper::GetDesign() {
			static const Address d_design(0x983088);
			return *(u32 *)d_design.addr;
		}

		u32 GameHelper::GetMail() {
			static const Address d_mail(0x983050);
			return *(u32 *)d_mail.addr;
		}*/

		GameMode GetGameMode() {
			static Address gtype(0x305ED8);
			return gtype.Call<GameMode>();
		}

		void ChangeGameMode(GameMode gameMode) {
			static Address gtype(0x625B88);
			gtype.Call<void>(gameMode);
		}

		void OpenMenu(u8 menuID, bool NoMenCall) {
			static Address SetupMenu(0x5C5398);
			SetupMenu.Call<void>(1);

			if(NoMenCall) {
				return;
			}
			
			static Address OpenMenu(0x6D3F8C); 
			OpenMenu.Call<void>(menuID, 0, 0);
		}

		void MoveBuilding() {
			ACNL_BuildingData *building = Building::GetSaveData();
			if(!building) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_SAVE_DATA_NOT_LOADED), Color::Red);
				return;
			}

			if(!PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
				return;
			}
			
			if(!IsGameInRoom(0)) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NOT_IN_TOWN), Color::Red);
				return;
			}
			
			if(GetOnlinePlayerCount() != 0) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NOT_IN_OWN_TOWN), Color::Red);
				return;
			}
			
			std::vector<u8> index;

			std::vector<std::string> buildingOpt;
			buildingOpt.clear();

			for(int i = 0; i < 56; i++) { 
				u8 bID = building->Buildings.Building[i].ID;
				if(bID != 0xFC) {
					index.push_back(i);
					buildingOpt.push_back(IDList::GetBuildingName(bID));
				}
			}
			
			Keyboard optKb("");
			optKb.Populate(buildingOpt);
			int val = optKb.Open();
			if(val < 0) {
				return;
			}
			
			u32 x, y;
			PlayerClass::GetInstance()->GetWorldCoords(&x, &y);
			building->Buildings.Building[index.at(val)].XCoord = x & 0xFF;
			building->Buildings.Building[index.at(val)].YCoord = y & 0xFF;

			Sleep(Milliseconds(20));
			ReloadRoom();
		}

		void RemoveBuilding() {
			ACNL_BuildingData *building = Building::GetSaveData();
			if(!building) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_SAVE_DATA_NOT_LOADED), Color::Red);
				return;
			}

			if(!PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
				return;
			}
			
			if(!IsGameInRoom(0)) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NOT_IN_TOWN), Color::Red);
				return;
			}
			
			if(GetOnlinePlayerCount() != 0) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NOT_IN_OWN_TOWN), Color::Red);
				return;
			}
			
			std::vector<u8> index;

			std::vector<std::string> buildingOpt;
			buildingOpt.clear();
			for(int i = 0; i < 56; i++) {
				u8 bID = building->Buildings.Building[i].ID;
				if(bID != 0xFC) {
					index.push_back(i);
					buildingOpt.push_back(IDList::GetBuildingName(bID));
				}
			}
			
			Keyboard optKb("");
			optKb.Populate(buildingOpt);
			int val = optKb.Open();
			if(val < 0) {
				return;
			}
			
			if(!IDList::BuildingValid(building->Buildings.Building[index.at(val)].ID)) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_CANT_REMOVE), Color::Red);
				return;
			}

			building->Buildings.Building[index.at(val)].ID = 0xFC;
			building->Buildings.Building[index.at(val)].XCoord = 0;
			building->Buildings.Building[index.at(val)].YCoord = 0;

			building->NormalPWPsAmount--;

			Sleep(Milliseconds(20));
			ReloadRoom();
		}

		bool IsBuildingSpotFree() {
			ACNL_BuildingData *building = Building::GetSaveData();
			if(!building) {
				return false;
			}

			int Bslot = 0;
			while(true) { 
				if(0xFC == building->Buildings.Building[Bslot].ID) { //If empty building slot was found
					return true;
				}
				
				Bslot++; //goto next slot
				
				if(56 < Bslot) { //no empty building slot found
					return false;
				}		
			}
		}

	//place building
		void PlaceBuilding(u8 buildingID) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
				return;
			}
			
			if(!IsGameInRoom(0)) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NOT_IN_TOWN), Color::Red);
				return;
			}
			
			if(!IDList::BuildingValid(buildingID)) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_INVALID_ID), Color::Red);
				return;
			}
			
			if(GetOnlinePlayerCount() != 0) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NOT_IN_OWN_TOWN), Color::Red);
				return;
			}
			
			if(!IsBuildingSpotFree()) {
				OSD::Notify(Language::getInstance()->get(TextID::BUILDING_MOD_NO_SLOT_FREE), Color::Red);
				return;
			}
		
			u32 x, y;	
			PlayerClass::GetInstance()->GetWorldCoords(&x, &y);
			PlaceBuildingUpdateCollisions(x, y, buildingID);

			Sleep(Milliseconds(20));

			float *coords = PlayerClass::GetInstance()->GetCoordinates();
			coords[2] += 20;

			ReloadRoom(coords);
		}

		bool IsGameInRoom(u8 room) {
			return GetRoom() == room;
		}
		
		void SetCurrentTime(bool forward, int Minutes, int Hours, int Days, int Months, int Years) {
			u64 SMinute = (Minutes * 60000000000);
			u64 SHour = (Hours * 60 * 60000000000);
			u64 SDay = (Days * 24 * 60 * 60000000000);
			u64 SMonth = (Months * 30 * 24 * 60 * 60000000000);
			u64 SYear = (Years * 365 * 24 * 60 * 60000000000);
			
			u64 Time = SMinute + SHour + SDay + SMonth + SYear;	

			static const Address RealTime(0x95D508);

		//makes time negative
			if(!forward) {
				Time = Time * (-1);
			}
			
			Town::GetSaveData()->CurrentTime += Time;
			*(u64 *)RealTime.addr += Time;
		}
		
		int DecryptValue(u64 *position) {
			static Address moneyget(0x3037DC);
			return moneyget.Call<int>(position);
		}

		void EncryptValue(u64 *position, int moneyamount) {
			static Address moneyset(0x3036A4); 
			moneyset.Call<void>(position, moneyamount);
		}

		void SetBadges(u8 badge, u8 type, bool WithStats) {
			ACNL_Player *player = Player::GetSaveData();
			if(!player) {
				return;
			}

			player->Badges.Badges[badge] = type;

			if(WithStats) {
				//type = {3 = Gold, 2 = Silver, 1 = Bronze, 0 = None}
				static const int Values[3][24] = {
					{ 36, 36, 15, 500, 500, 100, 30, 100, 
					50, 100, 1000000, 500000, 150, 100, 500, 500000, 
					50, 30, 20, 20, 50000, 50, 50, 50 },

					{ 58, 58, 24, 2000, 2000, 200, 100, 250, 
					200, 250, 10000000, 3000000, 1500, 300, 2000, 2000000, 
					100, 100, 50, 50, 100000, 200, 100, 200 },
					
					{ 72, 72, 30, 5000, 5000, 1000, 200, 500, 
					500, 500, 100000000, 10000000, 5000, 1000, 5000, 5000000, 
					200, 200, 80, 100, 150000, 500, 300, 500 }
				};

				u64 enc;
				EncryptValue(&enc, Values[type][badge]);
				player->Badges.BadgeValues[badge] = type == 0 ? 0 : enc;
			}
		}

		bool IsOutdoorItem(Item item) {
			return (item.ID <= 0xFD && item.ID >= 0);
		}

		void ToOutdoorFlowers(Item& input) {
			input.ID = (input.ID - 0x2890);
		}

		void ToIndoorFlowers(Item& input) {
			input.ID = (input.ID + 0x2890);
		}

		void Catalog(bool directcall) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				return;
			}

			if(!directcall) {
				Animation::Idle();
				OpenMenu(0, true);
			}
			
			static Address cfunction(0x6D33D8); 
			cfunction.Call<void>(0);
		}

		u32 BaseInvPointer() {
			static const Address InvMenu(0x98D500);
			return *(u32 *)InvMenu.addr; 
		}

		u32 TeleportToRoom(u8 room, bool u0, bool u1, bool u2) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				return -1;
			}

			if(GetOnlinePlayerCount() != 0) {
				return -2;
			}

			static Address roomfunc(0x304A60);	
			return roomfunc.Call<u32>(room, u0, u1, u2);
		}

		u8 GetRoom() {
			static Address GetRoom(0x2F7384);
			return GetRoom.Call<u8>();
		}

		u8 GetNextRoom() {
			static Address GetNextRoom(0x5B4C08);
			return GetNextRoom.Call<u8>();
		}

		bool IsRoomLoading() {
			static const Address LoadCheck(0x94F451);
			return *(bool *)LoadCheck.addr;
		}

		bool IsMapOpened() {
			static const Address mapBool(0x950C30);
			return *(bool *)mapBool.addr;
		}

		u8 GetOnlinePlayerIndex() {
			static Address playerIndex(0x305EF0);
			return playerIndex.Call<u8>();
		}

		u8 GetActualPlayerIndex() {
			u8 index = *(u8 *)((*(u32 *)Address(0x954648).addr) + 0x13268);
			if(index >= 4) {
				return 0;
			}
			
			return index;
		}

		u8 GetOnlinePlayerCount() {
			static Address getplayer2(0x75EFF8);
			return getplayer2.Call<u8>(*(u32 *)Address(0x954648).addr);
		}

		Item *GetItemAtWorldCoords(u32 x, u32 y) {
			static Address WorlditemCoords(0x2FEF9C);
			return WorlditemCoords.Call<Item *>(GetCurrentMap(), x, y, 0);
		}

		u32 GetCurrentMap(void) {
			static Address Currentmap(0x6A690C);
			return Currentmap.Call<u32>();
		}

		void PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID) {
			static Address PlaceBuilding(0x2425D8);
			PlaceBuilding.Call<void>(x, y, buildingID);
		}

		bool RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything, bool counting) {
			bool res = false;
			u8 x = wX, y = wY;
			u32 count = 0;
			Item Empty = {0x7FFE, 0};

			if(!PlayerClass::GetInstance()->IsLoaded()) {
				return res;
			}
			
			static Address rem1(0x597F54);
			static Address rem2(0x597F38);
			static Address rem3(0x597FAC);
			
			if(removeEverything) {
				if(!Player::IsIndoors()) {
					x = 0x10;
					y = 0x10;
				}
				else {
					x = 0;
					y = 0;
				}
			}
			res = true;
			if(trample) {
				rem1.Patch(0xE1A00000);
				rem2.Patch(0xE1A00000);
				rem3.Patch(0xE1A00000);
			}
			
			while(res && (x - wX < width || removeEverything)) {
				while(res && (y - wY < length || removeEverything)) {
					if(GetItemAtWorldCoords(x, y)) {
						if(GetItemAtWorldCoords(x, y)->ID != 0x7FFE) {
							count++;
							if(count % 300 == 0) {
								Sleep(Milliseconds(50));
							}

							if(trample) {
								TrampleAtWorldCoords(x, y);
							}
							else {
								Dropper::PlaceItemWrapper(6, ReplaceEverything, &Empty, &Empty, x, y, 0, 0, 0, 0, 0, 0x3C, 0xA5);
							}

							Controller::Update();
							if(Controller::IsKeyPressed(Key::B) && allowAbort) {
								OSD::Notify(Language::getInstance()->get(TextID::SEARCH_REPLACE_ABORT));
								goto end;
							}
						}
					}
					else {
						res = false;
					}

					y++;
				}
				res = true;
				
				if(removeEverything) {
					y = !Player::IsIndoors() ? 0x10 : 0;
				}
				else {
					y = wY;
				}

				x++;
				if(!GetItemAtWorldCoords(x, y)) {
					res = false;
				}
			}
		end:
			if(trample) {
				rem1.Unpatch();
				rem2.Unpatch();
				rem3.Unpatch();
			}
			
			if(counting) {
				OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::SEARCH_REPLACE_REPLACED).c_str(), count));
			}
			
			return true;
		}

		void SpawnParticlesAtCoords(u32 particleID, float *coords) {
			static Address particleclass(0x207B90);
			static const Address u0(0x976C0E);
			static const Address u1(0xAE6870);
			if(coords == nullptr) {
				return;
			}

			u32 pInstance = PlayerClass::GetInstance()->Offset();
			if(pInstance == 0) {
				return;
			}

			if(*(u32 *)(pInstance + 0x1B4) == 0) {
				return;
			}

			particleclass.Call<void>(particleID, coords, u0.addr, u1.addr);
		}

		void ClearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4) {
			static Address clearLocked(0x5A1278);
			clearLocked.Call<void>(wX, wY, roomID, param_4);
		}

		u32 CreateLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt) {
			static Address createLocked(0x5A13C4);
			static const Address lockspot1(0x5A13C8);
			u32 lockspot2 = lockspot1.addr + 4;
			u32 index;
			
			if(*(u32 *)lockspot1.addr != 0xE3A00000) {
				return createLocked.Call<u32>(DropID, wX, wY, roomID, sendPkt);
			}
			
			Process::Patch(lockspot1.addr, 0xE24DD01C);
			Process::Patch(lockspot2, 0xE1A07001);
			index = createLocked.Call<u32>(DropID, wX, wY, roomID, sendPkt);
			Process::Patch(lockspot1.addr, 0xE3A00000);
			Process::Patch(lockspot2, 0xE8BD83F0);
			return index;
		}

		u32 GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID) {
			static Address getlocked(0x5A11BC);
			return getlocked.Call<u32>(wX, wY, roomID);
		}

		void TrampleAtWorldCoords(u8 wX, u8 wY) {		
			Item *pItem = GetItemAtWorldCoords(wX, wY);
			Item Empty = {0x7FFE, 0};
			
			if(!pItem) {
				return;
			}
			
			u8 room = Player::GetRoom(GetActualPlayerIndex()); 
			if(GetOnlinePlayerCount() != 0) {	
				TramplePkt data { *pItem, room, wX, wY, 0 };
				
				static Address trample(0x625488);
				trample.Call<void>(0x47, 4, &data, 8);
			}
			
			static Address trample1(0x168E20); 
			trample1.Call<void>(wX, wY, 0, room, &Empty);

			static Address trample3(0x59F144);
			trample3.Call<void>(wX, wY, 1);
		}

		bool SetItem(Item *item) {		
			if(Player::GetSaveOffset(4) == 0) {
				return 0;
			}
			
			if(!item->isValid(false)) {
				return 0;
			}

			static Address writeitem(0x64FDEC);
			return writeitem.Call<bool>(PlayerClass::GetInstance()->Offset(), item);
		}
	}

	ACNL_BuildingData *Building::GetSaveData() {
		u32 *addr = (u32 *)(Address(0x2FB344).Call<u32>() + 0x4BE80);
		return (ACNL_BuildingData *)addr;
	}

	//Get instance of camera
	u32 Camera::GetInstance() {
		static const Address camera1(0x951884);
		return *(u32 *)camera1.addr;
	}
	
	float* Camera::GetCoordinates() {
		static const Address camcoord(0x9866F4);
		return (float *)camcoord.addr;
	}

//add float to x
	void Camera::AddToX(float val) {
		*(float *)(Camera::GetInstance() + 4) += val;
	}

//add float to y
	void Camera::AddToY(float val) {
		*(float *)(Camera::GetInstance() + 8) += val;
	}

//add float to z
	void Camera::AddToZ(float val) {
		*(float *)(Camera::GetInstance() + 0xC) += val;
	}
	
//add u16 to y rotation
	void Camera::AddToYRotation(u16 val) {
		*(u16 *)(Camera::GetInstance() + 0x1C) += val;
	}
}
