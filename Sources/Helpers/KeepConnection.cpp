#include <CTRPluginFramework.hpp>

#include "Helpers/Game.hpp"
#include "Address/Address.hpp"

/*
Thanks to PabloMK7 for the huge help with the thread loading!
*/

#define ONLINETHREADSAMOUNT 5
#define THREADVARS_MAGIC  0x21545624

std::tuple<u32, u32*> onlineThreadsInfo[ONLINETHREADSAMOUNT];

namespace CTRPluginFramework {
    void keepConnectionInCTRPF(bool runOnline) {
		static bool isRunOnline = false;

		if(isRunOnline == runOnline) {
			return;
		}

		isRunOnline = runOnline;
		static u32 tlsBackup[ONLINETHREADSAMOUNT];
		static s32 prioBackup[ONLINETHREADSAMOUNT];

		for(int i = 0; i < ONLINETHREADSAMOUNT; i++) {
			u32 onlineThreadID = std::get<0>(onlineThreadsInfo[i]);
			u32* onlineThreadTls = std::get<1>(onlineThreadsInfo[i]);

			if(onlineThreadID == 0xFFFFFFFF) {
				continue;
			}

			Handle onlineThreadHandle;
			Result res = svcOpenThread(&onlineThreadHandle, CUR_PROCESS_HANDLE, onlineThreadID);
			if(R_FAILED(res)) {
				return;
			}
			
			if(runOnline) {
				tlsBackup[i] = *onlineThreadTls;
				*onlineThreadTls = THREADVARS_MAGIC;
				svcGetThreadPriority(&prioBackup[i], onlineThreadHandle);
				svcSetThreadPriority(onlineThreadHandle, FwkSettings::Get().ThreadPriority - 1);
			}
			else {
				*onlineThreadTls = tlsBackup[i];
				svcSetThreadPriority(onlineThreadHandle, prioBackup[i]);
			}

			svcCloseHandle(onlineThreadHandle);
		}
	}

    u32 getThread1Func(s8 threadPos, int param_1, u32 param_2, u32 param_3, u32 param_4) {
        static Address origFunc(0x55D728);

        u32 obj_threadID = 0;
		u32* threadTls = nullptr;
		Result res = svcGetThreadId(&obj_threadID, CUR_THREAD_HANDLE);
		if(R_FAILED(res)) {
			obj_threadID = 0xFFFFFFFF;
		}
		else {
			threadTls = (u32*)getThreadLocalStorage();
		}

		onlineThreadsInfo[threadPos] = std::tuple<u32, u32*>(obj_threadID, threadTls);
		
		return origFunc.Call<u32>(param_1, param_2, param_3, param_4);
    }

    u32 getThread2Func(s8 threadPos, int param_1, u32 param_2, u32 param_3, u32 param_4) {
        static Address origFunc(0x53C04C);

		u32 obj_threadID = 0;
		u32* threadTls = nullptr;
		Result res = svcGetThreadId(&obj_threadID, CUR_THREAD_HANDLE);
		if(R_FAILED(res)) {
			obj_threadID = 0xFFFFFFFF;
		}
		else {
			threadTls = (u32*)getThreadLocalStorage();
		}
			
		onlineThreadsInfo[threadPos] = std::tuple<u32, u32*>(obj_threadID, threadTls);
		
		return origFunc.Call<u32>(param_1, param_2, param_3, param_4);
    }

	u32 GetThreadInfo1(int param_1, u32 param_2, u32 param_3, u32 param_4) {
		return getThread1Func(0, param_1, param_2, param_3, param_4);
	}

	u32 GetThreadInfo2(int param_1, u32 param_2, u32 param_3, u32 param_4) {
		return getThread2Func(1, param_1, param_2, param_3, param_4);
	}

	u32 GetThreadInfo3(int param_1, u32 param_2, u32 param_3, u32 param_4) {
		return getThread2Func(2, param_1, param_2, param_3, param_4);
	}

	u32 GetThreadInfo4(int param_1, u32 param_2, u32 param_3, u32 param_4) {
		return getThread2Func(3, param_1, param_2, param_3, param_4);
	}

	u32 GetThreadInfo5(int param_1, u32 param_2, u32 param_3, u32 param_4) {
		return getThread1Func(4, param_1, param_2, param_3, param_4);
	}

	void PatchThreadBegin(u32 threadfunc, u32 threadargs, u32 startFunc, u32 u0) {
		static Address point(0x953CA0);
		static Address calc(0x2C);

		static u32 threadAddress = *(u32 *)(*(u32 *)(point.addr) + 0xA8 + 0x80) - calc.addr;

		static u32 onlineThreadArgs[ONLINETHREADSAMOUNT] = {
			0x82C0FF8, //Region Free
			0x82C2DD0, //Region Free
			0x82D7588, //Region Free
			0x82E5AE0, //Region Free
			threadAddress
		};

		if(threadargs-0x18 == onlineThreadArgs[0]) {
			threadfunc = (u32)GetThreadInfo1;
		}
		else if(threadargs-0x18 == onlineThreadArgs[1]) {
			threadfunc = (u32)GetThreadInfo2;
		}
		else if(threadargs-0x18 == onlineThreadArgs[2]) {
			threadfunc = (u32)GetThreadInfo3;
		}
		else if(threadargs-0x18 == onlineThreadArgs[3]) {
			threadfunc = (u32)GetThreadInfo4;
		}
		else if(threadargs-0x18 == onlineThreadArgs[4]) {
			threadfunc = (u32)GetThreadInfo5;
		}

		Address(startFunc).Call<void>(threadfunc, threadargs);
	}

    void SendPlayerData/*0x1B6C28*/(Time time) { //needs to be set into OnNewFrame callback
		if(Game::GetOnlinePlayerCount() <= 1 || PluginMenu::GetRunningInstance() == nullptr) {
			return;
		}

		//0x1B6C28.Call<void>();
		//return;

		static Address sendData1(0x617D20);
		static Address sendData2(0x60758C);
		static Address sendData3(0x618024);

		static Address getData1(0x5204DC);
		static Address getData2(0x520C98);

		sendData1.Call<void>(*(u32 *)Address(0x954648).addr);

		if(*(u8 *)(Address(0x95D3F4).addr-4) == 0) {
			u32 uVar3 = getData1.Call<u32>();
			int iVar2 = getData2.Call<int>(uVar3, 2);
			if(iVar2 == 0) {
				sendData2.Call<void>();
			}

			sendData3.Call<void>(*(u32 *)Address(0x954648).addr);
		}
	}

    void InitKeepConnection(void) {
        static Address threadBeginAddress(0x12F3A8);

        static Hook onlineThreadHook;
		onlineThreadHook.Initialize(threadBeginAddress.addr, (u32)PatchThreadBegin);
		onlineThreadHook.SetFlags(USE_LR_TO_RETURN);
		onlineThreadHook.Enable();
		
		Process::OnPauseResume = [](bool goingToPause) {
			keepConnectionInCTRPF(goingToPause);
		};
    }
}