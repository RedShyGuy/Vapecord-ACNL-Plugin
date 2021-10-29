#include <cstdarg>
#include "Helpers/QRCodeGen.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Address.hpp"
#include "TextFileParser.hpp"
#include "MenuPointers.hpp"
#include "Files.h"
#include "Config.hpp"

namespace CTRPluginFramework {
	
//Dump File	
	ExHandler Wrap::Dump(const std::string& path, std::string& filename, const std::string& filetype, WrapLoc dump, ...) {
		File file; 
		Directory dir(path, true);     
			
		if(filename.find(filetype) == std::string::npos) 
			filename += filetype;    
			
	//Couldnt open file		
		if(dir.OpenFile(file, filename, File::RWC) != 0) {
			MessageBox(Language->Get("DUMP_ERROR2")).SetClear(ClearScreen::Top)();
			return ExHandler::ERROR_OP; //error opening file
		}

		va_list dumps;
		va_start(dumps, dump);

		while(dump.Address != 0xFFFFFFFF) { //the last arg needs to be -1 in order for the while loop to exit
			if(file.Dump(dump.Address, dump.Lenght) != 0) {
				MessageBox(Language->Get("DUMP_ERROR1")).SetClear(ClearScreen::Top)();
				return ExHandler::ERROR_DRD; //error dumping file
			}

			dump = va_arg(dumps, WrapLoc); //go to next argument		
		}
		va_end(dumps);

		MessageBox(Language->Get("DUMP_DUMPED") + file.GetFullName()).SetClear(ClearScreen::Top)();
		return ExHandler::SUCCESS; //success
	}

	Directory restoreDIR;
//Restore file	
	ExHandler Wrap::Restore(const std::string& path, const std::string& filetype, const std::string& KBMsg, OnChangeCallback cb, bool HasMSGBox, WrapLoc rest, ...) { 
		redo:
		std::string realPath = path;

		std::vector<std::string> f_Dir, f_File, f_All;
		std::vector<bool> isDir;

		File file;

		Directory::Open(restoreDIR, path);

		if(restoreDIR.ListDirectories(f_Dir) == Directory::OPResult::NOT_OPEN)
			return ExHandler::ERROR_UN;

		if(restoreDIR.ListFiles(f_File, filetype) == Directory::OPResult::NOT_OPEN)
			return ExHandler::ERROR_UN;

	//error if no directory and files found
		if(f_Dir.empty() && f_File.empty()) {
			if(HasMSGBox)
            	MessageBox(Language->Get("RESTORE_NOFILES")).SetClear(ClearScreen::Top)();

            return ExHandler::ERROR_LI; //error listing files
        }

		for(const std::string& str : f_Dir) {
			f_All.push_back(str);
			isDir.push_back(true);
		}
		f_Dir.clear();

	//list files without file extension
		for(const std::string& str : f_File) {
			std::string::size_type index = str.find(filetype);
			if(index != std::string::npos) {
				f_All.push_back(str.substr(0, index));
				isDir.push_back(false);
			}
		}
		f_File.clear();

		Keyboard kb(KBMsg);
		kb.Populate(f_All);
		kb.OnKeyboardEvent(cb);

		s8 uchoice = kb.Open();	
		if(uchoice < 0) 
			return ExHandler::ERROR_UN;

	//if directory show all files in it
		if(isDir[uchoice]) {
		//Can't open directory
			if(Directory::Open(restoreDIR, Utils::Format("%s/%s", path.c_str(), f_All[uchoice].c_str())) != Directory::OPResult::SUCCESS) 
				return ExHandler::ERROR_UN;

			if(restoreDIR.ListFiles(f_File, filetype) == Directory::OPResult::NOT_OPEN) 
				return ExHandler::ERROR_UN;

			realPath = Utils::Format("%s/%s", path.c_str(), f_All[uchoice].c_str());

			f_All.clear();
		//list files without file extension
			for(const std::string& str : f_File) {
				std::string::size_type index = str.find(filetype);
				if(index != std::string::npos) {
					f_All.push_back(str.substr(0, index));
				}
			}

			kb.Populate(f_All);
			uchoice = kb.Open();	
			if(uchoice < 0) 
				goto redo;
		}

		if(restoreDIR.OpenFile(file, f_All[uchoice] + filetype, File::RWC) != Directory::OPResult::SUCCESS) {
			if(HasMSGBox)
				MessageBox(Language->Get("RESTORE_ERROR2")).SetClear(ClearScreen::Top)();

			return ExHandler::ERROR_OP; //error opening file
		}

		va_list restore;
		va_start(restore, rest);

		while(rest.Address != 0xFFFFFFFF) {
			if(file.Inject(rest.Address, rest.Lenght) != 0) {
				if(HasMSGBox)
					MessageBox(Language->Get("RESTORE_ERROR1")).SetClear(ClearScreen::Top)();

				return ExHandler::ERROR_DRD; //error injecting file
			}

			rest = va_arg(restore, WrapLoc); //go to next argument
		}
		va_end(restore);

		if(HasMSGBox)
			MessageBox(Language->Get("RESTORE_RESTORE")).SetClear(ClearScreen::Top)();

		return ExHandler::SUCCESS; //success
	}

