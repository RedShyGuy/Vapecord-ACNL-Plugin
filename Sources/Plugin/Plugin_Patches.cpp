#include <CTRPluginFramework.hpp>
#include <csvc.h>
#include <3ds.h>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "cheats.hpp"
#include "RegionCodes.hpp"

namespace CTRPluginFramework {
//This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
    static void ToggleTouchscreenForceOn(void) {
        static u32 original = 0;
        static u32 *patchAddress = nullptr;

        if(patchAddress && original) {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern = {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Result  res;
        Handle  processHandle;
        s64     textTotalSize = 0;
        s64     startAddress = 0;
        u32 *   found;

        if(R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if(R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
            goto exit;

        found = (u32 *)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

        if(found != nullptr) {
            original = found[13];
            patchAddress = (u32 *)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000;
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
		exit:
        svcCloseHandle(processHandle);
    }
	
//FwkSettings Patch Process/Gets called even if the game is not supported
	void PatchProcess(FwkSettings &settings) {
		ToggleTouchscreenForceOn();
		settings.ThreadPriority = 0x30;
	}

//RAM Changes Only
	void RCO(void) {
		ItemSequence::Init();
	
		AccidentalCrashCheck();
		
	//Online Drop Lag Remover
		static const u32 host1 = Region::AutoRegion(0x5A1454, 0x5A096C, 0x5A049C, 0x5A049C, 0x59FD8C, 0x59FD8C, 0x59FA60, 0x59FA60);
		static const u32 host2 = Region::AutoRegion(0x5A12F4, 0x5A080C, 0x5A033C, 0x5A033C, 0x59FC2C, 0x59FC2C, 0x59F900, 0x59F900);
		static const u32 host3 = Region::AutoRegion(0x59FF5C, 0x59F474, 0x59EFA4, 0x59EFA4, 0x59E894, 0x59E894, 0x59E568, 0x59E568);
		static const u32 host4 = Region::AutoRegion(0x59FD98, 0x59F2B0, 0x59EDE0, 0x59EDE0, 0x59E6D0, 0x59E6D0, 0x59E3A4, 0x59E3A4);
		static const u32 host5 = Region::AutoRegion(0x5983F4, 0x59790C, 0x59743C, 0x59743C, 0x596D2C, 0x596D2C, 0x596A00, 0x596A00);
		static const u32 host6 = Region::AutoRegion(0x5984DC, 0x5979F4, 0x597524, 0x597524, 0x596E14, 0x596E14, 0x596AE8, 0x596AE8);
		static const u32 host7 = Region::AutoRegion(0x59F8A0, 0x59EDB8, 0x59E8E8, 0x59E8E8, 0x59E1D8, 0x59E1D8, 0x59DEAC, 0x59DEAC);
		static const u32 host8 = Region::AutoRegion(0x5A09B0, 0x59FEC8, 0x59F9F8, 0x59F9F8, 0x59F2E8, 0x59F2E8, 0x59EFBC, 0x59EFBC);
		static const u32 host9 = Region::AutoRegion(0x5A0DF4, 0x5A030C, 0x59FE3C, 0x59FE3C, 0x59F72C, 0x59F72C, 0x59F400, 0x59F400);
		Process::Patch(host1, 0xE1A00000); //create locked spot pkt send
		Process::Patch(host2, 0xE1A00000); //clear locked spot pkt send
		Process::Patch(host3, 0xEA000010); //pkt is from host
		Process::Patch(host4, 0xEA000003); //bypass check in 0x59FC7C
		Process::Patch(host5, 0xE1A00000); //unnecessary?
		Process::Patch(host6, 0xE1A00000); //unnecessary?
		Process::Patch(host7, 0xE1A00000);
		Process::Patch(host8, 0xEA000022); //item tree drop
		Process::Patch(host9, 0xEA00002D); //fruit tree drop
		
	//Drop Related Stuff
		static const u32 dadd = Region::AutoRegion(0x5A0F54, 0x5A046C, 0x59FF9C, 0x59FF9C, 0x59F88C, 0x59F88C, 0x59F560, 0x59F560);
		static const u32 dval = Region::AutoRegion(0x5A1120, 0x5A0638, 0x5A0168, 0x5A0168, 0x59FA58, 0x59FA58, 0x59F72C, 0x59F72C);
		for(int i = 0; i <= 0x21; ++i) 
			Process::Patch(dadd + i * 4, dval);
			
	//Drop Related Stuff
		static const u32 dropm = Region::AutoRegion(0x59FCA4, 0x59F1BC, 0x59ECEC, 0x59ECEC, 0x59E5DC, 0x59E5DC, 0x59E2B0, 0x59E2B0);
		Process::Patch(dropm, 0xEA000004);
			
	//Change Rockbreak Particle
		static const u32 prockc = Region::AutoRegion(0x5A2D20, 0x5A2238, 0x5A1D68, 0x5A1D68, 0x5A1658, 0x5A1658, 0x5A132C, 0x5A132C);
		Process::Patch(prockc, 0xF3);
	
	//Drop Items Everywhere
		static const u32 dever1 = Region::AutoRegion(0x1655EC, 0x165034, 0x16560C, 0x16560C, 0x1655D4, 0x1655D4, 0x1655D4, 0x1655D4);
		static const u32 dever2 = Region::AutoRegion(0x1655F8, 0x165040, 0x165618, 0x165618, 0x1655E0, 0x1655E0, 0x1655E0, 0x1655E0);
		static const u32 dever3 = Region::AutoRegion(0x1654EC, 0x164F34, 0x16550C, 0x16550C, 0x1654D4, 0x1654D4, 0x1654D4, 0x1654D4);
		static const u32 dever4 = Region::AutoRegion(0x165580, 0x164FC8, 0x1655A0, 0x1655A0, 0x165568, 0x165568, 0x165568, 0x165568);
		Process::Patch(dever1, 0xE3A00001);
		Process::Patch(dever2, 0xEA000006);
		Process::Patch(dever3, 0xEA000005);
		Process::Patch(dever4, 0xEA000010); //on other players
		
	//Idle After Tree Shake/Cut
		static const u32 idts1 = Region::AutoRegion(0x660600, 0x65FB28, 0x65F638, 0x65F638, 0x65F0F8, 0x65F0F8, 0x65ECA0, 0x65ECA0);
		static const u32 idts2 = Region::AutoRegion(0x662328, 0x661850, 0x661360, 0x661360, 0x660E20, 0x660E20, 0x6609C8, 0x6609C8);
		Process::Patch(idts1, 0xE3A01006);
		Process::Patch(idts2, 0xE3A01006);
		
	//Prevent Particle Crash
		static const u32 partc1 = Region::AutoRegion(0x5506D4, 0x54FBEC, 0x54F71C, 0x54F71C, 0x54F008, 0x54F008, 0x54ED2C, 0x54ED2C);
		static const u32 partc2 = Region::AutoRegion(0x5509CC, 0x54FEE4, 0x54FA14, 0x54FA14, 0x54F300, 0x54F300, 0x54F024, 0x54F024);
		static const u32 partc3 = Region::AutoRegion(0x721418, 0x720760, 0x720420, 0x7203F8, 0x71FBB4, 0x71FB8C, 0x71F75C, 0x71F75C);
		Process::Patch(partc1, 0xE3A0C000);
		Process::Patch(partc2, 0xE3A0C000);
		Process::Patch(partc3, 0xE3A0C000);
		
	//Bypass Checks
		static const u32 byp1 = Region::AutoRegion(0x1D43A4, 0x1D3DE8, 0x1D43C4, 0x1D43C4, 0x1D4300, 0x1D4300, 0x1D42CC, 0x1D42CC);
		static const u32 byp2 = Region::AutoRegion(0x1D43C0, 0x1D3E04, 0x1D43E0, 0x1D43E0, 0x1D431C, 0x1D431C, 0x1D42E8, 0x1D42E8);
		static const u32 byp3 = Region::AutoRegion(0x1D43D0, 0x1D3E14, 0x1D43F0, 0x1D43F0, 0x1D432C, 0x1D432C, 0x1D42F8, 0x1D42F8);
		static const u32 byp4 = Region::AutoRegion(0x759024, 0x758008, 0x75802C, 0x758004, 0x7577C4, 0x75779C, 0x75736C, 0x75736C);
		Process::Patch(byp1, 0xE3A00001);
		Process::Patch(byp2, 0xE3A00001);
		Process::Patch(byp3, 0xE3A00001);
		Process::Patch(byp4, 0xE1A00005);
	
	//check for non seed item
	    static const u32 NonSeed = Region::AutoRegion(0x76A894, 0x769878, 0x76989C, 0x769874, 0x769034, 0x76900C, 0x768BDC, 0x768BB4);
		Process::Patch(NonSeed, 0xE3A00001);

	//patches npc from moving back to their fixed position
		static const u32 patchNPCMovingBack = Region::AutoRegion(0x57B9C0, 0x57AED8, 0x57AA08, 0x57AA08, 0x57A2F8, 0x57A2F8, 0x57A018, 0x57A018);
		Process::Patch(patchNPCMovingBack, 0xE1A00000);
	}
}