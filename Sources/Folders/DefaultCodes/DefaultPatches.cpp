#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/Wrapper.hpp"

namespace CTRPluginFramework {
    void OnlineDropLagRemover(MenuEntry *entry) {
        static const Address host1(0x5A1454);
		static const Address host2(0x5A12F4);
		static const Address host3(0x59FF5C);
		static const Address host4(0x59FD98);
		static const Address host5(0x5983F4);
		static const Address host6(0x5984DC);
		static const Address host7(0x59F8A0);
		static const Address host8(0x5A09B0);
		static const Address host9(0x5A0DF4);

		if(entry->WasJustActivated()) {
			Process::Patch(host1.addr, 0xE1A00000); //create locked spot pkt send
			Process::Patch(host2.addr, 0xE1A00000); //clear locked spot pkt send
			Process::Patch(host3.addr, 0xEA000010); //pkt is from host
			Process::Patch(host4.addr, 0xEA000003); //bypass check in 0x59FC7C
			Process::Patch(host5.addr, 0xE1A00000); //unnecessary?
			Process::Patch(host6.addr, 0xE1A00000); //unnecessary?
			Process::Patch(host7.addr, 0xE1A00000);
			Process::Patch(host8.addr, 0xEA000022); //item tree drop
			Process::Patch(host9.addr, 0xEA00002D); //fruit tree drop
		}
		else if(!entry->IsActivated()) {
			Process::Patch(host1.addr, 0x0A00002C);
			Process::Patch(host2.addr, 0x0A00002C);
			Process::Patch(host3.addr, 0x0A000010);
			Process::Patch(host4.addr, 0x0A000003);
			Process::Patch(host5.addr, 0x0A00001D);
			Process::Patch(host6.addr, 0x0A000025);
			Process::Patch(host7.addr, 0x0A00003B);
			Process::Patch(host8.addr, 0x0A000022);
			Process::Patch(host9.addr, 0x0A00002D);
		}
    }

    void ChangeRockbreakParticle(MenuEntry *entry) {
        static const Address prockc(0x5A2D20);

		if(entry->WasJustActivated()) {
			Process::Patch(prockc.addr, 0xF3);
		}
		else if(!entry->IsActivated()) {
			Process::Patch(prockc.addr, 0x1F6);
		}
    }

    void DropItemsEverywhere(MenuEntry *entry) {
        static const Address dever1(0x1655EC);
		static const Address dever2(0x1655F8);
		static const Address dever3(0x1654EC);
		static const Address dever4(0x165580);
		static const Address dever5(0x767028);

		if(entry->WasJustActivated()) {
			Process::Patch(dever1.addr, 0xE3A00001);
			Process::Patch(dever2.addr, 0xEA000006);
			Process::Patch(dever3.addr, 0xEA000005);
			Process::Patch(dever4.addr, 0xEA000010); //on other players
		}
		else if(!entry->IsActivated()) {
			u32 instruction = Wrap::CalculateBranchInstruction(dever1.addr, 0x767028);
			Process::Patch(dever1.addr, 0xEB000000 + instruction);
			Process::Patch(dever2.addr, 0x1A000005);
			Process::Patch(dever3.addr, 0x0A000005);
			Process::Patch(dever4.addr, 0x0A000010);
		}
    }

    void IdleAfterTreeShakeOrCut(MenuEntry *entry) {
        static const Address idts1(0x660600);
		static const Address idts2(0x662328);

		Process::Patch(idts1.addr, 0xE3A01006);
		Process::Patch(idts2.addr, 0xE3A01006);
    }

    void DontMoveNPCBackToOriginalPosition(MenuEntry *entry) {
        static const Address patchNPCMovingBack(0x57B9C0);

		Process::Patch(patchNPCMovingBack.addr, 0xE1A00000);
    }

	void ReplaceDropFunctions(MenuEntry *entry) {
		static const Address dadd(0x5A0F54); //0x5A0F54
		static const Address dval(0x5A1120); //0x5A1120

		for(int i = 0; i <= 0x21; ++i) 
			Process::Patch(dadd.addr + i * 4, dval.addr);
	}

	void PreventParticleCrash(MenuEntry *entry) {
		static const Address partc1(0x5506D4);
		static const Address partc2(0x5509CC);
		static const Address partc3(0x721418);

		//Process::Patch(partc1.addr, 0xE3A0C000);
		//Process::Patch(partc2.addr, 0xE3A0C000);
		Process::Patch(partc3.addr, 0xE3A0C000);
	}

	void BypassGameChecks(MenuEntry *entry) {
		static const Address byp1(0x1D43A4);
		static const Address byp2(0x1D43C0);
		static const Address byp3(0x1D43D0);
		static const Address byp4(0x759024);

		Process::Patch(byp1.addr, 0xE3A00001);
		Process::Patch(byp2.addr, 0xE3A00001);
		Process::Patch(byp3.addr, 0xE3A00001);
		Process::Patch(byp4.addr, 0xE1A00005);
	}

	void DisableNonSeedItemCheck(MenuEntry *entry) {
		static const Address NonSeed(0x76A894);

		Process::Patch(NonSeed.addr, 0xE3A00001);
	}

	void PatchDropFunction(MenuEntry *entry) {
		static const Address dropm(0x59FCA4);

		Process::Patch(dropm.addr, 0xEA000004);
	}
}