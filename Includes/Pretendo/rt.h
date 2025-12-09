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

#ifndef _RT_H
#define _RT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct _RT_HOOK {
        u32 model;
        u32 isEnabled;
        u32 funcAddr;
        u32 bakCode[16];
        u32 jmpCode[16];
        u32 callCode[16];
    } RT_HOOK;
    void rtInitHook(RT_HOOK* hook, u32 funcAddr, u32 callbackAddr);
    void rtEnableHook(RT_HOOK* hook);
    void rtDisableHook(RT_HOOK* hook);
    u32 rtFlushInstructionCache(void* ptr, u32 size);
    u32 rtGenerateJumpCode(u32 dst, u32* buf);
#ifdef __cplusplus
}
#endif
#endif