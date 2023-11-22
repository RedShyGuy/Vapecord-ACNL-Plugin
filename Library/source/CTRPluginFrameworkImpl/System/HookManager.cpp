#include "CTRPluginFrameworkImpl/System/HookManager.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "3ds.h"
#include "csvc.h"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "CTRPluginFramework/System/Lock.hpp"

using namespace CTRPluginFramework;

HookManager  HookManager::_singleton;

HookManager::HookManager(void)
{
    HookWrapper zero;
    std::fill(_hookWrappers, _hookWrappers + MAX_HOOKS, zero);
    _asmWrappers.reserve(MAX_HOOKS);
}
#define PA_PTR(addr)            (void *)((u32)(addr) | 1 << 31)
#define REG32(addr)             (*(vu32 *)(PA_PTR(addr)))

extern "C"  void    __hook__CheckTlsValue(u32 value)
{
    if (value && (value < 0x06000000 || value > 0x07000000))
    {
        // A little flash to notify the user that there's an error
        for (u32 i = 0; i < 64; i++)
        {
            REG32(0x10202204) = 0x01FF00FF;
            svcSleepThread(5000000);
        }
        REG32(0x10202204) = 0;

        const char * str = "Hi there! You found a game which do not match tls expected value.\n" \
                         "Contact Nanquitas on discord.gg/z4ZMh27 to fix this asap.\n" \
                         "Thanks a lot and sorry for the crash.";
        MessageBox("Error",  str)();
        svcBreak(USERBREAK_ASSERT);

    }
    return;
}

static NAKED void   SetHookContextInTLS(HookContext *ctx)
{
#ifndef _MSC_VER ///< Intellisense fix
    __asm__ __volatile__
    (
        "stmfd  sp!, {r4-r5, lr}        \n"
        "mov    r4, r0                  \n"
        "mrc    p15, 0, r5, c13, c0, 3  @ get tls \n"
        "add    r5, r5, #0x3C           @ r5 = &tls[15] \n"
        "ldr    r0, [r5]                \n"
        "bl     __hook__CheckTlsValue   @ Check tls value is expected \n"
        "str    r4, [r5]                @ tls[15] = ctx \n"
        "ldmfd  sp!, {r4-r5, pc}        \n"
    );
#endif
}

static inline u32 ARMBranchLink(const void *src, const void *dst)
{
    u32 instrBase = 0xEB000000;
    u32 off = (u32)((const u8 *)dst - ((const u8 *)src + 8));

    return instrBase | ((off >> 2) & 0xFFFFFF);
}

static inline u32 ARMBranch(const void *src, const void *dst)
{
    u32 instrBase = 0xEA000000;
    u32 off = (u32)((const u8 *)dst - ((const u8 *)src + 8));

    return instrBase | ((off >> 2) & 0xFFFFFF);
}

static inline u32 DecodeARMBranch(const vu32 *src)
{
    s32 off = (*src & 0xFFFFFF) << 2;
    off = (off << 6) >> 6; // sign extend

    return (u32)src + 8 + off;
}

static inline u32 ARM__LDR_LR_PC(u32 offset)
{
    return 0xE59FE000 | offset;
}

static inline u32 ARM__STR_LR_PC(u32 offset)
{
    return 0xE58FE000 | offset;
}

