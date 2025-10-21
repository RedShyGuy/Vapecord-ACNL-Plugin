#include "Helpers/Game.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/Town.hpp"
#include "Address/Address.hpp"


namespace CTRPluginFramework {
	Keyboard optKb("");

	ACNL_Date GameHelper::GetCurrentDate(void) {
		static Address getDateData("GETDATEDATA");
		static Address convertToDate("CONVERTTODATE");

		ACNL_Date date;
		convertToDate.Call<void>(&date, getDateData.Call<u32>());

		return date;
	}

	ACNL_BuildingData *Building::GetSaveData() {
		u32 *addr = (u32 *)(Address("GARDENPLUS").Call<u32>() + 0x4BE80);
		return (ACNL_BuildingData *)addr;
	}

	Item_Categories GameHelper::GetItemCategorie(Item itemID) {
		static Address getCategorie("GETCATEGORIE");
		return getCategorie.Call<Item_Categories>(&itemID);
	}

	void GameHelper::PlaySound(u16 soundID) {
		static Address soundfunc("SOUNDFUNC");
		soundfunc.Call<void>(0x1000000 + soundID);
	}

	void GameHelper::ResetValueDisplay(void) {
	//TODO: port addresses
		static const Address reset("RESET"); 
		Process::Write8(reset.addr, 0);
	}

	bool GameHelper::SetValueDisplay(u8 type) {
	//TODO: port addresses
		static Address disp("DISP"); 
		return disp.Call<bool>(type);
	}

//returns current grass state of coordinates
	u8 GrassState(u8 wX, u8 wY) {	
		static Address gState("GSTATE"); 
		return gState.Call<u8>(wX, wY);
	}
//Water flower
	bool GameHelper::WaterFlower(u8 wX, u8 wY) {
		if(!PlayerClass::GetInstance()->IsLoaded())
			return 0;
		
		static Address WFlower("WFLOWER"); 
		WFlower.Call<void>(PlayerClass::GetInstance()->Offset(), wX, wY);
		return 1;
	}
//reload room
	void GameHelper::ReloadRoom(float *coords) {
		static const Address u0Data("U0DATA");

		static Address ReloadFunc("RELOADFUNC"); 
		ReloadFunc.Call<void>(GetRoomData(), RoomCheck(), coords, (u32 *)u0Data.addr, 6, 0, 0, 1, 1);
	}
	
//Get Room Reload Data
	u32 GameHelper::GetRoomData() {
		static Address RoomDat("ROOMDAT"); 
		return RoomDat.Call<u32>();
	}
//check if save screen is active 
	bool GameHelper::GameSaving() {
		static Address saving("SAVING"); 
		return saving.Call<bool>();
	}
	
	/*u32 GameHelper::GetExhibition() {
		static const Address d_exhibition("D_EXHIBITION");
		return *(u32 *)d_exhibition.addr;
	}

	u32 GameHelper::GetFriend() {
		static const Address d_friend("D_FRIEND");
		return *(u32 *)d_friend.addr;
	}

	u32 GameHelper::GetDesign() {
		static const Address d_design("D_DESIGN");
		return *(u32 *)d_design.addr;
	}

	u32 GameHelper::GetMail() {
		static const Address d_mail("D_MAIL");
		return *(u32 *)d_mail.addr;
	}*/
//get GameType
	u8 GameHelper::GetGameType() {
		static Address gtype("GTYPE");
		return gtype.Call<u8>();
	}
//Change GameType
	void GameHelper::ChangeGameType(u8 GameType) {
		static Address gtype("GTYPE");
		gtype.Call<void>(GameType);
	}
//call menu
	void GameHelper::OpenMenu(u8 menuID, bool NoMenCall) {
		static Address SetupMenu("SETUPMENU");
		SetupMenu.Call<void>(1);

		if(NoMenCall)
			return;
		
		static Address OpenMenu("OPENMENU"); 
		OpenMenu.Call<void>(menuID, 0, 0);
	}

//move building
	void GameHelper::MoveBuilding() {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) {
			OSD::Notify("Unknown Error: Building Save Data not loaded!", Color::Red);
			return;
		}

		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSD::Notify("Error: Player Needs To Be Loaded!", Color::Red);
			return;
		}
		
		if(!GameHelper::IsInRoom(0)) {
			OSD::Notify("Error: Only Works In Town!", Color::Red);
			return;
		}
		
		if(GameHelper::GetOnlinePlayerCount() != 0) {
			OSD::Notify("Error: Only Works In Your Own Town!", Color::Red);
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
		
		optKb.Populate(buildingOpt);
		int val = optKb.Open();
		if(val < 0) 
			return;
		
		u32 x, y;
		PlayerClass::GetInstance()->GetWorldCoords(&x, &y);
		building->Buildings.Building[index.at(val)].XCoord = x & 0xFF;
		building->Buildings.Building[index.at(val)].YCoord = y & 0xFF;

		Sleep(Milliseconds(20));
		GameHelper::ReloadRoom();
	}
