#include "features/cheats.hpp"
#include "core/game_api/Game.hpp"
#include "core/infrastructure/CROEditing.hpp"

#include "core/checks/IDChecks.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/hooks/GameLoopHook.hpp"
#include "core/ItemSequence.hpp"
#include "core/game_api/Dropper.hpp"
#include "core/game_api/Save.hpp"
#include "core/game_api/Player.hpp"
#include "Color.h"
#include "Files.h"

#include <array>

extern "C" void PATCH_KappnBypass1(void);
extern "C" void PATCH_KappnBypass2(void);

namespace CTRPluginFramework {
//For Acre And Building Mod
	struct sBuilding {
		u16 id;
		u8 x;
		u8 y;
	};

	struct Island {
		sBuilding b[2];
		u8 acres[16];
	};

	static Island isl {
		0x69, 0x1F, 0x15, //Building 1
		0x6A, 0x1D, 0x15, //Building 2
		0xAE, 0xAF, 0xAF, 0xB0, //Acre ID's
		0xAD, 0xCB, 0xB9, 0xAC, //Acre ID's
		0xAD, 0xC7, 0xC1, 0xAC, //Acre ID's
		0xAB, 0xA9, 0xA9, 0xAA  //Acre ID's
	};

	void UnlockIsland(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Both)();
			return;
		}

		std::vector<std::string> cmnOpt =  {
			Language::getInstance()->get(TextID::UNLOCK_ISLAND_ISL),
			Language::getInstance()->get(TextID::UNLOCK_ISLAND_CLUB)
		};

		bool IsON1 = player->PlayerFlags.UnlockedKappn == 1;
		bool IsON2 = player->PlayerFlags.HasClubTortimerMembership == 1;

		cmnOpt[0] = (IsON1 ? Color(pGreen) : Color(pRed)) << cmnOpt[0];
		cmnOpt[1] = (IsON2 ? Color(pGreen) : Color(pRed)) << cmnOpt[1];

		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), cmnOpt);

		int op = KB.Open();
		if(op < 0) {
			return;
		}

		else if(op == 0) {
			player->PlayerFlags.UnlockedKappn = !IsON1;
			player->PlayerFlags.KnowKappn = !IsON1;
		}

		else if(op == 1) {
			player->PlayerFlags.HasClubTortimerMembership = !IsON2;
			player->PlayerFlags.ClubTortimerFirstAsked = !IsON2;
			player->PlayerFlags.ClubTortimerRulesExplained = !IsON2;
		}

		UnlockIsland(entry);
	}

//Instant Bonus Ore | CRO Patch
	void bonusOre(MenuEntry *entry) {
		CRO::Write<u16>("Tour", 0x424, 0x4600); //Disables check if Item is Ore
		CRO::Write<u16>("Tour", 0x444, 0x4600); //Disables check if Item is secret ore

		if(!entry->IsActivated()) {
			CRO::Write<u16>("Tour", 0x424, 0xD124);
        	CRO::Write<u16>("Tour", 0x444, 0xD001);
		}
	}
//Instant Fruit | CRO Patch
	void instantFruit(MenuEntry *entry) {
		CRO::Write<u32>("Kotobuki", 0x17F4, 0xE1500000);

		if(!entry->IsActivated()) {
			CRO::Write<u32>("Kotobuki", 0x17F4, 0xE1500005);
		}
	}

//Defined u32 items for Island Shop Slot Mod
	Item ShopItem[4] = { {0x2018, 0}, {0x2018, 0}, {0x2018, 0}, {0x2018, 0} };
//Keyboard for Island Shop Slot Mod
	void IslandSettings(MenuEntry *entry) {
		Keyboard SetItem("a");
		SetItem.DisplayTopScreen = true;
		SetItem.OnKeyboardEvent(ItemChange);

		for(int i = 0; i < 4; ++i) {
			SetItem.GetMessage() = Utils::Format(Language::getInstance()->get(TextID::ISLAND_SHOP_MOD_ENTER_ID).c_str(), i + 1);

			int res = SetItem.Open(*(u32 *)&ShopItem[i]);
			if(res < 0) {
				break;
			}
		}
	}
