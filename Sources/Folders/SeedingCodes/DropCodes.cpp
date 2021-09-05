#include <CTRPluginFramework.hpp>
#include <cmath>
#include <algorithm>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
//Item Sequencer
	void Entry_itemsequence(MenuEntry *entry) {	
		std::vector<std::string> cmnOpt = { "" };

		cmnOpt[0] = (ItemSequence::Enabled() ? Color(pGreen) << Language->Get("VECTOR_ENABLED") : Color(pRed) << Language->Get("VECTOR_DISABLED"));

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
		
		ItemSequence::Switch(ItemSequence::Enabled() ? false : true);
		Entry_itemsequence(entry);
	}
//Drop Modifier /*Radius calculations done by Nico*/
	void dropMod(MenuEntry *entry) {		
		const std::vector<std::string> g_dropmod = {
			Language->Get("VECTOR_DROP_PICK"), 
			Language->Get("VECTOR_DROP_PULL"),
			Language->Get("VECTOR_DROP_DROP"),
			Language->Get("VECTOR_DROP_BURY"),
			Language->Get("VECTOR_DROP_PLANT"),
			Language->Get("VECTOR_DROP_PATTERN"),
			Language->Get("VECTOR_DROP_ROCK"),
			Language->Get("VECTOR_DROP_DIG"),
		};
		
		const std::vector<std::string> shapeOpt = {
			Language->Get("VECTOR_SHAPE_FULL_SQUARE"), //0x15 max
			Language->Get("VECTOR_SHAPE_CIRCLE"), //9 max
			Language->Get("VECTOR_SHAPE_HORI_LINE"), //0x10 max
			Language->Get("VECTOR_SHAPE_VERT_LINE"), //0x10 max
			Language->Get("VECTOR_SHAPE_SQUAREE"), //0x10 max
			Language->Get("VECTOR_SHAPE_NE_TO_SW"), //0x10 max
			Language->Get("VECTOR_SHAPE_NW_TO_SE"), //0x10 max
			Language->Get("VECTOR_SHAPE_RESET"),
		};

		if(entry->WasJustActivated()) {
			Process::Patch(Code::dropm3, 0xE1A00000);
			Process::Write8(Code::dropm4, 6); //set anim after bury
		}
		
	//Modify Drop Type	
		if(entry->Hotkeys[0].IsPressed()) { 
			Sleep(Milliseconds(200));
			Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
			optKb.Populate(g_dropmod);
			s8 res = optKb.Open();
			if(res < 0)
				return;

			waitAnim = DropTypes[res].dropAnim;
			DropType = DropTypes[res].dropID;
			Process::Write8(Code::dropm5, DropType);
			return;
		}

	//Modify Drop Radius
		if(entry->Hotkeys[1].IsPressed()) {
			if(IsIndoorsBool) {
				OSD::Notify("Drop Radius Modifier is not usable indoors!", Color::Red);
				return;
			}

			u8 range, count, val;

			Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
			optKb.Populate(shapeOpt);
			s8 index = optKb.Open();
			Sleep(Milliseconds(200));
			switch(index) {
				default: return;
			//Full Square
				case 0: {
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 0x15", true, 1, val, 0)) {
						if(val > 0x15)
							val = 0x15;

						val = val & 0xF;
						range = val * 2 + 1;
						count = range * range;
						Process::Write8(Code::dropm6, count);
						Process::Write8(Code::dropm7, count);
						for(s8 i = 0 - (s8)val; i <= (s8)val; ++i) {
							for(u8 j = 0; j < range; ++j) {
								Process::Write8(Code::DropPattern + j + ((i + val) * range), (0 - (s8)val) + j);
								Process::Write8(Code::DropPattern + count + j + ((i + val) * range), i);
							}
						}	
					} 
					DropPatternON = true;
				} break;
			//circle	
				case 1: {
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 9", true, 1, val, 0)) {
						if(val > 9)
							val = 9;

						val = val & 0xFF;
						count = 2 * val * val - 6 * val + 16;
						Process::Write8(Code::dropm6, count);
						Process::Write8(Code::dropm7, count);
						for(u8 i = 0; i < count; ++i) {
							Process::Write8(Code::DropPattern + i, (s8)nearbyint((float)(val * cos((6.28318530 / count) * i))));
							Process::Write8(Code::DropPattern + count + i, (s8)nearbyint((float)(val * sin((6.28318530 / count) * i))));
						}
					}
					DropPatternON = true;
				} break;
			//horizontal line	
				case 2: {
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 0x10", true, 2, val, 0)) {
						if(val > 0x10)
							val = 0x10;

						range = val * 2 + 1;
						Process::Write8(Code::dropm6, range);
						Process::Write8(Code::dropm7, range);
						for(s8 i = 0 - (s8)val; i <= (s8)val; ++i) {
							Process::Write8(Code::DropPattern + (i + val), i);
							Process::Write8(Code::DropPattern + range + (i + val), 0);
						}

					}
					DropPatternON = true;
				} break;
			//Vertical line	
				case 3: {
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 0x10", true, 2, val, 0)) {
						if(val > 0x10)
							val = 0x10;

						range = val * 2 + 1;
						Process::Write8(Code::dropm6, range);
						Process::Write8(Code::dropm7, range);
						for(s8 i = 0 - (s8)val; i <= (s8)val; ++i) {
							Process::Write8(Code::DropPattern + (i + val), 0);
							Process::Write8(Code::DropPattern + range + (i + val), i);
						}

					}
					DropPatternON = true;
				} break;	
			//square	
				case 4: {
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 0x10", true, 2, val, 0)) {
						if(val > 0x10)
							val = 0x10;

						range = val * 2 + 1;
						count = range * 4;
						Process::Write8(Code::dropm6, count);
						Process::Write8(Code::dropm7, count);
						for(u8 i = 0; i < range; ++i) {
							Process::Write8(Code::DropPattern + i * 4, i - val);
							Process::Write8(Code::DropPattern + count + i * 4, 0 - val);
							Process::Write8(Code::DropPattern + 1 + i * 4, 0 - val);
							Process::Write8(Code::DropPattern + 1 + count + i * 4, i - val);
							Process::Write8(Code::DropPattern + 2 + i * 4, i - val);
							Process::Write8(Code::DropPattern + 2 + count + i * 4, val);
							Process::Write8(Code::DropPattern + 3 + i * 4, val);
							Process::Write8(Code::DropPattern + 3 + count + i * 4, i - val);
						}
					}
					DropPatternON = true;
				} break;
			//NE to SW line	
				case 5: {
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 0x10", true, 2, val, 0)) {
						if(val > 0x10)
							val = 0x10;

						range = val * 2 + 1;
						Process::Write8(Code::dropm6, range);
						Process::Write8(Code::dropm7, range);
						for(s8 i = 0 - (s8)val; i <= (s8)val; ++i) {
							Process::Write8(Code::DropPattern + (i + val), -i);
							Process::Write8(Code::DropPattern + range + (i + val), i);
						}
					}
					DropPatternON = true;
				} break;
			//NW to SE line	
				case 6:	{
					Sleep(Milliseconds(100));
					if(Wrap::KB<u8>(Language->Get("DROP_MODS_RADIUS") << "\nMax Value: 0x10", true, 2, val, 0)) {
						if(val > 0x10)
							val = 0x10;

						range = val * 2 + 1;
						Process::Write8(Code::dropm6, range);
						Process::Write8(Code::dropm7, range);
						for(s8 i = 0 - (s8)val; i <= (s8)val; ++i) {
							Process::Write8(Code::DropPattern + (i + val), i);
							Process::Write8(Code::DropPattern + range + (i + val), i);
						}
					}
					DropPatternON = true;
				} break;
			//reset drop pattern	
				case 7: {
					Process::Write8(Code::dropm6, 9);
					Process::Write8(Code::dropm7, 9);
					Process::Patch(Code::dropm8, 0x0A000006);
					Process::Patch(Code::dropm9, 0xE7971100);
					
					RestoreDropPattern();
					DropPatternON = false;
				} return;
			}
			
			Process::Patch(Code::dropm8, 0xEA000006);
			Process::Patch(Code::dropm9, 0xE1A01000);
			return;
		}

	//set replace item	
		if(entry->Hotkeys[2].IsPressed()) {
			if(Wrap::KB<u32>(Language->Get("DROP_MODS_ENTER_ID"), true, 8, ItemIDToReplace, 0x7FFE))
				OSD::Notify("Now replacing: " << (ItemIDToReplace == 0xFFFFFFFF ? "everything" : Utils::Format("%08X", ItemIDToReplace)));
		}

	//set Item sequence	
		if(entry->Hotkeys[3].IsPressed()) 
			ItemSequence::OpenIS();

		if(GameHelper::GetActualPlayerIndex() == 0) 
			Process::Patch(Code::dropm2, 0x13A00001);
		else 
			Process::Patch(Code::dropm2, 0x13A00000);
		
		if(!entry->IsActivated()) {
			Process::Patch(Code::dropm2, 0x13A00001);
			Process::Patch(Code::dropm3, 0x0A000003);
			Process::Patch(Code::dropm4, 0xE3A01016);
			Process::Patch(Code::dropm5, 0xE3A0000C);
			Process::Patch(Code::dropm6, 0xE3540009);
			Process::Patch(Code::dropm7, 0xE28AB009);
			Process::Patch(Code::dropm8, 0x0A000006);
			Process::Patch(Code::dropm9, 0xE7971100);

			RestoreDropPattern();
		}
	}
