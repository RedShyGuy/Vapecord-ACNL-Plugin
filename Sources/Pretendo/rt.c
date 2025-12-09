/*
 * This file is part of Nimbus.
* https://github.com/PretendoNetwork/nimbus
 *
 * Copyright (C) 2025 YourName + Nimbus contributors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "3ds.h"
#include "Pretendo/rt.h"
#include <string.h>

Handle hCurrentProcess = 0;
u32 currentPid = 0;

u32 getCurrentProcessId() {
	svcGetProcessId(&currentPid, 0xffff8001);
	return currentPid;
}

u32 getCurrentProcessHandle() {
	u32 handle = 0;
	u32 ret;

	if (hCurrentProcess != 0) {
		return hCurrentProcess;
	}

	svcGetProcessId(&currentPid, 0xffff8001);
	ret = svcOpenProcess(&handle, currentPid);
	if (ret != 0) {
		return 0;
	}
	hCurrentProcess = handle;
	return hCurrentProcess;
}

u32 rtGetPageOfAddress(u32 addr) {
	return (addr / 0x1000) * 0x1000;
}

u32 rtFlushInstructionCache(void* ptr, u32 size) {
	return svcFlushProcessDataCache(getCurrentProcessHandle(), (u32)ptr, size);
}

u32 rtGenerateJumpCode(u32 dst, u32* buf) {
	buf[0] = 0xe51ff004;
	buf[1] = dst;
	return 8;
}

void rtInitHook(RT_HOOK* hook, u32 funcAddr, u32 callbackAddr) {
	hook->model = 0;
	hook->isEnabled = 0;
	hook->funcAddr = funcAddr;

	memcpy(hook->bakCode, (void*) funcAddr, 8);
	rtGenerateJumpCode(callbackAddr, hook->jmpCode);
	memcpy(hook->callCode, (void*) funcAddr, 8);
	rtGenerateJumpCode(funcAddr + 8, &(hook->callCode[2]));
	rtFlushInstructionCache(hook->callCode, 16);
}


void rtEnableHook(RT_HOOK* hook) {
	if (hook->isEnabled) {
		return;
	}
	u32* dst = (u32*)hook->funcAddr;
	dst[0] = hook->jmpCode[0];
	dst[1] = hook->jmpCode[1];
	rtFlushInstructionCache((void*) hook->funcAddr, 8);
	hook->isEnabled = 1;
}

void rtDisableHook(RT_HOOK* hook) {
	if (!hook->isEnabled) {
		return;
	}
	u32* dst = (u32*)hook->funcAddr;
	dst[0] = hook->bakCode[0];
	dst[1] = hook->bakCode[1];
	rtFlushInstructionCache((void*) hook->funcAddr, 8);
	hook->isEnabled = 0;
}