static __attribute__((noinline)) void     GenerateAsm(AsmWrapper& asmWrapper, HookContext& ctx)
{
    u32     flags = ctx.flags;
    vu32 *  code = &asmWrapper.code[0];
    vu32 *  ldrLrCb = nullptr;
    vu32 *  ldrLr;
    vu32 *  strLr;

    // Set common code
    *code++ = 0xE92D400F; ///< stmfd sp!, {r0-r3, lr}
    *code++ = 0xE51F0010; ///< ldr r0, [pc, #-16]
    *code++ = ARMBranchLink(&asmWrapper.code[2], (void *)SetHookContextInTLS); ///< bl SetHookContextInTLS
    *code++ = 0xE8BD400F; ///< ldmfd sp!, {r0-r3, lr}

    if (flags & MITM_MODE)
    {
        u32 op = ctx.overwrittenInstr >> 24;

        // Jump to callback
        *code++ = 0xE51FF004; ///< ldr pc, [pc, #-4]
        *code++ = ctx.callbackAddress;

        if (op == 0xEB) // If instruction is branch with link
            *code = ARMBranchLink((void *)code, (void *)DecodeARMBranch((vu32 *)ctx.targetAddress));
        else if (op == 0xEA) // If instruction is branch
            *code = ARMBranch((void *)code, (void *)DecodeARMBranch((vu32 *)ctx.targetAddress));
        else
            *code = ctx.overwrittenInstr;

        ++code;

        // Jump back to original function - code[7]
        *code++ = 0xE51FF004;///< ldr pc, [pc, #-4]
        *code = ctx.returnAddress;

        return;
    }

    if (flags & WRAP_SUB)
    {
        /*
         * ldr lr, [pc, #] @callback
         * blx lr
         * ldr lr, [pc, #] @target
         * blx lr
         * ldr lr, [pc, #] @afterCallback
         * blx lr
         * ldr pc, [pc, #-4]
         * @returnAddress
         * @callback
         * @target
         * @afterCallback
         */

        vu32 *  ldrLrCb2 = nullptr;
        vu32 *  ldrLrSub;

        // If there's a before callback
        if (ctx.callbackAddress)
        {
            ldrLrCb = code++;
            *code++ = 0xE12FFF3E; ///< blx lr
        }

        // Execute sub
        ldrLrSub = code++;
        *code++ = 0xE12FFF3E; ///< blx lr

        // If there's an after callback
        if (ctx.callbackAddress2)
        {
            ldrLrCb2 = code++;
            *code++ = 0xE12FFF3E; ///< blx lr
        }

        // Jump back to original function
        *code++ = 0xE51FF004; ///< ldr pc, [pc -4]
        *code++ = ctx.returnAddress;

        // Set callbacks addresses
        *code++ = ctx.callbackAddress;
        *code++ = DecodeARMBranch((vu32 *)ctx.targetAddress);
        *code = ctx.callbackAddress2;

        *ldrLrCb = ARM__LDR_LR_PC(u32(code) - u32(ldrLrCb) - 16);
        *ldrLrSub = ARM__LDR_LR_PC(u32(code) - u32(ldrLrSub) - 12);
        *ldrLrCb2 = ARM__LDR_LR_PC(u32(code) - u32(ldrLrCb2) - 8);

        return;
    }

    if (flags & USE_LR_TO_RETURN)
        strLr = code++;

    if (flags & EXECUTE_OI_BEFORE_CB)
        *code++ = ctx.overwrittenInstr;

    if (flags & USE_LR_TO_RETURN)
    {
        ldrLrCb = code++;
        //*code++ = 0xE59FE000; ///< ldr lr, [pc]
        *code++ = 0xE12FFF3E; ///< blx lr
        ldrLr = code++;
    }
    else
    {
        *code++ = 0xE51FF004; ///< ldr pc, [pc -4]
        *code++ = ctx.callbackAddress;
        return; ///< there's no no way to go there without assisted bx lr
    }

    if (flags & EXECUTE_OI_AFTER_CB)
        *code++ = ctx.overwrittenInstr;

    *code++ = 0xE51FF004; ///< ldr pc, [pc -4]
    *code++ = ctx.returnAddress;
    *code++ = ctx.callbackAddress;

    *strLr = ARM__STR_LR_PC(u32(code) - u32(strLr) - 8);
    *ldrLrCb = ARM__LDR_LR_PC(u32(code) - u32(ldrLrCb) - 12);
    *ldrLr = ARM__LDR_LR_PC(u32(code) - u32(ldrLr) - 8);
}

HookResult  HookManager::ApplyHook(HookContext &ctx)
{
    HookManager&    manager = _singleton;
    CTRPluginFramework::Lock            lock(manager._mutex);

    // Check if we have enough hooks unused
    if (manager._asmWrappers.size() >= MAX_HOOKS)
    {
        auto iter = manager._asmWrappers.begin();
        for (; iter != manager._asmWrappers.end(); ++iter)
            if (iter->ctx == nullptr)
                break;

        if (iter == manager._asmWrappers.end())
            return HookResult::TooManyHooks;
    }

    // Check if the target is already hooked
    for (const AsmWrapper& asmWrapper : manager._asmWrappers)
        if (asmWrapper.ctx && asmWrapper.ctx->targetAddress == ctx.targetAddress)
            return HookResult::AddressAlreadyHooked;

    // Everything is set, apply the hook
    s32             index;
    AsmWrapper&     asmWrapper =  manager.GetFreeAsmWrapper();
    HookWrapper&    hookWrapper = manager.GetFreeHookWrapper(index);

    // Generate asm based on hook params
    GenerateAsm(asmWrapper, ctx);

    // Update HookContext
    AtomicIncrement(&ctx.refcount);
    ctx.index = index;

    // Update AsmWrapper
    asmWrapper.ctx = &ctx;
    svcFlushProcessDataCache(Process::GetHandle(), (u32)&asmWrapper, sizeof(AsmWrapper));
    //svcFlushDataCacheRange(&asmWrapper, sizeof(AsmWrapper));

    // Update HookWrapper
    hookWrapper.jumpCode = 0xE51FF004; ///< ldr pc, [pc, #-4]
    hookWrapper.callback = (u32)&asmWrapper.code[0];
    hookWrapper.ctx = &ctx;
    svcFlushProcessDataCache(Process::GetHandle(), (u32)&hookWrapper, sizeof(HookWrapper));
    //svcFlushDataCacheRange(&hookWrapper, sizeof(HookWrapper));

    // Apply hook
    svcInvalidateEntireInstructionCache();
    *(vu32 *)ctx.targetAddress = ARMBranch((void *)ctx.targetAddress, &hookWrapper.jumpCode);
    svcFlushProcessDataCache(Process::GetHandle(), ctx.targetAddress, 4);
    //svcFlushDataCacheRange((void *)ctx.targetAddress, 4);

    return HookResult::Success;
}