//Drop Items	
	void instantDrop(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsPressed()) 
			Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, dropitem, dropitem, ItemChange);
		
		if(turbo ? entry->Hotkeys[1].IsDown() : entry->Hotkeys[1].IsPressed()) {//Key::L + Key::DPadDown
			u32 wX, wY, u0;
			if(Dropper::DropCheck(&wX, &wY, &u0, 0, 0) && !GameHelper::GameSaving()) 
				Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitem, &dropitem, wX, wY, 0, 0, 0, 0, 0, waitAnim, 0xA5);
		}
	}
//Auto Drop	
	void autoDrop(MenuEntry *entry) {
		static bool enabled = false;

		u8 drop;
		u32 wX, wY, u0;
		
		if(entry->Hotkeys[0].IsPressed()) 
			Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, dropitem, dropitem, ItemChange);
		
		else if(entry->Hotkeys[1].IsPressed()) {
			enabled = !enabled;
			OSD::Notify("Auto drop " << (enabled ? Color::Green << "ON" : Color::Red << "OFF"));
		}
		
		if(enabled) {
			if(Dropper::DropCheck(&wX, &wY, &u0, 0, 0) && !GameHelper::GameSaving()) 
				Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitem, &dropitem, wX, wY, 0, 0, 0, 0, 0, waitAnim, 0xA5);
		}
	}