//remove building
	void GameHelper::RemoveBuilding() {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) {
			OSD::Notify("Unknown Error: Building Save Data not loaded!", Color::Red);
			return;
		}

		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSD::Notify("Error: Player Needs To Be Loaded!", Color::Red);
			return;
		}
		
		if(!GameHelper::IsInRoom(0)) {
			OSD::Notify("Error: Only Works In Town!", Color::Red);
			return;
		}
		
		if(GameHelper::GetOnlinePlayerCount() != 0) {
			OSD::Notify("Error: Only Works In Your Own Town!", Color::Red);
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
		
		optKb.Populate(buildingOpt);
		int val = optKb.Open();
		if(val < 0) 
			return;
		
		if(!IDList::BuildingValid(building->Buildings.Building[index.at(val)].ID)) {
			OSD::Notify("Error: You can not remove that building!", Color::Red);
			return;
		}

		building->Buildings.Building[index.at(val)].ID = 0xFC;
		building->Buildings.Building[index.at(val)].XCoord = 0;
		building->Buildings.Building[index.at(val)].YCoord = 0;

		building->NormalPWPsAmount--;

		Sleep(Milliseconds(20));
		GameHelper::ReloadRoom();
	}
//check for free building place
	bool BuildingSpotFree() {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) 
			return 0;

		int Bslot = 0;
		while(true) { 
			if(0xFC == building->Buildings.Building[Bslot].ID) //If empty building slot was found
				return 1;
			
			Bslot++; //goto next slot
			
			if(56 < Bslot) //no empty building slot found
				return 0;			
		}
	}

//place building
	void GameHelper::PlaceBuilding(u8 buildingID) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSD::Notify("Error: Player Needs To Be Loaded!", Color::Red);
			return;
		}
		
		if(!GameHelper::IsInRoom(0)) {
			OSD::Notify("Error: Only Works In Town!", Color::Red);
			return;
		}
		
		if(!IDList::BuildingValid(buildingID)) {
			OSD::Notify("Error: Building Invalid!", Color::Red);
			return;
		}
		
		if(GameHelper::GetOnlinePlayerCount() != 0) {
			OSD::Notify("Error: Only Works In Your Own Town!", Color::Red);
			return;
		}
		
		if(!BuildingSpotFree()) {
			OSD::Notify("Error: No Building Slot free!", Color::Red);
			return;
		}
	
		u32 x, y;	
		PlayerClass::GetInstance()->GetWorldCoords(&x, &y);
		GameHelper::PlaceBuildingUpdateCollisions(x, y, buildingID);

		Sleep(Milliseconds(20));

		float *coords = PlayerClass::GetInstance()->GetCoordinates();
		coords[2] += 20;

		GameHelper::ReloadRoom(coords);
	}
//is in room
	bool GameHelper::IsInRoom(u8 room) {
		return GameHelper::RoomCheck() == room;
	} 
	
//set current time
	void GameHelper::SetCurrentTime(bool forward, int Minutes, int Hours, int Days, int Months, int Years) {
		u64 SMinute = (Minutes * 60000000000);
		u64 SHour = (Hours * 60 * 60000000000);
		u64 SDay = (Days * 24 * 60 * 60000000000);
		u64 SMonth = (Months * 30 * 24 * 60 * 60000000000);
		u64 SYear = (Years * 365 * 24 * 60 * 60000000000);
		
		u64 Time = SMinute + SHour + SDay + SMonth + SYear;	

	  //static const u32 RealTime = FollowPointer(0x300000BC, 0x2E0, -1) + 0x18;
		static const Address RealTime("REALTIME");
	//makes time negative
		if(!forward) 
			Time = Time * (-1);
		
		Town::GetSaveData()->CurrentTime += Time;
		*(u64 *)RealTime.addr += Time;
	}
	
//decrypt it
	int GameHelper::DecryptValue(u64 *position) {
		static Address moneyget("MONEYGET");
		return moneyget.Call<int>(position);
	}
//encrypt it
	void GameHelper::EncryptValue(u64 *position, int moneyamount) {
		static Address moneyset("MONEYSET"); 
		moneyset.Call<void>(position, moneyamount);
	}

