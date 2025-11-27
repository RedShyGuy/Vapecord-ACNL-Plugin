#include "cheats.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/NPC.hpp"
#include "Address/Address.hpp"

/*
    Amiibo Spoofer 3.0 by Slattz
    For non-exclusive use by anyone who wants it.
*/

namespace CTRPluginFramework {
    s32 AmiiboChooser(void* arg) {
        u32 offset = *(u32*)arg;
        Process::Pause(); //Pause game while user selects, so the timeout doesn't occur

        Keyboard keyboard(Language::getInstance()->get("AMIIBO_SPOOFER_SPECIES"));
        std::vector<std::string> keyVec;

        NPC::PopulateRace(keyVec);

        keyboard.Populate(keyVec);

        int res = keyboard.Open(); //Pick a species

        Process::Write32(offset + 0x10C, 0); //Game always sets this in the original function, so I'll do it too 
        if(res >= 0) { //User picked a species
            keyboard.GetMessage() = std::string(Language::getInstance()->get("AMIIBO_SPOOFER_VILLAGER"));
            keyVec.clear();

            std::vector<PACKED_AmiiboInfo> amiiboVec;

            NPC::PopulateAmiibo((SpecieID)res, keyVec, amiiboVec);

            keyboard.Populate(keyVec);

            res = keyboard.Open(); //Pick villager based on species

            if(res >= 0) { //User picked a specific villager
                Process::Write32(offset + 0xA8, 2); //"Successfully read tag" nfc flag
                Process::Write32(offset + 0xAC, 0); //success 3ds nfc result code

                const PACKED_AmiiboInfo& amiibo = amiiboVec[res];

                //Setup NFC Info into Amiibo buffer
                Process::Write32(offset + 0x38C, (amiibo.ID0 >> 8)); //amiibo ID (3 bytes, converted to 4)
                Process::Write8(offset + 0x390, (amiibo.ID0 & 0xFF)); //Tag type (1 byte)
                Process::Write8(offset + 0x391, ((amiibo.ID1 >> 8) & 0xFF)); //series ID (1 byte)
                Process::Write16(offset + 0x392, (amiibo.ID1 >> 16)); //Amiibo number (2 bytes)
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
        static Address offsetHook(0x51BBDC);

        static Address offsetPatch(0x51C104);
        static Address offsetPatch_o3ds1(0x3DAA7C);
        static Address offsetPatch_o3ds2(0x3DA824);
        static Hook nfcHook;

		if(entry->WasJustActivated()) {
            offsetPatch.Patch(0xE3A00003); //Force tell nfc game code that there's an amiibo ready (NFC_TagState_InRange)

            if(!System::IsNew3DS()) {
                offsetPatch_o3ds1.Patch(0xE3A00002); //Tells the o3ds the nfc reader is init'd
                offsetPatch_o3ds2.Patch(0xE3A00000); //Forces the game to think nfc sysmodule has started scanning
            }

            nfcHook.Initialize(offsetHook.addr, (u32)AmiiboHook);
            nfcHook.SetFlags(USE_LR_TO_RETURN);
            nfcHook.SetReturnAddress(offsetHook.addr + 0x90); //skip over most the function as the scanning is ''done''
            nfcHook.Enable();
		}
		
		else if(!entry->IsActivated()) {
			offsetPatch.Unpatch();

            if(!System::IsNew3DS()) {
                offsetPatch_o3ds1.Unpatch();
                offsetPatch_o3ds2.Unpatch();
            }

            nfcHook.Disable();
		}
    }
}
