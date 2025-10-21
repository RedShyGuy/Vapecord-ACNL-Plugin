#include <CTRPluginFramework.hpp>
#include <csvc.h>
#include "Address/Address.hpp"
#include "Helpers/ItemSequence.hpp"

namespace CTRPluginFramework {
	void InitCustomPresentOpeningFunction(); 

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


	void PluginHooks(void);

//FwkSettings Patch Process/Gets called even if the game is not supported
	void PatchProcess(FwkSettings &settings) {
		ToggleTouchscreenForceOn();
		settings.ThreadPriority = 0x30;

		settings.CachedDrawMode = true;
	}

//RAM Changes Only
	void RCO(void) {
		ItemSequence::Init();
	
		PluginHooks();

		InitCustomPresentOpeningFunction();
		
	//Online Drop Lag Remover
		static const Address host1("HOST1");
		static const Address host2("HOST2");
		static const Address host3("HOST3");
		static const Address host4("HOST4");
		static const Address host5("HOST5");
		static const Address host6("HOST6");
		static const Address host7("HOST7");
		static const Address host8("HOST8");
		static const Address host9("HOST9");
		Process::Patch(host1.addr, 0xE1A00000); //create locked spot pkt send
		Process::Patch(host2.addr, 0xE1A00000); //clear locked spot pkt send
		Process::Patch(host3.addr, 0xEA000010); //pkt is from host
		Process::Patch(host4.addr, 0xEA000003); //bypass check in 0x59FC7C
		Process::Patch(host5.addr, 0xE1A00000); //unnecessary?
		Process::Patch(host6.addr, 0xE1A00000); //unnecessary?
		Process::Patch(host7.addr, 0xE1A00000);
		Process::Patch(host8.addr, 0xEA000022); //item tree drop
		Process::Patch(host9.addr, 0xEA00002D); //fruit tree drop
		
	//Drop Related Stuff
		static const Address dadd("DADD");
		static const Address dval("DVAL");
		for(int i = 0; i <= 0x21; ++i) 
			Process::Patch(dadd.addr + i * 4, dval.addr);
			
	//Drop Related Stuff
		static const Address dropm("DROPM");
		Process::Patch(dropm.addr, 0xEA000004);
			
	//Change Rockbreak Particle
		static const Address prockc("PROCKC");
		Process::Patch(prockc.addr, 0xF3);
	
	//Drop Items Everywhere
		static const Address dever1("DEVER1");
		static const Address dever2("DEVER2");
		static const Address dever3("DEVER3");
		static const Address dever4("DEVER4");
		Process::Patch(dever1.addr, 0xE3A00001);
		Process::Patch(dever2.addr, 0xEA000006);
		Process::Patch(dever3.addr, 0xEA000005);
		Process::Patch(dever4.addr, 0xEA000010); //on other players
		
	//Idle After Tree Shake/Cut
		static const Address idts1("IDTS1");
		static const Address idts2("IDTS2");
		Process::Patch(idts1.addr, 0xE3A01006);
		Process::Patch(idts2.addr, 0xE3A01006);
		
	//Prevent Particle Crash
		static const Address partc1("PARTC1");
		static const Address partc2("PARTC2");
		static const Address partc3("PARTC3");
		//Process::Patch(partc1.addr, 0xE3A0C000);
		//Process::Patch(partc2.addr, 0xE3A0C000);
		Process::Patch(partc3.addr, 0xE3A0C000);
		
	//Bypass Checks
		static const Address byp1("BYP1");
		static const Address byp2("BYP2");
		static const Address byp3("BYP3");
		static const Address byp4("BYP4");
		Process::Patch(byp1.addr, 0xE3A00001);
		Process::Patch(byp2.addr, 0xE3A00001);
		Process::Patch(byp3.addr, 0xE3A00001);
		Process::Patch(byp4.addr, 0xE1A00005);
	
	//check for non seed item
	    static const Address NonSeed("NONSEED");
		Process::Patch(NonSeed.addr, 0xE3A00001);

	//patches npc from moving back to their fixed position
		static const Address patchNPCMovingBack("PATCHNPCMOVINGBACK");
		Process::Patch(patchNPCMovingBack.addr, 0xE1A00000);
	}
}