//Touch Drop
	void touchDrop(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsPressed()) 
			Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, dropitem, dropitem, ItemChange);
		
		if((turbo ? Touch::IsDown() : Controller::IsKeyPressed(Key::Touchpad)) && GameHelper::MapBoolCheck() == 1) {
			UIntVector pos = Touch::GetPosition();
			u32 worldx, worldy; 
			switch(GameHelper::RoomCheck()) {
				default: return;
				case 0:
					if(pos.x >= 68 && pos.y >= 50 && pos.x <= 245 && pos.y <= 192) {
						worldx = ((pos.x - 68) / 2.2125) + 0x10;
						worldy = ((pos.y - 50) / 2.2125) + 0x10;
					}
					else 
						return;
				break;
				
				case 0x68:
					if(pos.x >= 116 && pos.y >= 70 && pos.x <= 201 && pos.y <= 157) {
						worldx = ((pos.x - 116) / 2.65) + 0x10;
						worldy = ((pos.y - 70) / 2.65) + 0x10;
					}
					else 
						return;
				break;
			}
			
			Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitem, &dropitem, worldx, worldy, 0, 0, 0, 0, 0, waitAnim, 0xA5);
		}
	}
//Slot Drop
	void ShowInvSlotID(MenuEntry *entry) {
		static bool enabled = false;
		static u32 dropitemid = 0x7FFE;
		
	//Auto Drop Hotkeys
		if(entry->Hotkeys[0].IsPressed()) {
			enabled = !enabled;
			OSD::Notify("Multi Slot Drop " << (enabled ? Color::Green << "ON" : Color::Red << "OFF"));
		}
		
		if(Inventory::GetSelectedSlot() != -1 && Inventory::Opened()) {
			Inventory::ReadSlot(Inventory::GetSelectedSlot(), dropitemid);
			if(IDList::ValidID(dropitemid, 0x295B, 0x292F)) 
				GameHelper::ToOutdoorFlowers(dropitemid);

		//Auto Drop
			u32 wX, wY, u0;
			if(enabled) {
				if(Dropper::DropCheck(&wX, &wY, &u0, 0, 0) && !GameHelper::GameSaving()) 
					Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitemid, &dropitemid, wX, wY, 0, 0, 0, 0, 0, waitAnim, 0xA5);
			}
			
		//Single Drop
			if(turbo ? entry->Hotkeys[1].IsDown() : entry->Hotkeys[1].IsPressed()) {
				if(Dropper::DropCheck(&wX, &wY, &u0, 0, 0) && !GameHelper::GameSaving()) 
					Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitemid, &dropitemid, wX, wY, 0, 0, 0, 0, 0, waitAnim, 0xA5);
			}
		}	
	}
}
