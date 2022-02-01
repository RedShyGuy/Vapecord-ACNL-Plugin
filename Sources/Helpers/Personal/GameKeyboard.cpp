#include "Helpers/GameKeyboard.hpp"
#include "Helpers/Game.hpp"
#include "RegionCodes.hpp"

#define RANGE(X, START, END)	(X >= START && X <= END)

namespace CTRPluginFramework {
	bool GameKeyboard::Write(const std::string& str) {
		if(!GameKeyboard::IsOpen())
			return false;

		if(*(u32 *)Code::ChatPoint.addr == 0)
			return false;

		u16 buffer[str.size() + 1] = { 0 };
		utf8_to_utf16(buffer, reinterpret_cast<const u8*>(str.data()), str.size());

		static Address WriteFunc(0x5231D0, 0x522B24, 0x522218, 0x522218, 0x521B28, 0x521B28, 0x521514, 0x521514);	

		const u16* hex = (const u16 *)buffer;
		u8 i = *(u8 *)(*(u32 *)(Code::ChatPoint.addr) + 0x14);

		while(*hex) {
			u16 character = (u16)*hex++;
			WriteFunc.Call<void>(*(u32 *)Code::ChatPoint.addr, character, i, 0, 0);
			i++;
		}

		return true;
	}

	bool GameKeyboard::CopySelected(std::string& res) {
		if(!GameKeyboard::IsOpen())
			return false;

		bool IsSelected = *(bool *)(*(u32 *)(Code::ChatPoint.addr) + 0x20); //95F11C
		if(!IsSelected) 
			return false;

		u32	ChatText = *(u32 *)(*(u32 *)(Code::ChatPoint.addr) + 0x10);
		u8	CurrentPos = *(u8 *)(*(u32 *)(Code::ChatPoint.addr) + 0x14);
		u8	SelectStart = *(u8 *)(*(u32 *)(Code::ChatPoint.addr) + 0x1C);

		if(CurrentPos < SelectStart) 
			Process::ReadString(ChatText + (CurrentPos * 2), res, (SelectStart * 2) - (CurrentPos * 2), StringFormat::Utf16);

		if(CurrentPos > SelectStart) 
			Process::ReadString(ChatText + (SelectStart * 2), res, (CurrentPos * 2) - (SelectStart * 2), StringFormat::Utf16);

		return true;
	}

	bool GameKeyboard::DeleteSelected(void) {
		if(!GameKeyboard::IsOpen())
			return false;

		static Address DeleteFunc(0x523780, 0x5230D4, 0x5227C8, 0x5227C8, 0x5220D4, 0x5220D4, 0x521DCC, 0x521DCC);

		bool IsSelected = *(bool *)(*(u32 *)(Code::ChatPoint.addr) + 0x20);
		if(!IsSelected)
			return false;

		u8	CurrentPos = *(u8 *)(*(u32 *)(Code::ChatPoint.addr) + 0x14);
		u8	SelectStart = *(u8 *)(*(u32 *)(Code::ChatPoint.addr) + 0x1C);

		if(CurrentPos < SelectStart) {
			DeleteFunc.Call<void>(*(u32 *)Code::ChatPoint.addr, CurrentPos, SelectStart - CurrentPos);
		}
		if(CurrentPos > SelectStart) {
			DeleteFunc.Call<void>(*(u32 *)Code::ChatPoint.addr, SelectStart, CurrentPos - SelectStart);
		}

		*(bool *)(*(u32 *)(Code::ChatPoint.addr) + 0x20) = 0; //unselects
		return true;
	}

//If keyboard is opened 32DE75BC
	bool GameKeyboard::IsOpen() {
		static Address KeyBool(0x523F48, 0x52389C, 0x522F90, 0x522F90, 0x52287C, 0x52287C, 0x52256C, 0x52256C); 
		bool res = KeyBool.Call<bool>();
		if(res) {
			if(*(u32 *)(*(u32 *)(*(u32 *)(Code::ChatPoint.addr) + 4) + 0x50) == 0)
				return false;
		}
		return res;
	}

	bool GameKeyboard::IsEmpty() {
		if(!GameKeyboard::IsOpen())
			return true;

		return *(bool *)(*(u32 *)(*(u32 *)(Code::ChatPoint.addr) + 0x10) + 0x98 + 0x11B1) != true;
	}

