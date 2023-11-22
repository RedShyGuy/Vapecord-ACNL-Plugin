#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFrameworkImpl/System/HookManager.hpp"
#include "CTRPluginFramework/System/Lock.hpp"

using namespace CTRPluginFramework;

HookContext&    HookContext::GetCurrent(void)
{
    return *reinterpret_cast<HookContext *>(
        reinterpret_cast<u32 *>(getThreadLocalStorage())[15]);
}

extern "C" void *__ctrpfHookCtx__GetCallerCode(HookContext *ctx)
{
    if (ctx->flags & MITM_MODE)
    {
        AsmWrapper& wrapper = HookManager::_singleton.GetAsmWrapper(ctx);

        return static_cast<void *>(&wrapper.code[6]);
    }

    return nullptr; ///< mitm mode only
}

void *  HookContext::GetCallCode(void)
{
    if (flags & MITM_MODE)
    {
        AsmWrapper& wrapper = HookManager::_singleton.GetAsmWrapper(this);

        return static_cast<void *>(&wrapper.code[6]);
    }

    return nullptr; ///< mitm mode only
}

Hook::Hook(void)
{
    _ctx = new HookContext;

    _ctx->refcount = 1;
    _ctx->flags = HOOK_DEFAULT_PARAMS;
    _ctx->targetAddress = 0;
    _ctx->returnAddress = 0;
    _ctx->callbackAddress = 0;
    _ctx->callbackAddress2 = 0;
    _ctx->overwrittenInstr = 0;
    _ctx->index = 0xFFFFFFFF;
}

Hook::Hook(const Hook &hook)
{
    _ctx = hook._ctx;
    AtomicIncrement(&_ctx->refcount);
}

Hook::Hook(Hook&& hook) noexcept
{
    _ctx = hook._ctx;
    hook._ctx = nullptr;
}

Hook& Hook::operator=(const Hook &hook)
{
    if (_ctx)
    {
        AtomicDecrement(&_ctx->refcount);
        if (_ctx->refcount == 0)
            delete _ctx;
    }
    _ctx = hook._ctx;
    AtomicIncrement(&_ctx->refcount);

    return *this;
}

Hook&   Hook::operator=(Hook &&hook) noexcept
{
    if (_ctx)
    {
        AtomicDecrement(&_ctx->refcount);
        if (_ctx->refcount == 0)
            delete _ctx;
    }
    _ctx = hook._ctx;
    hook._ctx = nullptr;

    return *this;
}

Hook::~Hook(void)
{
    if (!_ctx)
        return;

    AtomicDecrement(&_ctx->refcount);
    if (_ctx->refcount == 0)
        delete _ctx;
}

Hook&   Hook::Initialize(u32 targetAddr, u32 callbackAddr)
{
    if (!_ctx)
        goto exit;

    _ctx->targetAddress = targetAddr;
    _ctx->callbackAddress = callbackAddr;
    _ctx->returnAddress = targetAddr + 4;
exit:
    return *this;
}

Hook&   Hook::InitializeForMitm(u32 targetAddr, u32 callbackAddr)
{
    if (!_ctx)
        goto exit;

    _ctx->targetAddress = targetAddr;
    _ctx->callbackAddress = callbackAddr;
    _ctx->returnAddress = targetAddr + 4;
    _ctx->flags = MITM_MODE;
exit:

    return *this;
}

Hook&   Hook::InitializeForSubWrap(u32 targetAddr, u32 beforeCallback, u32 afterCallback)
{
    if (!_ctx)
        goto exit;

    _ctx->targetAddress = targetAddr;
    _ctx->callbackAddress = beforeCallback;
    _ctx->callbackAddress2 = afterCallback;
    _ctx->returnAddress = targetAddr + 4;
    _ctx->flags = WRAP_SUB;
exit:

    return *this;
}

Hook&   Hook::SetFlags(u32 flags)
{
    if (_ctx)
        _ctx->flags = flags;
    return *this;
}

Hook&   Hook::SetReturnAddress(u32 returnAddr)
{
    if (_ctx)
        _ctx->returnAddress = returnAddr;
    return *this;
}

bool    Hook::IsEnabled(void)
{
    return _ctx && _ctx->index != 0xFFFFFFFF;
}


