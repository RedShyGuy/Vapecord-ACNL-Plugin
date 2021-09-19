#include "cheats.hpp"

namespace CTRPluginFramework {
//Chat Bubbles Don't Disappear /*Credits to Levi*/
	void bubblesDisappear(MenuEntry *entry) { 
		static const Address bubble(0x2145F8, 0x21403C, 0x214618, 0x214618, 0x214538, 0x214538, 0x214504, 0x214504);
		if(entry->WasJustActivated()) 
			Process::Patch(bubble.addr, 0xE1A00000);
		else if(!entry->IsActivated()) 
			Process::Patch(bubble.addr, 0x0A000006);
	}

	static std::string Holder = "";
//65 char is max (0x82)
	void ChatCopyPaste(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			if(!File::Exists(PATH_CBOARD)) 
				File::Create(PATH_CBOARD);

			Holder.clear();
			File f_board(PATH_CBOARD, File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();
		}

		if(entry->Hotkeys[0].IsPressed()) {
			Holder.clear();
			File f_board(PATH_CBOARD, File::READ);
			LineReader reader(f_board);
			reader(Holder);
			Holder.resize(65);
			Holder.shrink_to_fit();

			f_board.Flush();
			f_board.Close();

			if(GameKeyboard::Write(Holder))
				OSD::Notify("Pasted: " << Holder, Color(0x0091FFFF));
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			Holder.clear();
			if(GameKeyboard::CopySelected(Holder)) {
				File::Remove(PATH_CBOARD);
				File::Create(PATH_CBOARD);

				File f_board(PATH_CBOARD, File::WRITE);

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

				File::Remove(PATH_CBOARD);
				File::Create(PATH_CBOARD);

				File f_board(PATH_CBOARD, File::WRITE);
				
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
			if(GameKeyboard::DeleteSelected())
				OSD::Notify("Deleted Selected", Color::Red);
		}
	}

//Force Send Chat
	void Forcesendchat(MenuEntry *entry) {
		static Address callchat(0x52440C, 0x523D60, 0x523454, 0x523454, 0x522D40, 0x522D40, 0x522A48, 0x522A48);
		if(entry->WasJustActivated())
			Process::Patch(Code::DisableChatRemoval.addr, 0xEA000000);

		if(entry->Hotkeys[0].IsDown()) {
			if(!GameKeyboard::IsOpen())
				return;

			callchat.Call<void>(1, 2);
		}

		if(!entry->IsActivated())
			Process::Patch(Code::DisableChatRemoval.addr, 0xE5900000);
	}
}
