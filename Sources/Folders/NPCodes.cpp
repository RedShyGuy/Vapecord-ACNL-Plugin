#include <cmath>
#include "cheats.hpp"
#include "Helpers/NPC.hpp"
#include "Address/Address.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Dropper.hpp"
#include "RuntimeContext.hpp"

namespace CTRPluginFramework {
    static u32 CurrAddress = 0;

	bool ShowCoords(const Screen &screen) {
		if(CurrAddress == 0) {
			return 0;
		}

		float *coord = (float *)(CurrAddress + 0x14);
		u16 rotation = *(u16 *)(CurrAddress + 0x2E);
		if(coord == nullptr) {
			return 0;
		}

		if(screen.IsTop) {
			screen.Draw(Utils::Format("X | %f", coord[0]), 0, 0);
			screen.Draw(Utils::Format("Y | %f", coord[1]), 0, 10);
			screen.Draw(Utils::Format("Z | %f", coord[2]), 0, 20);
			screen.Draw(Utils::Format("R | %04X", rotation), 0, 30);
			screen.Draw(Utils::Format("A | %08X", CurrAddress), 0, 40);
			return 1;
		}

		return 0;
	}

	void checkloadstate(void) {
		if(CurrAddress == 0 || (float *)(CurrAddress + 0x14) == nullptr) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu -= checkloadstate;
			OSD::Stop(ShowCoords);
			return;
		}

		OSD::Run(ShowCoords);
	}

	void NPCFunction(MenuEntry *entry) {
		if(!entry->Hotkeys[0].IsPressed()) {//Key::L + Key::A
			return;
		}

		static const std::vector<std::string> option = {
			"Normal NPC", "Special NPC", "Player NPC"
		};
		
		std::vector<NPCdata> npc[3];
		std::vector<std::string> vec;

		NPC::GetLoadedNNPC(npc[0]);
		NPC::GetLoadedSPNPC(npc[1]);
		NPC::GetLoadedPNPC(npc[2]);

		Keyboard KB("Select NPC Type:", option);

	redo:
		int res = KB.Open();
		if(res < 0) {
			return;
		}

		if(npc[res].empty()) {
			MessageBox(Utils::Format("No %s is currently loaded!", option[res].c_str())).SetClear(ClearScreen::Both)();
			goto redo;
		}

		for(NPCdata& str : npc[res]) {
			vec.push_back(str.name);
		}

		KB.GetMessage() = "Select Loaded NPC:";
		KB.Populate(vec);

		int res2 = KB.Open();
		if(res2 < 0) {
			return;
		}

		CurrAddress = npc[res][res2].data;

		OSD::Notify(Utils::Format("%s selected!", npc[res][res2].name.c_str()));

	#if DEVMODE
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		*menu += checkloadstate;
	#endif
	}

	static int mode = 0;
	static u16 npcID = 0;

	void NPCSetAnim(MenuEntry *entry) {
		static const std::vector<std::string> vec = { "Animation", "Snake", "Emotion", "Item" };	

		Keyboard KB("Select Option", vec);

		int op = KB.Open();
		if(op < 0) {
			return;
		}

		if(Wrap::KB<u16>(Utils::Format("Set %s ID:", vec[op].c_str()), true, 4, npcID, npcID)) {
			mode = op;
		}
	}

	void NPCAnimation(MenuEntry *entry) {
        static Address func1(0x6E7D54);
        static Address func2(0x6EB384);
        static Address func3(0x6EC4E0);
        static Address func4(0x6EE778);
        static Address data1(0xAE6864);
        static Address data2(0x8816C4);

		if(CurrAddress == 0) {
			return;
		}

		if(RuntimeContext::getInstance()->isTurbo() ? entry->Hotkeys[0].IsDown() : entry->Hotkeys[0].IsPressed()) { //L + B
			u32 null[]{ 0 };
			switch(mode) {
				case 0:
					func1.Call<void>(CurrAddress + 0x78, npcID, 0, data1, null, null, 0, data2); //Animation	
				break;
				case 1:
					func2.Call<void>(CurrAddress + 0x78, 0, npcID, 0, 0); //Snake
				break;
				case 2:
					func3.Call<void>(CurrAddress + 0x78, 0, npcID); //Emote
				break;
				case 3:
					func4.Call<void>(CurrAddress + 0x78, 0, &npcID); //Item
				break;
			}
		}
	}

	void NPCCoordinates(MenuEntry *entry) {
		if(CurrAddress == 0) {
			return;
		}

		if(entry->Hotkeys[0].IsDown()) { //L
			float *pCoords = (float *)(CurrAddress + 0x14);
			if(pCoords != nullptr && !MapEditorActive) { //if not in tile selection mo
				if(entry->Hotkeys[1].IsDown()) {//DPadRight
					pCoords[0] += 5.0;
				}
				if(entry->Hotkeys[2].IsDown()) {//DPadLeft
					pCoords[0] -= 5.0;
				}
				if(entry->Hotkeys[3].IsDown()) {//DPadDown
					pCoords[2] += 5.0;
				}
				if(entry->Hotkeys[4].IsDown()) {//DPadUp
					pCoords[2] -= 5.0;
				}
			}
		}
	}

	void NPCTeleportToYou(MenuEntry *entry) {
		if(CurrAddress == 0) {
			return;
		}

		if(!entry->Hotkeys[0].IsPressed()) {//L + Y
			return;
		}

		float *pCoords = (float *)(CurrAddress + 0x14);
		float *coords = PlayerClass::GetInstance()->GetCoordinates();
		if(pCoords != nullptr && coords != nullptr) {
			pCoords[0] = coords[0];
			pCoords[1] = coords[1];
			pCoords[2] = coords[2];
			OSD::Notify("NPC teleported to you!", Color(0x00FA9AFF));
		}
	}

	u16 GetRawRotationData(void) {
        static Address addr1(0x522520);
        static Address addr2(0x5667A8);
        static Address addr3(0x57C4F0);
        static const Address point1(0x94FDE8);

		float fVar914 = 0;
		u16 uVar918 = 0;
		u8 uVar91C = 0;
		u8 uVar91D = 0;

		float *coord = addr1.Call<float *>();
		
		float fVar1 = coord[1];
		fVar914 = coord[0];

		fVar914 = 0.0 - fVar914;
        fVar914 = sqrt(fVar1 * fVar1 + fVar914 * fVar914);

		if(0x3F800000 < (int)fVar914) {
          fVar914 = 1.0;
        }

		uVar918 = addr2.Call<u16>(coord[0], coord[1]);

		u32 iVar2 = *(u32 *)point1.addr;
		if(iVar2 != 0) {
        	int iVar9 = *(int *)(iVar2 + 0x1A4);
        	u16 sVar5 = addr3.Call<u16>(iVar9 + 0x24, iVar9 + 0x30);
        	uVar918 = sVar5 + uVar918;
        }

		return (uVar918 * -1) - 0x8000;
	}

	void NPCRotate(MenuEntry *entry) {
		if(CurrAddress == 0) {
			return;
		}

		if(entry->Hotkeys[0].IsDown() && Controller::IsKeyDown(Key::CPad)) { //L
			*(u16 *)(CurrAddress + 0x2E) = GetRawRotationData();
		}
	}
}