	bool GameKeyboard::Copy(std::string& str, int pos, int lenght) {
		if(!GameKeyboard::IsOpen())
			return false;

		if(GameKeyboard::IsEmpty())
			return false;

		u32 ChatText = *(u32 *)(*(u32 *)(Code::ChatPoint.addr) + 0x10);

		return Process::ReadString(ChatText + pos, str, lenght, StringFormat::Utf16);
	}

	bool GameKeyboard::ConvertToItemID(std::string& str, Item &ItemID) {
		for(char& c : str)
			c = std::tolower(c);

		const u8* hex = (const u8*)str.c_str();
		while(*hex) {
			u8 byte = (u8)*hex++;

			if(RANGE(byte, '0', '9'))
				byte = byte - '0';
			else if(RANGE(byte, 'a','f'))
				byte = byte - 'a' + 10;
			else 
				return false; //Incorrect char

			*(u32 *)&ItemID = (*(u32 *)&ItemID << 4) | (byte & 0xF);
		}
		return true;
	}

	struct OnlineStack {
		u32** 	unknownPointer1; 	//address + 0x6C
		u32*  	unknownPointer2; 	//0x90B38C
		char*	message;			//string of message
	};

	void SetCustomOnlineStack(OnlineStack *stack, const std::string& str) {
		static const Address point(0x90B38C, 0x90A260, 0x90A20C, 0x90A20C, 0x904644, 0x903644, 0x9035B0, 0x9035B0);

		stack->unknownPointer1 = &stack->unknownPointer2;
		stack->unknownPointer2 = (u32 *)point.addr;

		std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
		stack->message = &cstr[0];
	}

	void GameKeyboard::SendMessage(const std::string& str) {
		static Address func2(0x56DE5C, 0x56D374, 0x56CEA4, 0x56CEA4, 0x56C794, 0x56C794, 0x56C4B4, 0x56C4B4);
		static Address func3(0x5FD774, 0x5FCCA4, 0x5FC7AC, 0x5FC7AC, 0x5FC02C, 0x5FC02C, 0x5FBCB4, 0x5FBCB4);
		static Address func4(0x5E3920, 0x5E2E50, 0x5E2958, 0x5E2958, 0x5E21D8, 0x5E21D8, 0x5E1E60, 0x5E1E60);
		static Address func5(0x300838, 0x300904, 0x3008C0, 0x3008C0, 0x300914, 0x300914, 0x300AF4, 0x300AF4);
		static Address func6(0x625C04, 0x62512C, 0x624C3C, 0x624C3C, 0x6246FC, 0x6246FC, 0x6242A4, 0x6242A4);

		u32 msgData = *(u32 *)Code::chatpointer.addr;
		if(msgData == 0)
			return;

		u8 pIndex = GameHelper::GetOnlinePlayerIndex();

		u32 Stack[12];
		OnlineStack *onlineStack = new OnlineStack();

		SetCustomOnlineStack(onlineStack, str);

		if(*(u8 *)(msgData + 0x858) != 0) {
			*(u16 *)(msgData + 0x854) = 0;
			*(u8 *)(msgData + 0x85A) = 0;
		}

		u32 var = Code::SetupStackData.Call<u32>(Stack); //Makes temporary "Stack" ready to be written to
		func2.Call<void>(var, pIndex); //Gets Player Name Data and writes it to temporary "Stack"

		func3.Call<void>((int *)(msgData + 0x47C), var); //Finished Player Name Data in Chat Box Data (?)

		Process::WriteString(msgData + 0x870, str, 0x30, StringFormat::Utf16);

		*(u8 *)(msgData + 0x84C) = 8;
		*(u8 *)(msgData + 0x850) = pIndex;

		func4.Call<void>(msgData + 0x6A0, msgData + 0x47C); //Actually writes Player Name Data to Chat Box Data

		*(u32 *)(msgData + 0x784) = (msgData + pIndex * 0x28 + 0x2AC);
		*(u16 *)(msgData + 0x856) = 300;
		*(u8 *)(msgData + 0x85A) = 1;
		*(u8 *)(msgData + 0x858) = 1;

		u32 val = func5.Call<u32>(); //Checks if playing online
		if(val != 0) 
			func6.Call<void>(0x8C + pIndex, onlineStack, 1); //Sends temporary Online "Stack" to others

		delete[] onlineStack;
	}
}