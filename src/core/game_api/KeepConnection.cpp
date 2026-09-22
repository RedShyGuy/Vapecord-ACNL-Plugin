#include <CTRPluginFramework.hpp>
#include "core/HUD.hpp"
#include "core/game_api/Game.hpp"
#include "core/infrastructure/Address.hpp"
#include "core/infrastructure/Language.hpp"

/*
Thanks to PabloMK7 for the huge help with the thread loading!
*/

#define ONLINETHREADSAMOUNT 5
#define THREADVARS_MAGIC  0x21545624

std::tuple<u32, u32*> onlineThreadsInfo[ONLINETHREADSAMOUNT];

namespace CTRPluginFramework {
	static bool s_keepConnectionEnabled = false;

	static Hook onlineThreadHook;
	static Hook userJoinsHook;
	static bool s_hooksInitialized = false;

	static void InitKeepConnection(void);
	void keepConnectionInCTRPF(bool runOnline);

	void SetKeepConnectionEnabled(bool enabled) {
		s_keepConnectionEnabled = enabled;

		if(enabled) {
			if(!s_hooksInitialized) {
				InitKeepConnection();
			}
			else {
				onlineThreadHook.Enable();
				userJoinsHook.Enable();
			}

			Process::OnPauseResume = [](bool goingToPause) {
				keepConnectionInCTRPF(goingToPause);
			};
		}
		else {
			if(s_hooksInitialized) {
				onlineThreadHook.Disable();
				userJoinsHook.Disable();
			}

			Process::OnPauseResume = nullptr;
		}
	}

	bool IsKeepConnectionEnabled(void) {
		return s_keepConnectionEnabled;
	}

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

		u8 fixOffset = 0x30; //For USAWA, EURWA, JPN, JPNWA, KORWA
		if (Address::IsRegion(Address::Region::USA) || Address::IsRegion(Address::Region::EUR)
			|| Address::IsRegion(Address::Region::KOR)) {
			fixOffset = 0x2C;
		}

		static u32 threadAddress = *(u32 *)(*(u32 *)(point.addr) + 0xA8 + 0x80) - fixOffset;

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

	bool userWantsToLeave() {
		static Address usersWhoWantToLeave(0x94EE0C);
		return *(u8 *)usersWhoWantToLeave.addr > 0;
	}

	//0x1B6C28
    void SendPlayerData(Time time) { //needs to be set into OnNewFrame callback
		if(!s_keepConnectionEnabled) {
			return;
		}

		{
			if (userWantsToLeave()) {
				PluginMenu *menu = PluginMenu::GetRunningInstance();
				if (menu != nullptr) {
					if (menu->IsOpen()) {
						HUD::Notify(Language::getInstance()->get(TextID::KEEP_CONN_PLAYER_LEAVING), Color::Purple);
						menu->ForceClose();

						static Address usersWhoWantToLeave(0x94EE0C);
						usersWhoWantToLeave.Write<u8>(0); //might cause issues like that, but without it, it could cause a infinite loop
						return;
					}
				}
			}
		}
		
		if(Game::GetOnlinePlayerCount() <= 1 || PluginMenu::GetRunningInstance() == nullptr) {
			return;
		}

		static Address netGameMgrHandleReceiveData(0x617D20);
		static Address netGameMgrs_pInstance(0x954648);
		static Address netGameMgrProcess(0x618024);

		static Address s_SkipProcesses = Address(0x95D3F4).MoveOffset(-4);

		u32 pInstance = *(u32 *)netGameMgrs_pInstance.addr;
		bool skipProcesses = *(bool *)(s_SkipProcesses.addr);

		netGameMgrHandleReceiveData.Call<void>(pInstance);

		if(!skipProcesses) {
			netGameMgrProcess.Call<void>(pInstance);
		}
	}

	/*
	Thanks to nico for this research info:

	User Joins: 0x62b2bc

	0x94ee0c bit field of users who want to leave
	0x94ee0b user who is allowed to leave next
	Both gets set in 0x32d2c4 and used in 0x28335c

	users who leave with talking to the npc calls function 0x282f8c

	TODO:
	show chat messages in plugin while paused
	*/

	//0x62b2bc
	void UserJoinsHook(u32 u0, u32 *u1, u32 u2, u32 u3, u32 u4) {
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if (menu != nullptr) {
			if (menu->IsOpen()) {
				HUD::Notify(Language::getInstance()->get(TextID::KEEP_CONN_PLAYER_JOINING), Color::Purple);
				menu->ForceClose();
			}
		}

		HookContext& ctx = HookContext::GetCurrent();
		ctx.OriginalFunction<void, u32, u32*, u32, u32, u32>(u0, u1, u2, u3, u4);
	}

    static void InitKeepConnection(void) {
        static Address threadBeginAddress(0x12F3A8);
		static Address userJoinsAddress(0x62B2BC);

		onlineThreadHook.Initialize(threadBeginAddress.addr, (u32)PatchThreadBegin);
		onlineThreadHook.SetFlags(USE_LR_TO_RETURN);
		onlineThreadHook.Enable();

		userJoinsHook.InitializeForMitm(userJoinsAddress.addr, (u32)UserJoinsHook);
		userJoinsHook.Enable();

		s_hooksInitialized = true;
		
		Process::OnPauseResume = [](bool goingToPause) {
			keepConnectionInCTRPF(goingToPause);
		};
    }
}