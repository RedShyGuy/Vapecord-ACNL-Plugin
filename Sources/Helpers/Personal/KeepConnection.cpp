#include <CTRPluginFramework.hpp>
#include "RegionCodes.hpp"

#define ONLINETHREADSAMOUNT 5
#define THREADVARS_MAGIC  0x21545624

std::tuple<u32, u32*> onlineThreadsInfo[ONLINETHREADSAMOUNT];

namespace CTRPluginFramework {
    void keepConnectionInCTRPF(bool runOnline) {
		static bool isRunOnline = false;

		if(isRunOnline == runOnline) 
			return;

		isRunOnline = runOnline;
		static u32 tlsBackup[ONLINETHREADSAMOUNT];
		static s32 prioBackup[ONLINETHREADSAMOUNT];

		for(int i = 0; i < ONLINETHREADSAMOUNT; i++) {
			u32 onlineThreadID = std::get<0>(onlineThreadsInfo[i]);
			u32* onlineThreadTls = std::get<1>(onlineThreadsInfo[i]);

			if(onlineThreadID == 0xFFFFFFFF) 
				continue;

			Handle onlineThreadHandle;
			Result res = svcOpenThread(&onlineThreadHandle, CUR_PROCESS_HANDLE, onlineThreadID);
			if(R_FAILED(res)) 
				continue;
			
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
        static Address origFunc(0x55D728, 0x55CC40, 0x55C770, 0x55C770, 0x55C060, 0x55C060, 0x55BD80, 0x55BD80);

        u32 obj_threadID = 0;
		u32* threadTls = nullptr;
		Result res = svcGetThreadId(&obj_threadID, CUR_THREAD_HANDLE);
		if(R_FAILED(res)) 
			obj_threadID = 0xFFFFFFFF;
		else 
			threadTls = (u32*)getThreadLocalStorage();

		onlineThreadsInfo[threadPos] = std::tuple<u32, u32*>(obj_threadID, threadTls);
		
		return origFunc.Call<u32>(param_1, param_2, param_3, param_4);
    }

    u32 getThread2Func(s8 threadPos, int param_1, u32 param_2, u32 param_3, u32 param_4) {
        static Address origFunc(0x53C04C, 0x53B9A0, 0x53B094, 0x53B094, 0x53A980, 0x53A980, 0x53A6A4, 0x53A6A4);

		u32 obj_threadID = 0;
		u32* threadTls = nullptr;
		Result res = svcGetThreadId(&obj_threadID, CUR_THREAD_HANDLE);
		if(R_FAILED(res)) 
			obj_threadID = 0xFFFFFFFF;
		else 
			threadTls = (u32*)getThreadLocalStorage();

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
		static u32 onlineThreadArgs[ONLINETHREADSAMOUNT] = {
			0x082C0FF8, //Region Free
			0x082C2DD0, //Region Free
			0x082D7588, //Region Free
			0x082E5AE0, //Region Free

        //TODO: Find Pointer for it as WL won't work with it
			Address(0x31F24028, 0x31EE1510, 0x31EFBC80, 0x31EE2788, 0x31F46AD0, 0x31F371F0, 0x31EF0310, 0x31EE22D0).addr //0x31EE2878 = WL
		};

        OSD::Notify(Utils::Format("%08X", threadargs-0x18));

		if(threadargs-0x18 == onlineThreadArgs[0])
			threadfunc = (u32)GetThreadInfo1;
		else if(threadargs-0x18 == onlineThreadArgs[1])
			threadfunc = (u32)GetThreadInfo2;
		else if(threadargs-0x18 == onlineThreadArgs[2])
			threadfunc = (u32)GetThreadInfo3;
		else if(threadargs-0x18 == onlineThreadArgs[3])
			threadfunc = (u32)GetThreadInfo4;
		else if(threadargs-0x18 == onlineThreadArgs[4])
			threadfunc = (u32)GetThreadInfo5;

		Address(startFunc).Call<void>(threadfunc, threadargs);
	}

//TODO: Fix up, port code and remove unnessecary code as some stuff is not needed
    void SendPlayerData(void) { //needs to be set into OnNewFrame callback
		int iVar1;
		int iVar2;
		u32 uVar3;

		*(u32 *)0x952680 += 1;
		Address(0x617D20).Call<void>(*(u32 *)0x954648);
		iVar2 = Address(0x5CF1AC).Call<int>();
		if(iVar2 == 0) 
			Address(0x1B7918).Call<void>(&*(u8 *)0xAD66E0);

		if(*(u8 *)(*(u32 *)0x94F3D8 + 0xFAE) == 0) {
			Address(0x52BE08).Call<void>(*(u32 *)0x95F0D4);
			Address(0x52BE08).Call<void>(*(u32 *)0x95F0D8);
			Address(0x52A9A8).Call<void>(*(u32 *)0x953648);
			Address(0x52A9A8).Call<void>(*(u32 *)0x95364C);
		}

		Address(0x6A6708).Call<void>();
		Address(0x1257BC).Call<void>(&*(u32 *)0xA23020);
		Address(0x51C86C).Call<void>(&*(u32 *)0xA837E8);

		if(*(u8 *)0x95D3F0 == 0) {
			uVar3 = Address(0x5204DC).Call<u32>();
			iVar2 = Address(0x520C98).Call<int>(uVar3, 2);
			if(iVar2 == 0) 
				Address(0x60758C).Call<void>();

			Address(0x618024).Call<void>(*(u32 *)0x954648);
		}
	}

    void InitKeepConnection(void) {
        static Address threadBeginAddress(0x12F3A8, 0x12E308, 0x12F3CC, 0x12F3CC, 0x12F394, 0x12F394, 0x12F394, 0x12F394);

        static Hook onlineThreadHook;
		onlineThreadHook.Initialize(threadBeginAddress.addr, (u32)PatchThreadBegin);
		onlineThreadHook.SetFlags(USE_LR_TO_RETURN);
		onlineThreadHook.Enable();
		
		Process::OnPauseResume = [](bool goingToPause) {
			keepConnectionInCTRPF(goingToPause);
		};
    }
}