	ExHandler Wrap::Delete(const std::string& path, const std::string& filetype) { 
		std::vector<std::string> f_list;
		File file;
		Directory dir(path);

		if(dir.ListFiles(f_list, filetype) == Directory::OPResult::NOT_OPEN)
			return ExHandler::ERROR_UN; //couldnt open directory

		if(f_list.empty()) {
            MessageBox(Language->Get("FILE_RES2")).SetClear(ClearScreen::Top)();
            return ExHandler::ERROR_LI; //error listing files | no files found
        }

		Keyboard kb("");
		kb.Populate(f_list);

		int uchoice = kb.Open();	
		if(uchoice == -1) 
			return ExHandler::ERROR_UN;

		if(dir.OpenFile(file, f_list[uchoice], File::RWC) != 0) {
			MessageBox(Language->Get("RESTORE_ERROR2")).SetClear(ClearScreen::Top)();
			return ExHandler::ERROR_OP; //error opening file
		}

		if((MessageBox(Utils::Format(Language->Get("FILE_PROMPT").c_str(), f_list.at(uchoice)), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			File::Open(file, (path << std::string("/") << f_list.at(uchoice)), File::Mode::WRITE);
			file.Flush();
			file.Close();
			if(File::Remove(path << std::string("/") << f_list.at(uchoice)) != 0) {
				MessageBox(Language->Get("FILE_RES3")).SetClear(ClearScreen::Top)();
				return ExHandler::ERROR_DRD; //error deleting file
			}
			MessageBox(Utils::Format(Language->Get("FILE_RES1").c_str(), f_list.at(uchoice))).SetClear(ClearScreen::Top)();
		}
		dir.Close();	
		return ExHandler::SUCCESS; //success
	}

	u32 Wrap::CalculateBranchInstruction(u32 PC, u32 target) {
		u32 instruction = ((target - PC) >> 2) - 2;

		if(PC > target)
			instruction -= 0x3F000000;

		return instruction;
	}

	u32 Wrap::GetBranchTarget(u32 PC, u32 instruction) {
		u32 target = instruction & 0xFFFFFF;
		
		target += 2;
		target = target << 2;
		target += PC;

		return (target & 0xFFFFFF);
	}

	u32 decodeARMBranch(const u32 src, const u32 val) {
		s32 off = (val & 0xFFFFFF) << 2;
		off = (off << 6) >> 6; //sign extend

		return (u32)src + 8 + off;
	}

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
		if(menu == nullptr) //if menu somehow isn't loaded
			return "NULL";

		std::vector<MenuFolder *> Folders = menu->GetFolderList();
		std::vector<MenuEntry *> Entrys = menu->GetEntryList();
		std::vector<MenuFolder *> SubFolders;

		for(MenuEntry *entry : Entrys) {
			if(entry->IsActivated())
				str += RemoveColorFromString(entry->Name()) + "\n";
		}

		for(MenuFolder *folder : Folders) {
			SubFolders = folder->GetFolderList();
			for(MenuFolder *subfolder : SubFolders) {
				Entrys = subfolder->GetEntryList();
				for(MenuEntry *entry : Entrys) {
					if(entry->IsActivated())
						str += (RemoveColorFromString(folder->Name()) + " -> " + RemoveColorFromString(subfolder->Name()) + " -> " + RemoveColorFromString(entry->Name())) + "\n";
				}	
			}

			Entrys = folder->GetEntryList();
			for(MenuEntry *entry : Entrys) {
				if(entry->IsActivated())
					str += (RemoveColorFromString(folder->Name()) + " -> " + RemoveColorFromString(entry->Name())) + "\n";
			}
		}

		return str;
	}

	int SaveActiveCheats(const std::string &str) {
		const char* data = str.c_str();
		
		Directory dir(Utils::Format(PATH_CRASH, regionName.c_str()), true);
		File file;

		std::string filename = "Active_Cheats";
		filename += ".txt";
		
		if(dir.OpenFile(file, filename, File::RWC | File::TRUNCATE) != 0) 
			return -1;

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
		Directory dir(Utils::Format(PATH_CRASH, regionName.c_str()), true);
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

		if(dir.OpenFile(file, filename, File::RWC) != 0) 
			return -1;

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

		for(int i = 0; i < 13; ++i) 
			str += (Utils::Format("R%02d:   %08X", i, regs->r[i])) + "\n";

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
	
		if(WasSaved == 0) TopScreen.Draw("A | Save Crash Dump", 15, 125, Color::DimGrey);
		else if(WasSaved == 1) TopScreen.Draw("Crash dump saved!", 15, 125, Color::DarkGrey);
		else TopScreen.Draw("Error!", 15, 125, Color::Red);

		TopScreen.Draw("B | Return To Home", 15, 135, Color::DimGrey);

		if(QRSaved == 0) TopScreen.Draw("X | Save QR-Code", 15, 145, Color::DimGrey);
		else if(QRSaved == 1) TopScreen.Draw("QR-Code saved!", 15, 145, Color::DarkGrey); 
		else TopScreen.Draw("Not supported!", 15, 145, Color::Red); 
		//else TopScreen.Draw("Error!", 15, 145, Color::Red); 

		if(!ShowInfo) TopScreen.Draw("Y | Show Crash Info", 15, 155, Color::DimGrey);
		else TopScreen.Draw("Showing crash info!", 15, 155, Color::DarkGrey);

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
			for(int i = 4; i < 18; i += 2)
				BottomScreen.Draw(vec[i], 15, 75 + (10 * ((i / 2) - 2)));
		//Registers 7 - 12
			for(int i = 5; i < 16; i += 2)
				BottomScreen.Draw(vec[i], 135, 75 + (10 * ((i / 2) - 2)));

			BottomScreen.Draw(vec[18], 15, 75 + (10 * 6) + 10); //LR
			BottomScreen.Draw(vec[20], 15, 75 + (10 * 6) + 20); //CPSR
			BottomScreen.Draw(vec[22], 15, 75 + (10 * 6) + 30); //FAR

			BottomScreen.Draw(vec[17], 135, 75 + (10 * 5) + 10); //SP
			BottomScreen.Draw(vec[19], 135, 75 + (10 * 5) + 20); //PC
			BottomScreen.Draw(vec[21], 135, 75 + (10 * 5) + 30); //FPEXC
		}

		OSD::SwapBuffers();

		Controller::Update();
		if(Controller::IsKeyPressed(Key::B))
			return Process::ExceptionCallbackState::EXCB_RETURN_HOME;

		if(Controller::IsKeyPressed(Key::A) && !WasSaved) 
			WasSaved = SaveCrashDump(data);

		if(Controller::IsKeyPressed(Key::Y) && !ShowInfo)
			ShowInfo = true;

		if(Controller::IsKeyPressed(Key::X) && !QRSaved) 
			QRSaved = QRCodeSave();

		return Process::ExceptionCallbackState::EXCB_LOOP;
	}
}
