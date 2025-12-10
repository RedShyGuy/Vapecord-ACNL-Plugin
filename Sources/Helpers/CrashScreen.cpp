#include <cstdarg>
#include "Helpers/QRCodeGen.hpp"
#include "Helpers/Wrapper.hpp"
#include "Address/Address.hpp"
#include "LibCtrpfExtras/ColorExtras.hpp"
#include "Files.h"
#include "Config.hpp"

namespace CTRPluginFramework {
    void DrawQrCode(const Screen& screen, u32 posX, u32 posY, const u8* qrcode) {
		const u32 size_qr = qrcodegen_getSize(qrcode);
		u32 size_qr_s = size_qr;
		u32 size_canvas = size_qr + 8;

	//handle scaling
		u32 scale = 1;
		for(; size_canvas * (scale + 1) < 240; scale++);
		size_qr_s *= scale;
		size_canvas *= scale;

		screen.DrawRect(posX - 5, posY - 5, size_canvas - 15, size_canvas - 15, Color::White);

		for(u32 y = 0; y <= size_qr_s; y++) {
			for(u32 x = 0; x <= size_qr_s; x++) {
				Color c = qrcodegen_getModule(qrcode, x / scale, y / scale) ? Color::Black : Color::White;
				screen.DrawRect(posX + x, posY + y, scale, scale, c);
			}
		}
	}

	std::string GetActiveCheats(void) {
		std::string str = "Active Cheats:\n\n";

		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(menu == nullptr) {//if menu somehow isn't loaded
			return "NULL";
		}

		std::vector<MenuFolder *> Folders = menu->GetFolderList();
		std::vector<MenuEntry *> Entrys = menu->GetEntryList();
		std::vector<MenuFolder *> SubFolders;

		for(MenuEntry *entry : Entrys) {
			if(entry->IsActivated()) {
				str += ColorExtras::RemoveColor(entry->Name()) + "\n";
			}
		}

		for(MenuFolder *folder : Folders) {
			SubFolders = folder->GetFolderList();
			for(MenuFolder *subfolder : SubFolders) {
				Entrys = subfolder->GetEntryList();
				for(MenuEntry *entry : Entrys) {
					if(entry->IsActivated()) {
						str += (ColorExtras::RemoveColor(folder->Name()) + " -> " + ColorExtras::RemoveColor(subfolder->Name()) + " -> " + ColorExtras::RemoveColor(entry->Name())) + "\n";
					}
				}	
			}

			Entrys = folder->GetEntryList();
			for(MenuEntry *entry : Entrys) {
				if(entry->IsActivated()) {
					str += (ColorExtras::RemoveColor(folder->Name()) + " -> " + ColorExtras::RemoveColor(entry->Name())) + "\n";
				}
			}
		}

		return str;
	}

	int SaveActiveCheats(const std::string &str) {
		const char* data = str.c_str();
		
		Directory dir(Utils::Format(PATH_CRASH, Address::regionName.c_str()), true);
		File file;

		std::string filename = "Active_Cheats";
		filename += ".txt";
		
		if(dir.OpenFile(file, filename, File::RWC | File::TRUNCATE) != 0) {
			return -1;
		}

		file.Write(data, strlen(data));

		file.Flush();
		file.Close();
		return 1;
	}

	std::string GetExceptionType(ERRF_ExceptionType& type) {
		switch(type) {
			case ERRF_ExceptionType::ERRF_EXCEPTION_PREFETCH_ABORT: 
				return "Prefetch Abort";
			case ERRF_ExceptionType::ERRF_EXCEPTION_DATA_ABORT: 
				return "Data Abort";
			case ERRF_ExceptionType::ERRF_EXCEPTION_UNDEFINED: 
				return "Undefined Instruction";
			case ERRF_ExceptionType::ERRF_EXCEPTION_VFP: 
				return "VFP Exception";
		}
		return "Unknown Error";
	}

