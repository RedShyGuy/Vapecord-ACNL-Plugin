#include <CTRPluginFramework.hpp>
#include <csvc.h>
#include "Helpers/Dropper.hpp"
#include "Helpers/Player.hpp"
#include "RuntimeContext.hpp"
#include "cheats.hpp"

namespace CTRPluginFramework {
//This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
    static void    ToggleTouchscreenForceOn(void)
    {
        static u32 original = 0;
        static u32 *patchAddress = nullptr;

        if (patchAddress && original)
        {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern =
        {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Result  res;
        Handle  processHandle;
        s64     textTotalRoundedSize = 0;
        s64     startAddress = 0;
        u32 *   found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalRoundedSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if(R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalRoundedSize, static_cast<MapExFlags>(0))))
            goto exit;

        found = (u32 *)Utils::Search<u32>(0x14000000, (u32)textTotalRoundedSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32 *)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000;
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalRoundedSize);
	exit:
        svcCloseHandle(processHandle);
    }

//FwkSettings Patch Process/Gets called even if the game is not supported
	void PatchProcess(FwkSettings &settings) {
		ToggleTouchscreenForceOn();
		settings.ThreadPriority = 0x30;

		settings.CachedDrawMode = true;
	}

    void OnExitProcess(void) {
        ToggleTouchscreenForceOn();

        DisableAllChecks();
		DisableAllPatches();
    }

//check for indoor items	
	void IndoorsSeedItemCheck(void) {
        RuntimeContext::getInstance()->setIndoors(Player::IsIndoors());

		if(!DropPatternON || !RuntimeContext::getInstance()->isIndoors()) {
            return;
        }
		
		Dropper::RestorePattern();
		DropPatternON = false;
		OSD::Notify("Drop Pattern Restored!", Color::Orange);
	}	

}