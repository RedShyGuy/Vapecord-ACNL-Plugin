#include <CTRPluginFramework.hpp>
#include <cmath>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
    static u32 CurrAddress = 0;

	bool ShowCoords(const Screen &screen) {
		if(CurrAddress == 0)
			return 0;

		float *coord = (float *)(CurrAddress + 0x14);
		u16 rotation = *(u16 *)(CurrAddress + 0x2E);
		if(coord == nullptr)
			return 0;

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
		}

		OSD::Run(ShowCoords);
	}

	void NPCFunction(MenuEntry *entry) {
		if(!entry->Hotkeys[0].IsPressed()) //Key::L + Key::A
			return;

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
		Sleep(Milliseconds(200));
		s8 res = KB.Open();
		if(res < 0)
			return;

		if(npc[res].empty()) {
			Sleep(Milliseconds(200));
			MessageBox(Utils::Format("No %s is currently loaded!", option[res].c_str())).SetClear(ClearScreen::Both)();
			goto redo;
		}

		for(NPCdata& str : npc[res]) 
			vec.push_back(str.name);

		KB.GetMessage() = "Select Loaded NPC:";
		KB.Populate(vec);
		Sleep(Milliseconds(200));
		s8 res2 = KB.Open();
		if(res2 < 0)
			return;

		CurrAddress = npc[res][res2].data;

		OSD::Notify(Utils::Format("%s selected!", npc[res][res2].name.c_str()));

        //return; //debug purpose
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		*menu += checkloadstate;
	}

	static int mode = 0;
	static u16 npcID = 0;

	void NPCSetAnim(MenuEntry *entry) {
		static const std::vector<std::string> vec = { "Animation", "Snake", "Emotion", "Item" };	

		Keyboard KB("Select Option", vec);
		s8 op = KB.Open();
		if(op < 0)
			return;

		if(Wrap::KB<u16>(Utils::Format("Set %s ID:", vec[op].c_str()), true, 4, npcID, npcID))
			mode = op;
	}

	void NPCAnimation(MenuEntry *entry) {
        static const u32 func1 = Region::AutoRegion(0x6E7D54, 0x6E7204, 0x6E6D74, 0x6E6D4C, 0x6E6520, 0x6E64F8, 0x6E60C8, 0x6E60A0);
        static const u32 func2 = Region::AutoRegion(0x6EB384, 0x6EA834, 0x6EA3A4, 0x6EA37C, 0x6E9B50, 0x6E9B28, 0x6E96F8, 0x6E96D0);
        static const u32 func3 = Region::AutoRegion(0x6EC4E0, 0x6EB990, 0x6EB500, 0x6EB4D8, 0x6EACAC, 0x6EAC84, 0x6EA854, 0x6EA82C);
        static const u32 func4 = Region::AutoRegion(0x6EE778, 0x6EDC28, 0x6ED798, 0x6ED770, 0x6ECF44, 0x6ECF1C, 0x6ECAEC, 0x6ECAC4);
        static const u32 data1 = Region::AutoRegion(0xAE6864, 0xAE5658, 0xAE5864, 0xAE5864, 0xADF864, 0xADE864, 0xADE864, 0xADE864);
        static const u32 data2 = Region::AutoRegion(0x8816C4, 0x8806BC, 0x880550, 0x880550, 0x87A970, 0x879970, 0x879920, 0x879920);

		if(CurrAddress == 0)
			return;

		if(turbo ? entry->Hotkeys[0].IsDown() : entry->Hotkeys[0].IsPressed()) { //L + B
			u32 null[]{ 0 };
			switch(mode) {
				case 0:
					FUNCTION(func1).Call<void>(CurrAddress + 0x78, npcID, 0, data1, null, null, 0, data2); //Animation	
				break;
				case 1:
					FUNCTION(func2).Call<void>(CurrAddress + 0x78, 0, npcID, 0, 0); //Snake
				break;
				case 2:
					FUNCTION(func3).Call<void>(CurrAddress + 0x78, 0, npcID); //Emote
				break;
				case 3:
					FUNCTION(func4).Call<void>(CurrAddress + 0x78, 0, &npcID); //Item
				break;
			}
		}
	}

	void NPCCoordinates(MenuEntry *entry) {
		if(CurrAddress == 0)
			return;

		if(entry->Hotkeys[0].IsDown()) { //L
			float *pCoords = (float *)(CurrAddress + 0x14);
			if(pCoords != nullptr && !MapEditorActive) { //if not in tile selection mo
				if(entry->Hotkeys[1].IsDown()) //DPadRight
					pCoords[0] += 5.0;
				if(entry->Hotkeys[2].IsDown()) //DPadLeft
					pCoords[0] -= 5.0;
				if(entry->Hotkeys[3].IsDown()) //DPadDown
					pCoords[2] += 5.0;
				if(entry->Hotkeys[4].IsDown()) //DPadUp
					pCoords[2] -= 5.0;
			}
		}
	}

	void NPCTeleportToYou(MenuEntry *entry) {
		if(CurrAddress == 0)
			return;

		if(!entry->Hotkeys[0].IsPressed()) //L + Y
			return;

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
        static const u32 addr1 = Region::AutoRegion(0x522520, 0x521E74, 0x521568, 0x521568, 0x520E84, 0x520E84, 0x520818, 0x520818);
        static const u32 addr2 = Region::AutoRegion(0x5667A8, 0x565CC0, 0x5657F0, 0x5657F0, 0x5650E0, 0x5650E0, 0x564E00, 0x564E00);
        static const u32 addr3 = Region::AutoRegion(0x57C4F0, 0x57BA08, 0x57B538, 0x57B538, 0x57AE28, 0x57AE28, 0x57AB48, 0x57AB48);
        static const u32 point1 = Region::AutoRegion(0x94FDE8, 0x94EDD8, 0x94EDE8, 0x94EDE8, 0x948DE8, 0x947DE8, 0x947DE8, 0x947DE8);

		float fVar914 = 0;
		u16 uVar918 = 0;
		u8 uVar91C = 0;
		u8 uVar91D = 0;

		float *coord = FUNCTION(addr1).Call<float *>();
		
		float fVar1 = coord[1];
		fVar914 = coord[0];

		fVar914 = 0.0 - fVar914;
        fVar914 = sqrt(fVar1 * fVar1 + fVar914 * fVar914);

		if(0x3F800000 < (int)fVar914) {
          fVar914 = 1.0;
        }

		uVar918 = FUNCTION(addr2).Call<u16>(coord[0], coord[1]);

		u32 iVar2 = *(u32 *)point1;
		if(iVar2 != 0) {
        	int iVar9 = *(int *)(iVar2 + 0x1A4);
        	u16 sVar5 = FUNCTION(addr3).Call<u16>(iVar9 + 0x24, iVar9 + 0x30);
        	uVar918 = sVar5 + uVar918;
        }

		return (uVar918 * -1) - 0x8000;
	}

	void NPCRotate(MenuEntry *entry) {
		if(CurrAddress == 0)
			return;

		if(entry->Hotkeys[0].IsDown() && Controller::IsKeyDown(Key::CPad)) { //L
			*(u16 *)(CurrAddress + 0x2E) = GetRawRotationData();
		}
	}
}