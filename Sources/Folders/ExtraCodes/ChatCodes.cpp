#include "cheats.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "Helpers/Chat.hpp"
#include "Files.h"

namespace CTRPluginFramework {
//Chat Bubbles Don't Disappear /*Credits to Levi*/
	void bubblesDisappear(MenuEntry *entry) { 
		static Address bubble(0x2145F8);

		if(entry->WasJustActivated()) {
			bubble.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			bubble.Unpatch();
		}
	}

	static std::string Holder = "";
//65 char is max (0x82)
	void ChatCopyPaste(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			if(!File::Exists(Utils::Format(PATH_CBOARD, Address::regionName.c_str()))) {
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
			}

			Holder.clear();
			File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();
		}

		if(entry->Hotkeys[0].IsPressed()) {
			Holder.clear();
			File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();

			if(GameKeyboard::Write(Holder)) {
				OSD::Notify("Pasted: " << Holder, Color(0x0091FFFF));
			}
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			Holder.clear();
			if(GameKeyboard::CopySelected(Holder)) {
				File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

				File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);

				LineWriter writer(f_board);
				writer << Holder;
				writer.Flush();
				writer.Close();

				f_board.Flush();
				f_board.Close();

				OSD::Notify("Copied: " << Holder, Color(0xFF0077FF));
			}
		}

		else if(entry->Hotkeys[2].IsPressed()) {
			Holder.clear();
			if(GameKeyboard::CopySelected(Holder)) {
				GameKeyboard::DeleteSelected();

				File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

				File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);
				
				LineWriter writer(f_board);
				writer << Holder;
				writer.Flush();
				writer.Close();

				f_board.Flush();
				f_board.Close();

				OSD::Notify("Cut: " << Holder, Color(0x00FF6FFF));
			}
		}

		else if(entry->Hotkeys[3].IsPressed()) {
			if(GameKeyboard::DeleteSelected()) {
				OSD::Notify("Deleted Selected", Color::Red);
			}
		}
	}

//Force Send Chat
	void Forcesendchat(MenuEntry *entry) {
		static Address callchat(0x52440C);
		static Address patch(0x1939EC);

		if(entry->WasJustActivated()) {
			patch.Patch(0xEA000000);
		}

		if(entry->Hotkeys[0].IsDown()) {
			if(!GameKeyboard::IsOpen()) {
				return;
			}

			callchat.Call<void>(1, 2);
		}

		if(!entry->IsActivated()) {
			patch.Unpatch();
		}
	}

	void chatCommands(MenuEntry *entry) {
		if (entry->WasJustActivated()) {
			Chat::EnableCommands();
		}
		else if (!entry->IsActivated()) {
			Chat::DisableCommands();
		}
	}

	int ButtonList[5][3] = {
		{16, 5, 4}, //Copy
		{48, 5, 5}, //Paste
		{96, 5, 3}, //Cut
		{128, 5, 6}, //Delete
		{170, 5, 4}, //Send
	};

	int TouchButton(void) {
		static UIntRect CopyCoord(ButtonList[0][0], ButtonList[0][1], ButtonList[0][2] * 6, 11);
		static UIntRect PasteCoord(ButtonList[1][0], ButtonList[1][1], ButtonList[1][2] * 6, 11);
		static UIntRect CutCoord(ButtonList[2][0], ButtonList[2][1], ButtonList[2][2] * 6, 11);
		static UIntRect DeleteCoord(ButtonList[3][0], ButtonList[3][1], ButtonList[3][2] * 6, 11);
		static UIntRect SendCoord(ButtonList[4][0], ButtonList[4][1], ButtonList[4][2] * 6, 11);

		if (CopyCoord.Contains(Touch::GetPosition())) {
			return 1;
		}
		if (PasteCoord.Contains(Touch::GetPosition())) {
			return 2;
		}
		if (CutCoord.Contains(Touch::GetPosition())) {
			return 3;
		}
		if (DeleteCoord.Contains(Touch::GetPosition())) {
			return 4;
		}
		if (SendCoord.Contains(Touch::GetPosition())) {
			return 5;
		}

		return 0;
	}

	bool DrawChatButton(const Screen &scr) {
		if(!GameKeyboard::IsOpen()) {
			return false;
		}

		if (scr.IsTop) {
			return false;
		}

		scr.Draw("Copy", ButtonList[0][0], ButtonList[0][1], Color::Yellow, Color::Black);
		scr.Draw("Paste", ButtonList[1][0], ButtonList[1][1], Color::Yellow, Color::Black);
		scr.Draw("Cut", ButtonList[2][0], ButtonList[2][1], Color::Yellow, Color::Black);
		scr.Draw("Delete", ButtonList[3][0], ButtonList[3][1], Color::Yellow, Color::Black);
		scr.Draw("Send", ButtonList[4][0], ButtonList[4][1], Color::Yellow, Color::Black);
		
		return true;
	}

	// Add a convenient button to chat.
	void ChatButton(MenuEntry *entry) {
		static Address callchat(0x52440C);

		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(entry->WasJustActivated()) {
			OSD::Run(DrawChatButton); 
			if(!File::Exists(Utils::Format(PATH_CBOARD, Address::regionName.c_str()))) {
				File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
			}

			Holder.clear();
			File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();
		}
		else if(!entry->IsActivated()) {
			OSD::Stop(DrawChatButton); 
		}

		if (Controller::IsKeyPressed(Touchpad) && GameKeyboard::IsOpen()) {
			int ButtonID = TouchButton();
			switch (ButtonID) {
				case 1: { // Copy
					Holder.clear();
					if (GameKeyboard::CopySelected(Holder)) {
						File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
						File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

						File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);

						LineWriter writer(f_board);
						writer << Holder;
						writer.Flush();
						writer.Close();

						f_board.Flush();
						f_board.Close();
						OSD::Notify("Copied", Color(0xFF0077FF));
					}
				}
				break;
				case 2: {// Paste
					Holder.clear();
					File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::READ);
					LineReader reader(f_board);
					reader(Holder);
					Holder.resize(65);
					Holder.shrink_to_fit();

					f_board.Flush();
					f_board.Close();

					GameKeyboard::Write(Holder);
				}
				break;
				case 3: {// Cut
					Holder.clear();
					if (GameKeyboard::CopySelected(Holder)){
						GameKeyboard::DeleteSelected();

						File::Remove(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));
						File::Create(Utils::Format(PATH_CBOARD, Address::regionName.c_str()));

						File f_board(Utils::Format(PATH_CBOARD, Address::regionName.c_str()), File::WRITE);
						
						LineWriter writer(f_board);
						writer << Holder;
						writer.Flush();
						writer.Close();

						f_board.Flush();
						f_board.Close();

						OSD::Notify("Cut", Color(0x00FF6FFF));
					}
				}
				break;
				case 4: // Delete
					GameKeyboard::Delete();
				break;
				case 5: // Send
					callchat.Call<void>(1, 2);
				break;
				default:
				break;
			}
		}
	}
}
