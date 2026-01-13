#include <cfloat>
#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/IDList.hpp"

namespace CTRPluginFramework {
    u8 ConvertItemIdToInsectId(u8 itemId) {
        if (itemId >= 0x228E && itemId <= 0x22DE) {
            return itemId - 0x228E;
        }
        return 0x51; //default id
    }

    Item ConvertInsectIdToItemId(u8 insectId) {
        if (insectId <= 0x50) {
            return Item(0x228E + insectId);
        }
        return Item(0x7FFE); //invalid item
    }

     void DespawnLastInsectInstantly() {
        static Address dataPointer(0x950534);
        static Address despawnInsect(0x521754);

        u32 dataOffset = *(u32 *)dataPointer.addr;
        if (dataOffset == 0) {
            OSD::Notify("Data Offset is NULL", Color::Red);
            return;
        }

        u32 lastInsectData = 0;

        u32 *piVar5;
        for (piVar5 = *(u32 **)(dataOffset + 0x1C); piVar5; piVar5 = (u32 *)piVar5[1]) {
            u32 insectData = *piVar5;
            if (!insectData) {
                continue;
            }

            if (*(u8 *)(insectData + 0xC6) != 2) {
                continue;
            }

            lastInsectData = insectData;
        }

        if (lastInsectData == 0) {
            return;
        }

        despawnInsect.Call<void>(lastInsectData);
    }

    void SetRoomPriorityForPlayer(u8 target) {
        if (Game::GetOnlinePlayerCount() <= 1) {
            return;
        }

        u8 packet[4];

        static Address updateRoomControl(0x62287C);
        static Address roomPrioritySetPacket(0x626348);
        static Address enqueuePacket(0x625488);

        u8 roomId = Game::GetRoom();

        for (u8 i = 0; i < 4; i++) {
            if (!Player::Exists(i) || !Player::IsInRoom(roomId, i)) {
                continue;
            }

            updateRoomControl.Call<void>(((*(u32 *)Address(0x954648).addr) + 0x1C0), i, i == target);
            roomPrioritySetPacket.Call<void>(packet, i, false, 0xA5, true, i == target, 4);
            enqueuePacket.Call<void>(1, 4, &packet, sizeof(packet));
        }
    }

    static u8 insectId = 0x51;

	void SpawnInsect() {
		static Address spawnInsect(0x2AC924);
		static Address dataPointer(0x950534);
		static Address functionTable(0x8694A0);

		u32 dataOffset = *(u32 *)dataPointer.addr;

		if (dataOffset == 0) {
            OSD::Notify("Data Offset is NULL", Color::Red);
			return;
		}

        SetRoomPriorityForPlayer(Game::GetOnlinePlayerIndex());

        u8 insectIdLocal = insectId;

		if (insectId == 0x51) {
			insectIdLocal = Utils::Random(0, 0x50);
		}

		if (insectIdLocal > 0x50) {
			OSD::Notify("Invalid Insect ID!", Color::Red);
			return;
		}

        /*
        229e mantis crashes
        229f orchid mantis crashes
        22ab ant crashes
        22b7 ladybug crashes
        */
        if (insectIdLocal == 0x10 || insectIdLocal == 0x11 || insectIdLocal == 0x1D || insectIdLocal == 0x29) {
            Item insectItem = ConvertInsectIdToItemId(insectIdLocal);
            OSD::Notify(Utils::Format("Can't spawn %s!", insectItem.GetName().c_str()), Color::Red);
			return;
        }

		u32 *table = (u32 *)functionTable.addr;
		if (!table) {
            OSD::Notify("Data Table is NULL", Color::Red);
			return;
		}
		Address dataFunction(table[insectIdLocal]);

		float u0[3]; //probably rotation of the insect?
		float coords[3];

		if (*(u32 *)(dataOffset + 0x20) == 0) {
            DespawnLastInsectInstantly();
            return;
		}

		dataFunction.Call<bool>(coords, u0, insectIdLocal);

		float *pCoords = PlayerClass::GetInstance()->GetCoordinates();
		if (!pCoords) {
            OSD::Notify("Player Coords are NULL", Color::Red);
            return;
		}

		coords[0] = pCoords[0];
		//coords[1] = pCoords[1]; //keep y coord the same to spawn at proper level
		coords[2] = pCoords[2];

		spawnInsect.Call<void>(dataOffset, insectIdLocal, coords, u0);

        OSD::Notify(Utils::Format("Spawned at %08X", *(u32 *)(*(u32 *)(dataOffset + 0x1C))));

        //u32 insectData = *(u32 *)(*(u32 *)(dataOffset + 0x1C));
        //u8 insectIdLocal = *(u8 *)(insectData + 8);
        //Item insectItem = ConvertInsectIdToItemId(insectIdLocal);
        //OSD::Notify(Utils::Format("Spawned %s (%08X)", insectItem.GetName().c_str(), insectData), Color::Green);
	}
    