HookResult    HookManager::DisableHook(HookContext &ctx)
{
    HookManager&    manager = _singleton;
    CTRPluginFramework::Lock            lock(manager._mutex);

    if (ctx.index > MAX_HOOKS)
        return HookResult::Success;

    HookWrapper&    hookWrapper = manager._hookWrappers[ctx.index];
    AsmWrapper&     asmWrapper = manager.GetAsmWrapper(hookWrapper.ctx);

    // First disable hook
    svcInvalidateEntireInstructionCache();
    *(vu32 *)ctx.targetAddress = ctx.overwrittenInstr;
    //svcFlushDataCacheRange((void *)ctx.targetAddress, 4);
    svcFlushProcessDataCache(Process::GetHandle(), ctx.targetAddress, 4);

    // Free HookWrapper
    hookWrapper.ctx = nullptr;

    // Free AsmWrapper
    asmWrapper.ctx = nullptr;

    // Update HookContext
    ctx.index = -1;
    AtomicDecrement(&ctx.refcount);

    if (ctx.refcount == 0)
        delete &ctx;

    return HookResult::Success;
}

AsmWrapper&    HookManager::GetFreeAsmWrapper(void)
{
    CTRPluginFramework::Lock    lock(_mutex);

    for (AsmWrapper& asmWrapper : _asmWrappers)
        if (asmWrapper.ctx == nullptr)
            return asmWrapper;

    _asmWrappers.emplace_back();
    return _asmWrappers.back();
}

HookWrapper&    HookManager::GetFreeHookWrapper(s32& index)
{
    CTRPluginFramework::Lock    lock(_mutex);

    HookWrapper *begin = _hookWrappers;
    HookWrapper *end = begin + MAX_HOOKS;

    index = 0;
    for (; begin != end; ++begin, ++index)
        if (begin->ctx == nullptr)
            return *begin;

    // We didn't found a free hook
    svcBreak(USERBREAK_ASSERT);
    return *begin;
}

AsmWrapper&     HookManager::GetAsmWrapper(HookContext *ctx)
{
    CTRPluginFramework::Lock    lock(_mutex);

    for (AsmWrapper& asmWrapper : _asmWrappers)
        if (asmWrapper.ctx == ctx)
            return asmWrapper;

    // We didn't found the specified AsmWrapper
    svcBreak(USERBREAK_ASSERT);
    return _asmWrappers.back();
}

void HookManager::Lock()
{
    _singleton._mutex.Lock();
}

void HookManager::Unlock()
{
    _singleton._mutex.Unlock();
}

void HookManager::PrepareToUnmapMemory()
{
    CTRPluginFramework::Lock lock(_singleton._mutex);

    HookWrapper* begin = _singleton._hookWrappers;
    HookWrapper* end = begin + MAX_HOOKS;

    svcInvalidateEntireInstructionCache();

    for (; begin != end; ++begin) {
        if (!begin->ctx) continue;
        HookContext& ctx = *begin->ctx;
        *(vu32*)ctx.targetAddress = ctx.overwrittenInstr;
        svcFlushProcessDataCache(Process::GetHandle(), ctx.targetAddress, 4);
    }
}

void HookManager::RecoverFromUnmapMemory()
{
    CTRPluginFramework::Lock lock(_singleton._mutex);

    HookWrapper* begin = _singleton._hookWrappers;
    HookWrapper* end = begin + MAX_HOOKS;

    svcInvalidateEntireInstructionCache();

    for (; begin != end; ++begin) {
        if (!begin->ctx) continue;
        HookContext& ctx = *begin->ctx;
        *(vu32*)ctx.targetAddress = ARMBranch((void*)ctx.targetAddress, &begin->jumpCode);
        svcFlushProcessDataCache(Process::GetHandle(), ctx.targetAddress, 4);
    }
}
