#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"

/*
    Amiibo Spoofer 3.0 by Slattz
    For non-exclusive use by anyone who wants it.
*/

namespace CTRPluginFramework {
    s32 AmiiboChooser(void* arg) {
        u32 offset = *(u32*)arg;
        Process::Pause(); //Pause game while user selects, so the timeout doesn't occur

        Keyboard keyboard(Language->Get("AMIIBO_SPOOFER_SPECIES"));
        std::vector<std::string> keyVec;

        for(const char* specie : amiiboSpecies) 
            keyVec.push_back(std::string(specie));

        keyboard.Populate(keyVec);
        s8 res = keyboard.Open(); //Pick a species
        Sleep(Milliseconds(100));

        Process::Write32(offset + 0x10C, 0); //Game always sets this in the original function, so I'll do it too 
        if(res >= 0) { //User picked a species
            keyboard.GetMessage() = std::string(Language->Get("AMIIBO_SPOOFER_VILLAGER"));
            keyVec.clear();

            std::vector<PACKED_AmiiboInfo> amiiboVec;

            IDList::PopulateNPCAmiibo((SpecieID)res, keyVec, amiiboVec);

            keyboard.Populate(keyVec);
            res = keyboard.Open(); //Pick villager based on species
            Sleep(Milliseconds(100));

            if(res >= 0) { //User picked a specific villager
                Process::Write32(offset + 0xA8, 2); //"Successfully read tag" nfc flag
                Process::Write32(offset + 0xAC, 0); //success 3ds nfc result code

                const PACKED_AmiiboInfo& amiibo = amiiboVec[res];

                //Setup NFC Info into Amiibo buffer
                Process::Write32(offset + 0x38C, (amiibo.ID0 >> 8)); //amiibo ID (3 bytes, converted to 4)
                Process::Write8(offset + 0x390, (amiibo.ID0 & 0xFF)); //Tag type (1 byte)
                Process::Write8(offset + 0x391, ((amiibo.ID1 >> 8) & 0xFF)); //series ID (1 byte)
                Process::Write16(offset + 0x393, (amiibo.ID1 >> 16)); //Amiibo number (2 bytes)
            }

            amiiboVec.clear();
        }

        keyVec.clear();
        if(res < 0) { //User Aborted one of the keyboards
            Process::Write32(offset + 0xA8, 3); //"There was an error" nfc flag
            Process::Write32(offset + 0xAC, 0xC8A18600); //"There was an error" 3ds nfc result code
        }

        Process::Play(); //Resume game after user selects/cancels
        return 0;
    }

    void AmiiboHook(u32 offset) {
        Task task(AmiiboChooser, &offset); // Task needed here, otherwise Process::Pause breaks
        task.Start();
        task.Wait();
    }

    void AmiiboSpoofer(MenuEntry *entry) {
        static const u32 offsetPatch = Region::AutoRegion(0x51C104, 0x51BA58, 0x51B14C, 0x51B14C, 0x51AA68, 0x51AA68, 0x51A3FC, 0x51A3FC);
        static const u32 offsetHook = Region::AutoRegion(0x51BBDC, 0x51B530, 0x51AC24, 0x51AC24, 0x51A540, 0x51A540, 0x519ED4, 0x519ED4);
        static const u32 offsetPatch_o3ds1 = Region::AutoRegion(0x3DAA7C, 0x3DA48C, 0x3D9AC4, 0x3D9AC4, 0x3D975C, 0x3D975C, 0x3D961C, 0x3D961C);
        static const u32 offsetPatch_o3ds2 = Region::AutoRegion(0x3DA824, 0x3DA234, 0x3D986C, 0x3D986C, 0x3D9504, 0x3D9504, 0x3D93C4, 0x3D93C4);
        static u32 originalCode[3] = {0};
        static Hook nfcHook;

		if(entry->WasJustActivated()) {
			Process::Patch(offsetPatch, 0xE3A00003, (void*)&originalCode[0]); //Force tell nfc game code that there's an amiibo ready (NFC_TagState_InRange)

            if(!System::IsNew3DS()) {
                Process::Patch(offsetPatch_o3ds1, 0xE3A00002, (void*)&originalCode[1]); //Tells the o3ds the nfc reader is init'd
                Process::Patch(offsetPatch_o3ds2, 0xE3A00000, (void*)&originalCode[2]); //Forces the game to think nfc sysmodule has started scanning
            }

            nfcHook.Initialize(offsetHook, (u32)AmiiboHook);
            nfcHook.SetFlags(USE_LR_TO_RETURN);
            nfcHook.SetReturnAddress(offsetHook + 0x90); //skip over most the function as the scanning is ''done''
            nfcHook.Enable();
		}
		
		else if(!entry->IsActivated()) {
			Process::Patch(offsetPatch, originalCode[0]);

            if(!System::IsNew3DS()) {
                Process::Patch(offsetPatch_o3ds1, originalCode[1]);
                Process::Patch(offsetPatch_o3ds2, originalCode[2]);
            }

            nfcHook.Disable();
		}
    }
}