	int SaveCrashDump(const char *str) {
		Directory dir(Utils::Format(PATH_CRASH, Address::regionName.c_str()), true);
		File file;

		time_t rawtime;
		struct tm * timeinfo;
		char timestamp[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(timestamp, 80, " [%F_%I-%M%p]", timeinfo);

		std::string filename = "Crash_Dump";
		filename += timestamp;
		filename += ".txt";

		if(dir.OpenFile(file, filename, File::RWC) != 0) {
			return -1;
		}

		file.Write(str, strlen(str));

		file.Flush();
		file.Close();
		return 1;
	}

	std::string StoreCrashData(ERRF_ExceptionInfo* excep, CpuRegisters* regs) {
		std::string str;

		str += ("Vapecord ACNL Plugin | Version: " + Utils::Format("%d.%d.%d", majorV, minorV, revisV)) + "\n";
		str += ("Exception Type: " + GetExceptionType(excep->type)) + "\n";
		
		std::string name = "";
		Process::GetName(name);
		str += ("Process: " + name) + "\n";

		str += (Utils::Format("Title ID: %016llX", Process::GetTitleID())) + "\n\n";

		for(int i = 0; i < 13; ++i) {
			str += (Utils::Format("R%02d:   %08X", i, regs->r[i])) + "\n";
		}

		str += "\n" + (Utils::Format("SP:    %08X", regs->sp)) + "\n";
		str += (Utils::Format("LR:    %08X", regs->lr)) + "\n";
		str += (Utils::Format("PC:    %08X", regs->pc)) + "\n\n";
		str += (Utils::Format("CPSR:  %08X", regs->cpsr)) + "\n";
		str += (Utils::Format("FPEXC: %08X", excep->fpexc)) + "\n";
		str += (Utils::Format("FAR:   %08X", excep->far));

		return str;
	}

	std::vector<std::string> SplitCrashData(char* sentence) {
		std::vector<std::string> vec;
		char *token = strtok(sentence, "\n");

		while(token != nullptr) {
			vec.push_back(token);
        	token = strtok(nullptr, "\n");
		}

		return vec;
	}

	int QRCodeSave(void) {
		return -1;
	}

	static int WasSaved = 0;
	static bool ShowInfo = false;
	static int QRSaved = 0;

	static bool ActiveCheatsSaved = false;

	Process::ExceptionCallbackState CustomExceptionHandler(ERRF_ExceptionInfo* excep, CpuRegisters* regs) {
		if(!ActiveCheatsSaved) {
			std::string str = GetActiveCheats();
			SaveActiveCheats(str);

			ActiveCheatsSaved = true;
		}

		const Screen& TopScreen = OSD::GetTopScreen();
		const Screen& BottomScreen = OSD::GetBottomScreen();

	//top screen
		TopScreen.DrawRect(10, 10, 376, 216, Color::Black);

		u8 qrcode[qrcodegen_BUFFER_LEN_MAX];
		u8 temp[qrcodegen_BUFFER_LEN_MAX];

		std::string datastr = StoreCrashData(excep, regs);

		const char* data = datastr.c_str();
		qrcodegen_encodeText(data, temp, qrcode, qrcodegen_Ecc_LOW, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	
		DrawQrCode(TopScreen, 175, 15, qrcode);

		TopScreen.Draw("Oh Uh, looks like", 15, 15, Color(225 ,0 ,52));
		TopScreen.Draw("your game crashed! :(", 15, 25, Color(225 ,0 ,52));

		TopScreen.Draw("Report this crash on", 15, 45);
		TopScreen.Draw("the Vapecord Discord", 15, 55);
		TopScreen.Draw("by sending the QR-Code", 15, 65);
		TopScreen.Draw("with a description of", 15, 75);
		TopScreen.Draw("what happened!", 15, 85);
	
		if(WasSaved == 0) {
			TopScreen.Draw("A | Save Crash Dump", 15, 125, Color::DimGrey);
		}
		else if(WasSaved == 1) {
			TopScreen.Draw("Crash dump saved!", 15, 125, Color::DarkGrey);
		}
		else {
			TopScreen.Draw("Error!", 15, 125, Color::Red);
		}

		TopScreen.Draw("B | Return To Home", 15, 135, Color::DimGrey);

		if(QRSaved == 0) {
			TopScreen.Draw("X | Save QR-Code", 15, 145, Color::DimGrey);
		}
		else if(QRSaved == 1) {
			TopScreen.Draw("QR-Code saved!", 15, 145, Color::DarkGrey); 
		}
		else {
			TopScreen.Draw("Not supported!", 15, 145, Color::Red); 
		}
		//else TopScreen.Draw("Error!", 15, 145, Color::Red); 

		if(!ShowInfo) {
			TopScreen.Draw("Y | Show Crash Info", 15, 155, Color::DimGrey);
		}
		else {
			TopScreen.Draw("Showing crash info!", 15, 155, Color::DarkGrey);
		}

		TopScreen.Draw("Discord Link:", 15, 195);
		TopScreen.Draw(DISCORDINV, 15, 205, Color::Purple);

	//bottom screen
		if(ShowInfo) {
			BottomScreen.DrawRect(10, 10, 300, 220, Color::Black);

			char datachar[datastr.length() + 1];
			strcpy(datachar, datastr.c_str());
			std::vector<std::string> vec = SplitCrashData(datachar);

			BottomScreen.Draw(vec[0], 15, 15, Color::DimGrey); //Vapecord Version
			BottomScreen.Draw(vec[1], 15, 30, Color::Red); //Exception Type
			BottomScreen.Draw(vec[2], 15, 45); //Process Name
			BottomScreen.Draw(vec[3], 15, 55); //Title ID
			
		//Registers 0 - 6
			for(int i = 4; i < 18; i += 2) {
				BottomScreen.Draw(vec[i], 15, 75 + (10 * ((i / 2) - 2)));
			}
		//Registers 7 - 12
			for(int i = 5; i < 16; i += 2) {
				BottomScreen.Draw(vec[i], 135, 75 + (10 * ((i / 2) - 2)));
			}

			BottomScreen.Draw(vec[18], 15, 75 + (10 * 6) + 10); //LR
			BottomScreen.Draw(vec[20], 15, 75 + (10 * 6) + 20); //CPSR
			BottomScreen.Draw(vec[22], 15, 75 + (10 * 6) + 30); //FAR

			BottomScreen.Draw(vec[17], 135, 75 + (10 * 5) + 10); //SP
			BottomScreen.Draw(vec[19], 135, 75 + (10 * 5) + 20); //PC
			BottomScreen.Draw(vec[21], 135, 75 + (10 * 5) + 30); //FPEXC
		}

		OSD::SwapBuffers();

		Controller::Update();
		if(Controller::IsKeyPressed(Key::B)) {
			return Process::ExceptionCallbackState::EXCB_RETURN_HOME;
		}

		if(Controller::IsKeyPressed(Key::A) && !WasSaved) {
			WasSaved = SaveCrashDump(data);
		}

		if(Controller::IsKeyPressed(Key::Y) && !ShowInfo) {
			ShowInfo = true;
		}

		if(Controller::IsKeyPressed(Key::X) && !QRSaved) {
			QRSaved = QRCodeSave();
		}

		return Process::ExceptionCallbackState::EXCB_LOOP;
	}
}