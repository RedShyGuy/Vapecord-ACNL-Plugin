#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/Wrapper.hpp"
#include <functional>

namespace CTRPluginFramework {
	void ToggleWithOptionKeyboard(const std::function<void(bool)>& fn) {
        std::vector<std::string> cmnOpt =  {
			Language::getInstance()->get("VECTOR_ENABLE"),
			Language::getInstance()->get("VECTOR_DISABLE")
		};

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), cmnOpt);
        int res = optKb.Open();
		if (res < 0) {
			return;
		}

		fn(res == 0); // 0 = enable, 1 = disable
    }

    void OnlineDropLagRemover(bool enable) {
        static Address host1(0x5A1454);
		static Address host2(0x5A12F4);
		static Address host3(0x59FF5C);
		static Address host4(0x59FD98);
		static Address host5(0x5983F4);
		static Address host6(0x5984DC);
		static Address host7(0x59F8A0);
		static Address host8(0x5A09B0);
		static Address host9(0x5A0DF4);

		if(enable) {
			host1.Patch(0xE1A00000); //create locked spot pkt send
			host2.Patch(0xE1A00000); //clear locked spot pkt send
			host3.Patch(0xEA000010); //pkt is from host
			host4.Patch(0xEA000003); //bypass check in 0x59FC7C
			host5.Patch(0xE1A00000); //unnecessary?
			host6.Patch(0xE1A00000); //unnecessary?
			host7.Patch(0xE1A00000);
			host8.Patch(0xEA000022); //item tree drop
			host9.Patch(0xEA00002D); //fruit tree drop
		}
		else {
			host1.Unpatch();
			host2.Unpatch();
			host3.Unpatch();
			host4.Unpatch();
			host5.Unpatch();
			host6.Unpatch();
			host7.Unpatch();
			host8.Unpatch();
			host9.Unpatch();
		}
    }

    void ChangeRockbreakParticle(bool enable) {
        static Address prockc(0x5A2D20);

		if(enable) {
			prockc.Patch(0xF3);
		}
		else {
			prockc.Unpatch();
		}
    }

    void DropItemsEverywhere(bool enable) {
        static Address dever1(0x1655EC);
		static Address dever2(0x1655F8);
		static Address dever3(0x1654EC);
		static Address dever4(0x165580);

		if(enable) {
			dever1.Patch(0xE3A00001);
			dever2.Patch(0xEA000006);
			dever3.Patch(0xEA000005);
			dever4.Patch(0xEA000010); //on other players
		}
		else {
			dever1.Unpatch();
			dever2.Unpatch();
			dever3.Unpatch();
			dever4.Unpatch();
		}
    }

    void IdleAfterTreeShakeOrCut(bool enable) {
        static Address idts1(0x660600);
		static Address idts2(0x662328);

		if(enable) {
			idts1.Patch(0xE3A01006);
			idts2.Patch(0xE3A01006);
		}
		else {
			idts1.Unpatch();
			idts2.Unpatch();
		}
    }

    void DontMoveNPCBackToOriginalPosition(bool enable) {
        static Address patchNPCMovingBack(0x57B9C0);

		if(enable) {
			patchNPCMovingBack.Patch(0xE1A00000);
		}
		else {
			patchNPCMovingBack.Unpatch();
		}
    }

	void ReplaceDropFunctions(bool enable) {
		static Address dadd(0x5A0F54); //0x5A0F54
		static Address dval(0x5A1120); //0x5A1120

		static u32 origVal[0x22];

		if(enable) {
			for(int i = 0; i <= 0x21; ++i) {
				Process::Patch(dadd.addr + i * 4, dval.addr, &origVal[i]);
			}
		}
		else {
			for(int i = 0; i <= 0x21; ++i) {
				Process::Patch(dadd.addr + i * 4, origVal[i]);
			}
		}
	}

	void PreventParticleCrash(bool enable) {
		//static Address partc1(0x5506D4);
		//static Address partc2(0x5509CC);
		static Address partc3(0x721418);

		if(enable) {
			//partc1.Patch(0xE3A0C000);
			//partc2.Patch(0xE3A0C000);
			partc3.Patch(0xE3A0C000);
		}
		else {
			//partc1.Unpatch();
			//partc2.Unpatch();
			partc3.Unpatch();
		}
	}

	void BypassGameChecks(bool enable) {
		static Address byp1(0x1D43A4);
		static Address byp2(0x1D43C0);
		static Address byp3(0x1D43D0);
		static Address byp4(0x759024);

		if(enable) {
			byp1.Patch(0xE3A00001);
			byp2.Patch(0xE3A00001);
			byp3.Patch(0xE3A00001);
			byp4.Patch(0xE1A00005);
		}
		else {
			byp1.Unpatch();
			byp2.Unpatch();
			byp3.Unpatch();
			byp4.Unpatch();
		}
	}

	void DisableNonSeedItemCheck(bool enable) {
		static Address NonSeed(0x76A894);

		if(enable) {
			NonSeed.Patch(0xE3A00001);
		}
		else {
			NonSeed.Unpatch();
		}
	}

	void PatchDropFunction(bool enable) {
		static Address dropm(0x59FCA4);

		if(enable) {
			dropm.Patch(0xEA000004);
		}
		else {
			dropm.Unpatch();
		}
	}

	void EnableAllPatches() {
		OnlineDropLagRemover(true);
		ChangeRockbreakParticle(true);
		DropItemsEverywhere(true);
		IdleAfterTreeShakeOrCut(true);
		DontMoveNPCBackToOriginalPosition(true);
		ReplaceDropFunctions(true);
		PreventParticleCrash(true);
		BypassGameChecks(true);
		DisableNonSeedItemCheck(true);
		PatchDropFunction(true);
	}

	void DisableAllPatches() {
		OnlineDropLagRemover(false);
		ChangeRockbreakParticle(false);
		DropItemsEverywhere(false);
		IdleAfterTreeShakeOrCut(false);
		DontMoveNPCBackToOriginalPosition(false);
		ReplaceDropFunctions(false);
		PreventParticleCrash(false);
		BypassGameChecks(false);
		DisableNonSeedItemCheck(false);
		PatchDropFunction(false);
	}

	void OnlineDropLagRemoverEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(OnlineDropLagRemover);
	}

	void ChangeRockbreakParticleEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(ChangeRockbreakParticle);
	}

	void DropItemsEverywhereEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(DropItemsEverywhere);
	}

	void IdleAfterTreeShakeOrCutEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(IdleAfterTreeShakeOrCut);
	}

	void DontMoveNPCBackToOriginalPositionEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(DontMoveNPCBackToOriginalPosition);
	}

	void ReplaceDropFunctionsEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(ReplaceDropFunctions);
	}

	void PreventParticleCrashEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(PreventParticleCrash);
	}

	void BypassGameChecksEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(BypassGameChecks);
	}

	void DisableNonSeedItemCheckEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(DisableNonSeedItemCheck);
	}

	void PatchDropFunctionEntry(MenuEntry *entry) {
		ToggleWithOptionKeyboard(PatchDropFunction);
	}
}