//set badges	
	void GameHelper::SetBadges(u8 badge, u8 type, bool WithStats) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) 
			return;

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
			GameHelper::EncryptValue(&enc, Values[type][badge]);
			player->Badges.BadgeValues[badge] = type == 0 ? 0 : enc;
		}
	}
//get country name
	std::string GameHelper::GetCountryName() {
		static Address countryfunc("COUNTRYFUNC");
		u8 country = countryfunc.Call<u8>();
		return IDList::SetCountryName(country);
	}
//if item is outdoor only
	bool GameHelper::IsOutdoorItem(Item item) {
		return (item.ID <= 0xFD && item.ID >= 0);
	}
//converts flower to outdoor flower
	void GameHelper::ToOutdoorFlowers(Item& input) {
		input.ID = (input.ID - 0x2890);
	}
//converts flower to indoor flower
	void GameHelper::ToIndoorFlowers(Item& input) {
		input.ID = (input.ID + 0x2890);
	}

//catalog function
	void GameHelper::Catalog(bool directcall) {
		if(!PlayerClass::GetInstance()->IsLoaded()) 
			return;

		if(!directcall) {
			Animation::Idle();
			GameHelper::OpenMenu(0, true);
		}
		
		static Address cfunction("CFUNCTION"); 
		cfunction.Call<void>(0);
	}
//get base inventory pointer
	u32 GameHelper::BaseInvPointer() {
		static const Address InvMenu("INVMENU");
		return *(u32 *)InvMenu.addr; 
	}
//room function
	u32 GameHelper::RoomFunction(u8 room, bool u0, bool u1, bool u2) {
		if(!PlayerClass::GetInstance()->IsLoaded())
			return -1;

		if(GameHelper::GetOnlinePlayerCount() != 0) 
			return -2;

		static Address roomfunc("ROOMFUNC");	
		return roomfunc.Call<u32>(room, u0, u1, u2);
	}
//get room ID
	u8 GameHelper::RoomCheck() {
		static Address GetRoom("GETROOM");
		return GetRoom.Call<u8>();
	}
//get next loaded room ID
	u8 GameHelper::NextRoomCheck() {
		static Address GetNextRoom("GETNEXTROOM");
		return GetNextRoom.Call<u8>();
	}
//If loading screen is active
	bool GameHelper::LoadRoomBool() {
		static const Address LoadCheck("LOADCHECK");
		return *(bool *)LoadCheck.addr;
	}
//get map boolen pointer
	bool GameHelper::MapBoolCheck() {
		return *(bool *)Address("MAPBOOL").addr;
	}
//Get online index
	u8 GameHelper::GetOnlinePlayerIndex() {
		return Address("A_GETONLINEPLAYERINDEX").Call<u8>();
	}
//Get actual index
	u8 GameHelper::GetActualPlayerIndex() {
		u8 index = *(u8 *)((*(u32 *)Address("GAMEPOINTER").addr) + 0x13268);
		if(index >= 4) 
			return 0;
		
		return index;
	}
//Get player count
	u8 GameHelper::GetOnlinePlayerCount() {
		static Address getplayer2("GETPLAYER2");
		return getplayer2.Call<u8>(*(u32 *)Address("GAMEPOINTER").addr);
	}
//Get item at world coords
	Item *GameHelper::GetItemAtWorldCoords(u32 x, u32 y) {
		static Address WorlditemCoords("WORLDITEMCOORDS");
		return WorlditemCoords.Call<Item *>(GetCurrentMap(), x, y, 0);
	}
//Get current map
	u32 GameHelper::GetCurrentMap(void) {
		static Address Currentmap("CURRENTMAP");
		return Currentmap.Call<u32>();
	}
//Building Update Collision	
	void GameHelper::PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID) {
		static Address PlaceBuilding("PLACEBUILDING");
		PlaceBuilding.Call<void>(x, y, buildingID);
	}