//Island Shop
	void IslandShop(MenuEntry *entry) {
		static const Address IslandShopPointer(0x954238);
		if(*(u32 *)IslandShopPointer.addr == 0) {
			return;
		}

		if(Game::GetNextRoom() == 0xA5 && Game::IsGameInRoom(0x65)) {
			for(int i = 0; i < 4; ++i) {
				Process::Write32(*(u32 *)IslandShopPointer.addr + 0x10 + (i * 4), ShopItem[i].isValid(false) ? *(u32 *)&ShopItem[i] : 0x2018);
			}
		}
	}

//All Tours
	void alltour(MenuEntry *entry) {
		static const Address TourPatch(0x76FCC0);
		if(entry->WasJustActivated()) {
			Process::Patch(TourPatch.addr, 0xE1A00000); //unsure? (still keeping it for safety)
			Process::Patch(TourPatch.addr + 0x54, 0xE1A00000);  //Adds tour difficulty
			Process::Patch(TourPatch.addr + 0xD0, 0xE1A00000); //Adds tour names and tour infos
			Process::Patch(TourPatch.addr + 0x138, 0xE1A00000); //Adds tour time

			Process::Patch(TourPatch.addr + 0xA8, 0xE2800001); //Adds all tours to be selectable
		}
		else if(!entry->IsActivated()) {
			Process::Patch(TourPatch.addr, 0x0A000004);
			Process::Patch(TourPatch.addr + 0x54, 0x0A000004);
			Process::Patch(TourPatch.addr + 0xD0, 0x0A000004);
			Process::Patch(TourPatch.addr + 0x138, 0x0A000004);

			Process::Patch(TourPatch.addr + 0xA8, 0x12800001);
		}
    }

//Island Acre Mod
	void acreMod(MenuEntry *entry) {
		if(*(u32 *)Address(0x953708).addr == 0) {
			return;
		}

		u32 IslAcreOffset = *(u32 *)Address(0x953708).addr + 2; //0x953708

		for(u8 i = 0; i < 16; ++i) {
			Process::Write8(IslAcreOffset + i * 2, isl.acres[i]);
		}
	}
//Island Acre Mod Keyboard
	void menuAcreMod(MenuEntry *entry) {
		Keyboard kb(Language::getInstance()->get(TextID::ENTER_ID));
		kb.SetMaxLength(2);
		kb.IsHexadecimal(true);

		for(u8 i = 0; i < 16; ++i) {
			kb.GetMessage() = Utils::Format(Language::getInstance()->get(TextID::ISLAND_ACRE_ENTER_ID).c_str(), i + 1);

			kb.Open(isl.acres[i], isl.acres[i]);
		}
	}
//Island Building Mod
	void buildingMod(MenuEntry *entry) {
		if(*(u32 *)Address(0x953708).addr == 0) {
			return;
		}

		u32 islandBuildings = *(u32 *)Address(0x953708).addr + 0x1022;

		for(u8 i = 0; i < 2; ++i) {
			Process::Write16(islandBuildings + i * 4, isl.b[i].id);
			Process::Write8(islandBuildings + 2 + i * 4, isl.b[i].x);
			Process::Write8(islandBuildings + 3 + i * 4, isl.b[i].y);
		}
	}
