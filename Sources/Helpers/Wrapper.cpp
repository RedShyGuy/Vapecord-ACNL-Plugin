#include <cstdarg>
#include "Helpers/Wrapper.hpp"
#include "Address/Address.hpp"
#include "Files.h"
#include "Config.hpp"
#include "cheats.hpp"

namespace CTRPluginFramework {
	ExHandler Wrap::Dump(const std::string& path, std::string& filename, const std::string& filetype, WrapLoc *dump, ...) {
		File file; 
		Directory dir(path, true);     
			
		if(filename.find(filetype) == std::string::npos) {
			filename += filetype;
		}
			
	//Couldnt open file		
		if(dir.OpenFile(file, filename, File::RWC) != 0) {
			MessageBox(Language::getInstance()->get("DUMP_ERROR2")).SetClear(ClearScreen::Top)();
			return ExHandler::ERROR_OP; //error opening file
		}

		va_list dumps;
		va_start(dumps, dump);

		while(dump != nullptr) { //the last arg needs to be nullptr in order for the while loop to exit
			if(file.Dump(*(u32 *)&dump->Address, dump->Lenght) != 0) {
				MessageBox(Language::getInstance()->get("DUMP_ERROR1")).SetClear(ClearScreen::Top)();
				return ExHandler::ERROR_DRD; //error dumping file
			}

			OSD::Notify(Utils::Format("Address: %08X", dump->Address));
			OSD::Notify(Utils::Format("Lenght: %08X", dump->Lenght));

			dump = va_arg(dumps, WrapLoc*); //go to next argument		
		}
		va_end(dumps);

		MessageBox(Language::getInstance()->get("DUMP_DUMPED") + file.GetFullName()).SetClear(ClearScreen::Top)();
		return ExHandler::SUCCESS; //success
	}

	Directory Wrap::restoreDIR;
	ExHandler Wrap::Restore(const std::string& path, const std::string& filetype, const std::string& KBMsg, OnChangeCallback cb, bool HasMSGBox, WrapLoc *rest, ...) {
		redo:
		std::string realPath = path;

		std::vector<std::string> f_Dir, f_File, f_All;
		std::vector<bool> isDir;

		File file;

		Directory::Open(restoreDIR, path);

		if(restoreDIR.ListDirectories(f_Dir) == Directory::OPResult::NOT_OPEN) {
			return ExHandler::ERROR_UN;
		}

		if(restoreDIR.ListFiles(f_File, filetype) == Directory::OPResult::NOT_OPEN) {
			return ExHandler::ERROR_UN;
		}

	//error if no directory and files found
		if(f_Dir.empty() && f_File.empty()) {
			if(HasMSGBox) {
				MessageBox(Language::getInstance()->get("RESTORE_NOFILES")).SetClear(ClearScreen::Top)();
			}

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

		int uchoice = kb.Open();	
		if(uchoice < 0) {
			return ExHandler::ERROR_UN;
		}

	//if directory show all files in it
		if(isDir[uchoice]) {
		//Can't open directory
			if(Directory::Open(restoreDIR, Utils::Format("%s/%s", path.c_str(), f_All[uchoice].c_str())) != Directory::OPResult::SUCCESS) {
				return ExHandler::ERROR_UN;
			}

			if(restoreDIR.ListFiles(f_File, filetype) == Directory::OPResult::NOT_OPEN) {
				return ExHandler::ERROR_UN;
			}

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
			if(uchoice < 0) {
				goto redo;
			}
		}

		if(restoreDIR.OpenFile(file, f_All[uchoice] + filetype, File::RWC) != Directory::OPResult::SUCCESS) {
			if(HasMSGBox) {
				MessageBox(Language::getInstance()->get("RESTORE_ERROR2")).SetClear(ClearScreen::Top)();
			}

			return ExHandler::ERROR_OP; //error opening file
		}

		va_list restore;
		va_start(restore, rest);

		while(rest != nullptr) {
			if(file.Inject(*(u32 *)&rest->Address, rest->Lenght) != 0) {
				if(HasMSGBox) {
					MessageBox(Language::getInstance()->get("RESTORE_ERROR1")).SetClear(ClearScreen::Top)();
				}

				return ExHandler::ERROR_DRD; //error injecting file
			}

			rest = va_arg(restore, WrapLoc*); //go to next argument
		}
		va_end(restore);

		if(HasMSGBox) {
			MessageBox(Language::getInstance()->get("RESTORE_RESTORE")).SetClear(ClearScreen::Top)();
		}

		return ExHandler::SUCCESS; //success
	}

	ExHandler Wrap::Delete(const std::string& path, const std::string& filetype) { 
		std::vector<std::string> f_list;
		File file;
		Directory dir(path);

		if(dir.ListFiles(f_list, filetype) == Directory::OPResult::NOT_OPEN) {
			return ExHandler::ERROR_UN; //couldnt open directory
		}

		if(f_list.empty()) {
            MessageBox(Language::getInstance()->get("FILE_RES2")).SetClear(ClearScreen::Top)();
            return ExHandler::ERROR_LI; //error listing files | no files found
        }

		Keyboard kb("");
		kb.Populate(f_list);

		int uchoice = kb.Open();	
		if(uchoice == -1) {
			return ExHandler::ERROR_UN;
		}

		if(dir.OpenFile(file, f_list[uchoice], File::RWC) != 0) {
			MessageBox(Language::getInstance()->get("RESTORE_ERROR2")).SetClear(ClearScreen::Top)();
			return ExHandler::ERROR_OP; //error opening file
		}

		if((MessageBox(Utils::Format(Language::getInstance()->get("FILE_PROMPT").c_str(), f_list.at(uchoice).c_str()), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			File::Open(file, (path << std::string("/") << f_list.at(uchoice)), File::Mode::WRITE);
			file.Flush();
			file.Close();

			if(File::Remove(path << std::string("/") << f_list.at(uchoice)) != 0) {
				MessageBox(Language::getInstance()->get("FILE_RES3")).SetClear(ClearScreen::Top)();
				return ExHandler::ERROR_DRD; //error deleting file
			}
			MessageBox(Utils::Format(Language::getInstance()->get("FILE_RES1").c_str(), f_list.at(uchoice).c_str())).SetClear(ClearScreen::Top)();
		}
		dir.Close();
		return ExHandler::SUCCESS; //success
	}

//Calculations for branch code

	u32 Wrap::CalculateBranchInstruction(u32 PC, u32 target) {
		u32 instruction = ((target - PC) >> 2) - 2;

		if(PC > target) {
			instruction -= 0x3F000000;
		}

		return instruction;
	}

	u32 Wrap::GetBranchTarget(u32 PC, u32 instruction) {
		u32 target = instruction & 0xFFFFFF;
		
		target += 2;
		target = target << 2;
		target += PC;

		return (target & 0xFFFFFF);
	}
}
