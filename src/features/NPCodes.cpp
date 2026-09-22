#include <cmath>
#include "features/cheats.hpp"
#include "core/game_api/NPC.hpp"
#include "core/infrastructure/Address.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/game_api/Dropper.hpp"
#include "core/RuntimeContext.hpp"
#include "core/HUD.hpp"

namespace CTRPluginFramework {
    static u32 CurrAddress = 0;

	void NPCFunction(MenuEntry *entry) {
		const std::vector<std::string> option = {
			Language::getInstance()->get(TextID::NPC_FUNC_NORMAL),
			Language::getInstance()->get(TextID::NPC_RACE_SPECIAL),
			Language::getInstance()->get(TextID::NPC_FUNC_PLAYER)
		};

		std::vector<NPCdata> npc[3];
		std::vector<std::string> vec;

		NPC::GetLoadedNNPC(npc[0]);
		NPC::GetLoadedSPNPC(npc[1]);
		NPC::GetLoadedPNPC(npc[2]);

		Keyboard KB(Language::getInstance()->get(TextID::NPC_FUNC_SELECT), option);

	redo:
		int res = KB.Open();
		if(res < 0) {
			return;
		}

		if(npc[res].empty()) {
			MessageBox(Utils::Format(Language::getInstance()->get(TextID::NPC_FUNC_NPC_NOT_LOADED).c_str(), option[res].c_str())).SetClear(ClearScreen::Both)();
			goto redo;
		}

		for(NPCdata& str : npc[res]) {
			vec.push_back(str.name);
		}

		KB.GetMessage() = Language::getInstance()->get(TextID::NPC_FUNC_SELECT_LOADED_NPC);
		KB.Populate(vec);

		int res2 = KB.Open();
		if(res2 < 0) {
			return;
		}

		CurrAddress = npc[res][res2].data;

		MessageBox(Utils::Format(Language::getInstance()->get(TextID::NPC_FUNC_SELECTED).c_str(), npc[res][res2].name.c_str())).SetClear(ClearScreen::Both)();
	}

	static int mode = 0;
	static u16 npcID = 0;

	void NPCSetAnimApplySaved(MenuEntry *entry, u32 savedValue) {
		(void)entry;
		mode = static_cast<int>(savedValue & 0x3);
		npcID = static_cast<u16>((savedValue >> 16) & 0xFFFF);
	}

	void NPCSetAnim(MenuEntry *entry) {
		const std::vector<std::string> vec = {
			Language::getInstance()->get(TextID::NPC_ANIM_ANIMATION),
			Language::getInstance()->get(TextID::NPC_ANIM_SNAKE),
			Language::getInstance()->get(TextID::NPC_ANIM_EMOTION),
			Language::getInstance()->get(TextID::NPC_ANIM_ITEM)
		};

		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), vec);

		int op = KB.Open();
		if(op < 0) {
			return;
		}

		if(PluginUtils::Input::PromptNumber<u16>({ Utils::Format(Language::getInstance()->get(TextID::NPC_ANIM_SET).c_str(), vec[op].c_str()), true, 4, npcID }, npcID)) {
			mode = op;
			entry->SetSavedValue((static_cast<u32>(npcID) << 16) | static_cast<u32>(mode & 0x3));
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

		float *pCoords = (float *)(CurrAddress + 0x14);
		if(pCoords != nullptr && !MapEditorActive) { //if not in tile selection mo
			if(entry->Hotkeys[0].IsDown()) {//DPadRight
				pCoords[0] += 5.0;
			}
			if(entry->Hotkeys[1].IsDown()) {//DPadLeft
				pCoords[0] -= 5.0;
			}
			if(entry->Hotkeys[2].IsDown()) {//DPadDown
				pCoords[2] += 5.0;
			}
			if(entry->Hotkeys[3].IsDown()) {//DPadUp
				pCoords[2] -= 5.0;
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
			HUD::Notify(Language::getInstance()->get(TextID::NPC_TELEPORTED_TO_YOU), Color(0x00FA9AFF));
		}
	}

	u16 GetRawRotationData(void) {
        static Address addr1(0x522520);
        static Address addr2(0x5667A8);
        static Address addr3(0x57C4F0);
        static Address point1(0x94FDE8);

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

	/*
	u32 StopNPCMovement(u32 NPCData, u32 u0, u32 u1, u32 u2, u32 u3, u32 u4, u32 u5, u32 u6, u32 u7, u32 u8, u32 u9, u32 u10, u32 u11, u32 u12, u32 u13, u32 u14) {
		if (NPCData == CurrAddress + 0x78) {
			return 0;
		}

		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        return func.Call<u32>(NPCData, u0, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13, u14);
	}

	void NPCMover(MenuEntry *entry) {
		const u16 max_pos = 146; //seems to be the max value
		static Hook hook;

		static Address cameraAsm(0x764504);
		static Address camerapan(0x1A2058);

		static Address func1(0x6E7D54);
		static Address func2(0x6EB384);

		static Address stopAnim(0x6EA828);

		static Address data1(0xAE6864);
        static Address data2(0x8816C4);
		u32 null[]{ 0 };

		if(CurrAddress == 0) {
			return;
		}

		if (entry->IsActivated() && !cameraAsm.IsPatched()) {
			Animation::ExecuteAnimationWrapper(4, 0xF, {0, 0}, 0, 0, 0, 0, 0, 0, 0);
			cameraAsm.Patch(0xEA000020);
			camerapan.Patch(0xE3A00000);

			hook.Initialize(stopAnim.addr, (u32)StopNPCMovement);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		}

		if (!entry->IsActivated() && cameraAsm.IsPatched()) {
			Animation::ExecuteAnimationWrapper(4, 6, {0, 0}, 0, 0, 0, 0, 0, 0, 0);
			cameraAsm.Unpatch();
			camerapan.Unpatch();
			hook.Disable();
		}

		float *pCoords = (float *)(CurrAddress + 0x14);
		if(pCoords == nullptr || MapEditorActive) {
			return;
		}

		float *coord = Camera::GetCoordinates();
		if(coord == nullptr) {
			return;
		}

		coord[0] = pCoords[0];
		coord[2] = pCoords[2];

		if(Controller::IsKeyDown(Key::CPad)) {
			float cspeed = 1.5;

			func2.Call<void>(CurrAddress + 0x78, 0, 0xE, 0, 0); //Walking Anim

			if (Controller::IsKeyDown(Key::B)) {
				cspeed = 2.5;
				func2.Call<void>(CurrAddress + 0x78, 0, 0xF, 0, 0); //Running Anim
			}

			*(u16 *)(CurrAddress + 0x2E) = GetRawRotationData();

			shortVector pos = Controller::GetCirclePadPosition();

			//(circlePosition / max_pos) * 100 = percent_pushed
			float percent_pushedx = (float)pos.x / max_pos * 100;
			float percent_pushedy = (float)pos.y / max_pos * 100;
			//(percent_pushed / 100) * cspeed = speed
			float speedx = (percent_pushedx / 100) * cspeed;
			float speedy = (percent_pushedy / 100) * cspeed;

			pCoords[0] += speedx;
			pCoords[2] += (speedy * -1); //negate y
		}

		//if (Controller::IsKeyPressed(Key::CPad)) {
		//	func1.Call<void>(CurrAddress + 0x78, 0xD, 0, data1, null, null, 0, data2); //Walking
		//}

		//if (Controller::IsKeyReleased(Key::CPad)) {
		//	func1.Call<void>(CurrAddress + 0x78, 6, 0, data1, null, null, 0, data2);
		//}
	}
	*/
}