//Island Building Mod Keyboard
	void menuBuildingMod(MenuEntry *entry) {
		Keyboard kb(Language::getInstance()->get(TextID::ENTER_ID));
		kb.SetMaxLength(2);
		kb.IsHexadecimal(true);

		for(u8 i = 0; i < 2; ++i) {
			kb.GetMessage() = Language::getInstance()->get(TextID::ISLAND_BUILDING_ENTER_ID) << Utils::Format(" %d", i + 1);
			kb.Open(isl.b[i].id, isl.b[i].id);

			kb.GetMessage() = Language::getInstance()->get(TextID::ISLAND_BUILDING_ENTER_X) << Utils::Format(" %d", i + 1);
			kb.Open(isl.b[i].x, isl.b[i].x);

			kb.GetMessage() = Language::getInstance()->get(TextID::ISLAND_BUILDING_ENTER_Y) << Utils::Format(" %d", i + 1);
			kb.Open(isl.b[i].y, isl.b[i].y);
		}
	}

	void FreeKappn(MenuEntry *entry) {
		static Hook hook1, hook2;
		static const Address kappn1(0x5DC048);
		static const Address kappn2(0x5DAF98);

		if(entry->WasJustActivated()) {
			hook1.Initialize(kappn1.addr, (u32)PATCH_KappnBypass1);
			hook2.Initialize(kappn2.addr, (u32)PATCH_KappnBypass2);

		  	hook1.SetFlags(USE_LR_TO_RETURN);
			hook2.SetFlags(USE_LR_TO_RETURN);

			hook1.Enable();
			hook2.Enable();
		}

		else if(!entry->IsActivated()) {
			hook1.Disable();
			hook2.Disable();
		}
	}

	static constexpr u32 IslandGridStart = 0x10;
	static constexpr u32 IslandGridEnd = 0x2F;
	static constexpr size_t IslandGridWidth = IslandGridEnd - IslandGridStart + 1;
	static constexpr size_t IslandTileCount = IslandGridWidth * IslandGridWidth;

	void RestoreIsland(const std::vector<Item> &fileData) {
		bool itemSequenceWasOn = false;

		if(ItemSequence::Enabled()) {
			itemSequenceWasOn = true;
			ItemSequence::Switch(false);
		}

		const bool changedDropLock = !bypassing;
		if(changedDropLock) {
			Dropper::DropItemLock(true);
		}

		auto restoreState = [&]() {
			if(changedDropLock) {
				Dropper::DropItemLock(false);
			}

			if(itemSequenceWasOn) {
				ItemSequence::Switch(true);
			}
		};

		if(fileData.size() != IslandTileCount) {
			restoreState();
			OSD::NotifySysFont("Invalid island backup.", Color::Red);
			return;
		}

		u32 count = 0;
		size_t tileIndex = 0;

		for(u32 x = IslandGridStart; x <= IslandGridEnd; ++x) {
			for(u32 y = IslandGridStart; y <= IslandGridEnd; ++y, ++tileIndex) {
				Item *currentItem = Game::GetItemAtWorldCoords(x, y);
				if(currentItem == nullptr) {
					continue;
				}

				if (!currentItem->isValid()) {
					continue;
				}

				Item desiredItem = fileData[tileIndex];
				if(*currentItem != desiredItem) {
					if(Dropper::PlaceItemWrapper(1, ReplaceEverything, &desiredItem, &desiredItem, x, y, 0, 0, 0, 0, 0, 0x3D, 0xA5, false)) {
						count++;
						if(count % 300 == 0) {
							Sleep(Milliseconds(500));
						}
					}
				}
			}
		}

		OSD::NotifySysFont(Utils::Format(Language::getInstance()->get(TextID::ISLAND_RESTORE_PLACED_COUNT).c_str(), count));
		restoreState();
	}

	void IslandSaver(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		if(!Game::IsGameInRoom(0x68)) {
			MessageBox(Language::getInstance()->get(TextID::ISLAND_SAVER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		const std::vector<std::string> options = {
			Language::getInstance()->get(TextID::ISLAND_SAVER_BACKUP_ISLAND),
			Language::getInstance()->get(TextID::ISLAND_SAVER_RESTORE_ISLAND),
			Language::getInstance()->get(TextID::FILE_DELETE)
		};

		Keyboard KB(Language::getInstance()->get(TextID::ISLAND_SAVER_DUMPER_DUMP), options);
		int index = KB.Open();
		switch(index) {
			default: break;
			case 0: {
				std::vector<u32> dumpVec;
				dumpVec.reserve(IslandTileCount);

				for (u32 x = IslandGridStart; x <= IslandGridEnd; ++x) {
					for (u32 y = IslandGridStart; y <= IslandGridEnd; ++y) {
						Item* atCoords = Game::GetItemAtWorldCoords(x, y);
						dumpVec.push_back(atCoords ? static_cast<u32>(*atCoords) : 0);
					}
				}

				MemoryRange backupLoc = MemoryRange{ dumpVec.data(), static_cast<int>(dumpVec.size() * sizeof(u32)) };

				std::string filename = "";
				Keyboard KB(Language::getInstance()->get(TextID::ISLAND_SAVER_NAME_BACKUP));

				if(KB.Open(filename) == -1) {
					return;
				}

				PluginUtils::Backup::DumpMemory(
					Utils::Format(PATH_ISLAND, Address::regionName.c_str()),
					filename,
					".dat",
					{ backupLoc }
				);
			} break;

			case 1: {
				u32 fileData[IslandTileCount];
				std::vector<Item> IslandItems;
				IslandItems.reserve(IslandTileCount);
				MemoryRange restoreLoc = MemoryRange{ fileData, static_cast<int>(sizeof(fileData)) };

				if (PluginUtils::Backup::RestoreMemory(
						Utils::Format(PATH_ISLAND, Address::regionName.c_str()),
						".dat",
						Language::getInstance()->get(TextID::SAVE_RESTORE_SELECT),
						{ restoreLoc },
						PluginUtils::Backup::RestoreOptions{ nullptr, false }
					) == OperationResult::Success) {
					for(size_t i = 0; i < IslandTileCount; ++i) {
						IslandItems.push_back({ static_cast<u16>(fileData[i] & 0xFFFF), static_cast<u16>(fileData[i] >> 16) });
					}
					RestoreIsland(IslandItems);
				}
			} break;
			case 2: {
				PluginUtils::Backup::DeleteBackup(
					Utils::Format(PATH_ISLAND, Address::regionName.c_str()),
					".dat"
				);
			} break;
		}
	}

	class OnlineIslandOptions {
	public:
		static OnlineIslandOptions& GetInstance() {
			static OnlineIslandOptions instance;
			return instance;
		}

		void Enable() {
			//needs setSessionInfoListBufferSize(0x512EB8) to be patched as well (already patched in FixPretendoFindSessionByOwnerCall)
	
			//change IslandFieldHeap size from 0x100000 to 0x45000
			//creating this heap normally fails when joining the island from outside, since there is not enough space in parent heap
			//decrease the size to allow that to work (seems like it doesn't actually need the full 0x100000)
			setIslandFieldHeapSize.Patch(0xB0852445);
			setIslandFieldHeapSize2.Patch(0xA10A0324);

			netCmdHook.Initialize(netCmdStub25.addr, (u32)NetCommand25Hook);
			netCmdHook.SetFlags(USE_LR_TO_RETURN);
			netCmdHook.Enable();

			joinRandomSessionCallHook.Initialize(joinRandomSessionCall.addr, (u32)JoinRandomSessionCallHook);
			joinRandomSessionCallHook.SetFlags(USE_LR_TO_RETURN);
			joinRandomSessionCallHook.Enable();
		}

		void Disable() {
			setIslandFieldHeapSize.Unpatch();
			setIslandFieldHeapSize2.Unpatch();

			netCmdHook.Disable();
			joinRandomSessionCallHook.Disable();

			joinSessionId = 0;
		}

		void OpenOptions() {
			const std::vector<std::string> options = {
				"Quick join", //Language::getInstance()->get(TextID::ISLAND_QUICK_JOIN),
				"Browse sessions", //Language::getInstance()->get(TextID::ISLAND_BROWSE_SESSIONS),
			};

			Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), options);
			if (int index = KB.Open(); index == 0) {
				QuickJoinIsland();
			} else if (index == 1) {
				BrowseSessions();
			}
		}

	private:
		static void QuickJoinIsland() {
			static Address netGameMgrInstance{0x954648};
			static Address netInstance{0xA23020};
			static Address initNet{0x617BE8};
			static Address netIsError{0x747C44};
			static Address connectRandomMatch{0x61937C};
			static Address islandFieldHeapPtr{0x953C60};
			static Address stageHeapPtr{0x953C4C};
			static Address createIslandFieldHeap{0x10C404};
			static Address resetTripRequestInfo{0x283328};
			static Address startFindRandomMatch{0x61E3B0};
			static Address disconnectAndFiniNet{0x627368};
			static Address getRandomMatchingResult{0x75F09C};
			static Address endAdmissionDemo{0x61E3E8};

			enum class State : u32 {
				Init,
				Connect,
				Error,
				Match
			};

			using enum State;
			static State state = Init;

			GameLoopHook::GetInstance()->Add(+[] {
				auto& instance = GetInstance();
				auto* language = Language::getInstance();
				u8* netGameMgr = *reinterpret_cast<u8**>(netGameMgrInstance.addr);

				if (state == Init) {
					if (*(netGameMgr + 0x13266) != 6) {
						OSD::NotifySysFont("Already in a session!", Color::Red);
						instance.joinSessionId = 0;
						return true;
					}

					if (!initNet.Call<bool>()) {
						OSD::NotifySysFont("Failed to initialize network!", Color::Red);
						instance.joinSessionId = 0;
						return true;
					}

					OSD::NotifySysFont("Connecting...", Color::Yellow);
					state = Connect;
				} else if (state == Connect) {
					if (netIsError.Call<bool>(netInstance.addr)) {
						OSD::NotifySysFont("Network error!", Color::Red);
						state = Error;
						return false;
					}

					if (connectRandomMatch.Call<bool>(netGameMgr)) {
						void* islandFieldHeap = *reinterpret_cast<void**>(islandFieldHeapPtr.addr);
						void* stageHeap = *reinterpret_cast<void**>(stageHeapPtr.addr);
						if (islandFieldHeap == nullptr && createIslandFieldHeap.Call<void*>(stageHeap) == nullptr) {
							OSD::NotifySysFont("Not enough memory! Try again from a different room.", Color::Red);
							state = Error;
							return false;
						}

						resetTripRequestInfo.Call<void>(); //called from ModuleEventDemo+0x9778
						Game::ChangeGameMode(Game::GameMode::RANDOM_MATCH);
						startFindRandomMatch.Call<bool>(netGameMgr + 0x27a0); //always returns true
						OSD::NotifySysFont("Joining, please wait...", Color::Yellow);
						state = Match;
					}
				} else if (state == Error && disconnectAndFiniNet.Call<bool>()) {
					state = Init;
					instance.joinSessionId = 0;
					return true;
				} else if (state == Match) {
					u8* islandMgr = netGameMgr + 0x27a0;

					if (s32 result = getRandomMatchingResult.Call<s32>(islandMgr); result < 0) {
						OSD::NotifySysFont("Failed to join island!", Color::Red);
						state = Error;
					} else if (result > 0) {
						endAdmissionDemo.Call<bool>(islandMgr, false); //always returns true
						OSD::NotifySysFont("Joined successfully!", Color::Green);
						state = Init;
						return true;
					}
				}
				return false;
			});
		}

		static void BrowseSessions() {
			static Address netInstance(0xA23020);
			static Address netIsError(0x747C44);
			static Address connectBestFriend(0x62739C); //initializes network automatically
			static Address isConnectedBestFriend(0x626218);
			static Address netFrameworkProcessCmd(0x132AC8);

			enum class State : u32 {
				Init,
				Connect,
			};

			using enum State;
			static State state = Init;

			GameLoopHook::GetInstance()->Add(+[] {
				if (state == Init) {
					if (!connectBestFriend.Call<bool>()) {
						OSD::NotifySysFont("Failed to initialize network!", Color::Red);
						return true;
					}
					OSD::NotifySysFont("Browsing, please wait...", Color::Yellow);
					state = Connect;
				} else if (state == Connect) {
					if (netIsError.Call<bool>(netInstance.addr)) {
						OSD::NotifySysFont("Network error!", Color::Red);
						state = Init;
						return true;
					}

					u32 framework = *reinterpret_cast<u32*>(netInstance.addr + 0xb8);
					if (isConnectedBestFriend.Call<bool>() && netFrameworkProcessCmd.Call<bool>(framework, 0x25, nullptr)) {
						state = Init;
						return true;
					}
				}
				return false;
			});
		}

		static void OnBrowseSessionsFinish() {
			auto& instance = GetInstance();

			static constexpr auto makeMsg = [](size_t index) {
				const auto& instance = GetInstance();
				const auto& session = instance.sessionInfos[index];

				return Utils::Format(
					"Found %zu sessions.\n"
					"Session ID: %08x %s\n"
					"GameMode: %u\n"
					"Host: %08x\n"
					"Attributes:\n"
					"[0]: %08x, [1]: %08x\n"
					"[2]: %08x, [3]: %08x\n"
					"[4]: %08x, [5]: %08x\n",
					instance.foundSessionCount, *(u32*)(session + 0x8), *(bool*)(session + 0x18) ? "" : "Closed",
					*(u32*)(session + 0x4), *(u32*)(session + 0x444),
					*(u32*)(session + 0x1c), *(u32*)(session + 0x20),
					*(u32*)(session + 0x24), *(u32*)(session + 0x28),
					*(u32*)(session + 0x2c), *(u32*)(session + 0x30)
				);
			};

			std::vector<std::string> options;
			options.reserve(instance.foundSessionCount);
			for (size_t i = 0; i < instance.foundSessionCount; i++) {
				const auto& session = instance.sessionInfos[i];
				const bool isOpened = *(bool*)(session + 0x18);
				options.push_back(Utils::Format("%08x: [%u/4] %s", *(u32*)(session + 0x8), *(u32*)(session + 0xc), isOpened ? " " : "-"));
			}

			Keyboard KB(makeMsg(0), options);
			KB.OnKeyboardEvent(+[](Keyboard& kb, KeyboardEvent& event) {
				if (event.type != KeyboardEvent::SelectionChanged) return;
				kb.GetMessage() = makeMsg(static_cast<size_t>(event.selectedIndex));
			});

			if (int index = KB.Open(); index >= 0) {
				instance.joinSessionId = *(u32*)(instance.sessionInfos[index] + 0x8);
				QuickJoinIsland();
			}

			*PluginMenu::GetRunningInstance() -= OnBrowseSessionsFinish;
		}

		static void HandleBrowseSessionsCommand(u32 netSystem) {
			static Address nexSessionSearchCriteriaConstructor(0x40BC10);
			static Address setMaxParticipants(0x40B358);
			static Address setMinParticipants(setMaxParticipants.MoveOffset(0x20));
			static Address setOpenedOnly(setMaxParticipants.MoveOffset(-0x54));
			static Address searchSessions(0x51173C);
			static Address joinSessionById(0x514BAC);
			auto& instance = GetInstance();

			u8 searchCriteria[0xa18];
			nexSessionSearchCriteriaConstructor.Call<void>(searchCriteria);
			searchCriteria[4] = 2;
			*(u32*)&searchCriteria[0xc] = sessionBufSize;
			setMinParticipants.Call<void>(searchCriteria, 1);
			setMaxParticipants.Call<void>(searchCriteria, 4);
			setOpenedOnly.Call<void>(searchCriteria, false);

			instance.foundSessionCount = searchSessions.Call<size_t>(netSystem + 0xc, searchCriteria,
					instance.sessionInfos.data(), instance.sessionInfos.size());

			if (instance.foundSessionCount == 0) {
				OSD::NotifySysFont("No sessions found.", Color::Red);
				return;
			}

			PluginMenu::GetRunningInstance()->Callback(OnBrowseSessionsFinish);
		}

		static NAKED void NetCommand25Hook() {
			asm(R"(
				MOV R0, R4
				B %a0
			)" : : "i" (HandleBrowseSessionsCommand));
		}

		// static NAKED void DispatchAutoMatchmakeSetParamHook() {
		// 	asm(R"(
		// 		LDR R3, .sessionIdPtr
		// 		LDR R1, [R3] @ R1 = joinSessionId
		// 		STR R0, [R3] @ joinSessionId = 0
		// 		STR R1, [SP, #4]
		// 		BX LR
		// 	.sessionIdPtr:
		// 		.word %a0
		// 	)" : : "i" (&GetInstance().joinSessionId));
		// }

		// static NAKED void JoinRandomSessionCallHook() {
		// 	asm(R"(
		// 		LDR R3, [R4, #0x2B8] @ R3 = cmd
		// 		LDR R2, [R3, #0xC] @ R2 = param2 (session ID to join)
		// 		BX LR
		// 	)" : :);
		// }

		static bool JoinRandomSessionCallHook(u32 netImpl, u32 countryCode) {
			auto& instance = GetInstance();
			if (u32 sessionId = instance.joinSessionId; sessionId != 0) {
				instance.joinSessionId = 0;
				return instance.joinSessionById.Call<u32>(netImpl, sessionId) == 0;
			}

			const HookContext& curr = HookContext::GetCurrent();
			static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
			return func.Call<bool>(netImpl, countryCode);
		}

		static constexpr u32 sessionBufSize = 18;

		Address setIslandFieldHeapSize{0x10C40E};
		Address setIslandFieldHeapSize2 = setIslandFieldHeapSize.MoveOffset(6);
		Address netCmdStub25{0x5188D0};
		Address joinRandomSessionCall{0x5177C4};
		Address joinSessionById{0x514BAC};
		Hook netCmdHook;
		Hook dispatchAutoMatchmakeHook;
		Hook joinRandomSessionCallHook;

		std::array<u8*, sessionBufSize> sessionInfos;
		size_t foundSessionCount = 0;
		u32 joinSessionId = 0;
	};

	void IslandOnlineOptions(MenuEntry *entry) {
		if (entry->WasJustActivated()) {
			OnlineIslandOptions::GetInstance().Enable();
		} else if (!entry->IsActivated()) {
			OnlineIslandOptions::GetInstance().Disable();
			return;
		}

		if (entry->Hotkeys[0].IsPressed()) {
			OnlineIslandOptions::GetInstance().OpenOptions();
		}
	}
}