static bool  IsTargetAlreadyHooked(u32 target, u32 instruction)
{
    if ((instruction >> 24) != 0xEA)
        return false;

    u32     minOffset = (0x1E80000 - (target + 8)) >> 2;
    u32     maxOffset = (0x1E81000 - (target + 8)) >> 2;
    u32     off = instruction & 0xFFFFFF;

    return off >= minOffset && off < maxOffset;
}

// Ensure that the instruction isn't a memory access depending on PC
static bool  IsInstructionPCDependant(u32 instruction)
{
    static const u32  forbiddenInstructions[] =
    {
        0xE59F, ///< ldr x, [pc, x]
        0xE58F, ///< str x, [pc, x]
        0xED9F, ///< vldr x, [pc, x] even register (s0, s2 etc)
        0xEDDF, ///< vldr x, [pc, x] odd register (s1, s3 etc)
        0xED8F, ///< vstr x, [pc, x] even register (s0, s2 etc)
        0xEDCF, ///< vstr x, [pc, x] odd register (s1, s3 etc)
    };

    instruction >>= 16;

    for (u32 forbiddenInstruction : forbiddenInstructions)
        if (instruction == forbiddenInstruction)
            return true;

    return false;
}

HookResult    Hook::Enable(void)
{
    if (!_ctx)
        return HookResult::InvalidContext;

    if (IsEnabled())
        return HookResult::Success;

    // Only 1 thread at a time please
    Lock    lock(HookManager::GetLock());

    u32 flags = _ctx->flags;

    // Check hook parameters
    if (flags & EXECUTE_OI_BEFORE_CB && flags & EXECUTE_OI_AFTER_CB)
        return HookResult::HookParamsError;

    // Check that the target is valid
    if (!Process::CheckAddress(_ctx->targetAddress))
        return HookResult::InvalidAddress;

    // Get the current instruction
    _ctx->overwrittenInstr = *reinterpret_cast<u32 *>(_ctx->targetAddress);

    // Check if the target is already hooked
    if (IsTargetAlreadyHooked(_ctx->targetAddress, _ctx->overwrittenInstr))
        return HookResult::AddressAlreadyHooked;

    // Check if the instruction is PC dependent
    if (flags & EXECUTE_OI_BEFORE_CB || flags & EXECUTE_OI_AFTER_CB)
        if (IsInstructionPCDependant(_ctx->overwrittenInstr))
            return HookResult::TargetInstructionCannotBeHandledAutomatically;

    // Check validity for WRAP_SUB mode
    if (flags & WRAP_SUB && _ctx->overwrittenInstr >> 24 != 0xEB)
        return HookResult::HookParamsError;

    // Apply the hook
    return HookManager::ApplyHook(*_ctx);
}

HookResult  Hook::Disable(void)
{
    if (!_ctx)
        return HookResult::InvalidContext;

    if (!IsEnabled())
        return HookResult::Success;

    return HookManager::DisableHook(*_ctx);
}

const HookContext&  Hook::GetContext(void) const
{
    return *_ctx;
}

extern "C"
{
    void * NAKED    CTRPluginFramework::ctrpfHook__GetCurrent(void)
    {
#ifndef _MSC_VER
    __asm__ __volatile__
    (
        "mrc    p15, 0, r0, c13, c0, 3  @ get tls \n"
        "ldr    r0, [r0, #0x3C]         @ get HookContext\n"
        "bx     lr                      \n"
    );
#endif
    }

    void * NAKED    CTRPluginFramework::ctrpfHook__ExecuteOriginalFunction(void)
    {
#ifndef _MSC_VER
    __asm__ __volatile__
    (
        "stmfd sp!, {r0-r3, lr, pc}     \n"
        "mrc    p15, 0, r0, c13, c0, 3  @ get tls \n"
        "ldr    r0, [r0, #0x3C]         @ get HookContext \n"
        "bl     __ctrpfHookCtx__GetCallerCode \n"
        "cmp   r0, #0                   \n"
        "strne r0, [sp, #20]            @ sp[pc] = callerCode\n"
        "ldreq r0, [sp, #16]            @ sp[pc] = sp[lr] if caller code is null \n"
        "streq r0, [sp, #20]            \n"
        "ldmfd sp!, {r0-r3, lr, pc}     \n"
    );
#endif
    }
}