//Remove items with trample
	bool GameHelper::RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything, bool counting) {
		bool res = false;
		u8 x = wX, y = wY;
		u32 count = 0;
		Item Empty = {0x7FFE, 0};

		if(!PlayerClass::GetInstance()->IsLoaded()) 
			return res;
		
		static const Address rem1("REM1");
		static const Address rem2("REM2");
		static const Address rem3("REM3");
		
		if(removeEverything) {
			if(!IsIndoorsBool) {
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
			Process::Patch(rem1.addr, 0xE1A00000);
			Process::Patch(rem2.addr, 0xE1A00000);
			Process::Patch(rem3.addr, 0xE1A00000);
		}
		
		while(res && (x - wX < width || removeEverything)) {
			while(res && (y - wY < length || removeEverything)) {
				if(GameHelper::GetItemAtWorldCoords(x, y)) {
					if(GameHelper::GetItemAtWorldCoords(x, y)->ID != 0x7FFE) {
						count++;
						if(count % 300 == 0) 
							Sleep(Milliseconds(50));

						if(trample) 
							GameHelper::TrampleAt(x, y);
						else 
							Dropper::PlaceItemWrapper(6, ReplaceEverything, &Empty, &Empty, x, y, 0, 0, 0, 0, 0, 0x3C, 0xA5);

						Controller::Update();
						if(Controller::IsKeyPressed(Key::B) && allowAbort) {
							OSD::Notify("Aborted.");
							goto end;
						}
					}
				}
				else 
					res = false;

				y++;
			}
			res = true;
			
			if(removeEverything) 
				y = !IsIndoorsBool ? 0x10 : 0;
			else 
				y = wY;

			x++;
			if(!GameHelper::GetItemAtWorldCoords(x, y)) 
				res = false;
		}
	end:
		if(trample) {
			Process::Patch(rem3.addr, 0x0A000039);
			Process::Patch(rem2.addr, 0x0A000056);
			Process::Patch(rem1.addr, 0x0A00004F);
		}
		
		if(counting)
			OSD::Notify(std::to_string(count) << " items removed!");
		
		return true;
	}
//Spawn Particles
	void GameHelper::Particles(u32 particleID, float *floats) {
		static Address particleclass("PARTICLECLASS");
		static const Address u0("U0");
		static const Address u1("U1");
		if(floats == nullptr) 
			return;

		u32 pInstance = PlayerClass::GetInstance()->Offset();
		if(pInstance == 0)
			return;

		if(*(u32 *)(pInstance + 0x1B4) == 0)
			return;

		particleclass.Call<void>(particleID, floats, u0.addr, u1.addr);
	}
//Clear a locked spot	
	void GameHelper::ClearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4) {
		static Address clearLocked("CLEARLOCKED");
		clearLocked.Call<void>(wX, wY, roomID, param_4);
	}
//Create a locked spot
	u32 GameHelper::CreateLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt) {
		static Address createLocked("CREATELOCKED");
		static const Address lockspot1("LOCKSPOT1");
		u32 lockspot2 = lockspot1.addr + 4;
		u32 index;
		
		if(*(u32 *)lockspot1.addr != 0xE3A00000) 
			return createLocked.Call<u32>(DropID, wX, wY, roomID, sendPkt);
		
		Process::Patch(lockspot1.addr, 0xE24DD01C);
		Process::Patch(lockspot2, 0xE1A07001);
		index = createLocked.Call<u32>(DropID, wX, wY, roomID, sendPkt);
		Process::Patch(lockspot1.addr, 0xE3A00000);
		Process::Patch(lockspot2, 0xE8BD83F0);
		return index;
	}
//Get index for locked spot
	u32 GameHelper::GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID) {
		static Address getlocked("GETLOCKED");
		return getlocked.Call<u32>(wX, wY, roomID);
	}
//Trample at specific position	
	void GameHelper::TrampleAt(u8 wX, u8 wY) {		
		Item *pItem = GameHelper::GetItemAtWorldCoords(wX, wY);
		Item Empty = {0x7FFE, 0};
		
		if(!pItem) 
			return;
		
		u8 room = Player::GetRoom(GameHelper::GetActualPlayerIndex()); 
		if(GameHelper::GetOnlinePlayerCount() != 0) {	
			TramplePkt data { *pItem, room, wX, wY, 0 };
			
			static Address trample("TRAMPLE");
			trample.Call<void>(0x47, 4, &data, 8);
		}
		
		static Address trample1("TRAMPLE1"); 
		trample1.Call<void>(wX, wY, 0, room, &Empty);

		static Address trample3("TRAMPLE3");
		trample3.Call<void>(wX, wY, 1);
	}

//sets first empty slot
	bool GameHelper::SetItem(Item *item) {		
		if(Player::GetSaveOffset(4) == 0) 
			return 0;
		
		if(!IDList::ItemValid(*item, false)) 
			return 0;

		static Address writeitem("WRITEITEM");
		return writeitem.Call<bool>(PlayerClass::GetInstance()->Offset(), item);
	}

//Get instance of camera
	u32 Camera::GetInstance() {
		static const Address camera1("CAMERA1");
		return *(u32 *)camera1.addr;
	}
	
	float* Camera::GetCoordinates() {
		static const Address camcoord("CAMCOORD");
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