    void DespawnAllInsects() {
        static Address dataPointer(0x950534);

		u32 dataOffset = *(u32 *)dataPointer.addr;
        if (dataOffset == 0) {
            OSD::Notify("Data Offset is NULL", Color::Red);
            return;
        }

        int count = 0;

        u32 *piVar5;
        for (piVar5 = *(u32 **)(dataOffset + 0x1C); piVar5; piVar5 = (u32 *)piVar5[1]) {
            u32 insectData = *piVar5;
            if (!insectData) {
                continue;
            }

            if (*(u8 *)(insectData + 0xC6) != 2) {
                continue;
            }

            u32 vtable = *(u32 *)insectData;
            if (!vtable) {
                continue;
            }

            if (*(u8 *)(insectData + 8) == 0xC) { //0xC = bee ID
                continue;
            }

            Address(*(u32 *)(vtable + 0xA8)).Call<void>(insectData);
            count++;
        }

        OSD::Notify(Utils::Format("Despawned %d insects", count), Color::Green);
    }

	static bool spawnInsectEnabled = false;

	void LoopHookMethod(u32 u0) {
		if (spawnInsectEnabled) {
			if (Controller::IsKeysPressed(Key::R + Key::DPadUp)) {
				SpawnInsect();
			}
            if (Controller::IsKeysPressed(Key::R + Key::DPadDown)) {
				DespawnAllInsects();
			}
		}

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		func.Call<void>(u0);
	}

	void SetInsectIdEntry(MenuEntry *entry) {
        static const std::vector<std::string> options = {
            "Set Insect Id",
            "Choose from list"
        };

        static u16 itemInsectId;

        Keyboard KB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), options);
        int choice = KB.Open();
        if (choice < 0) {
            return;
        }
        if (choice == 0) {
            if (Wrap::KB<u16>("", true, 4, itemInsectId, itemInsectId, TextItemChange)) {
                insectId = ConvertItemIdToInsectId(itemInsectId);
            }
        } else {
            std::vector<std::string> insectList;
            for (u16 id = 0x228E; id <= 0x22DE; id++) {
                Item item(id);
                insectList.push_back(item.GetName().c_str());
            }

            insectList.push_back("random insect");

            Keyboard listKB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), insectList);
            int insectChoice = listKB.Open();
            if (insectChoice < 0) {
                return;
            }
            insectId = insectChoice;
        }
	}

	void SpawnInsectEntry(MenuEntry *entry) {
		static Address loopToHookOnto(0x54DB00);
		static Hook hook;

		spawnInsectEnabled = entry->IsActivated();

		if (entry->WasJustActivated()) {
			hook.Initialize((u32)loopToHookOnto.addr, (u32)LoopHookMethod);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		} else if (!spawnInsectEnabled) {
			hook.Disable();
		}
	}

    /*
    insects will still flee if tree is shaken or tree is hit in any way! TODO
    */
    void InsectsCantBeScared(MenuEntry *entry) {
        static Address scareWhenRunning(0x2ACC00);
        static Address scareWhenTooCloseOrMissingWithNet(0x6B19B8);

        if (entry->WasJustActivated()) {
            scareWhenRunning.Patch(0xEA00001A);
            scareWhenTooCloseOrMissingWithNet.Patch(0xEA000068);
        }
        else if (!entry->IsActivated()) {
            scareWhenRunning.Unpatch();
            scareWhenTooCloseOrMissingWithNet.Unpatch();
        }
    }
}