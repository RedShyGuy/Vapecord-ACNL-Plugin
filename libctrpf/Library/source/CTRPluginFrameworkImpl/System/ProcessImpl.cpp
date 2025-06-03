#include <3ds.h>

// Fix std::vector<MemInfo> == operator
/*static bool      operator==(const MemInfo left, const MemInfo right)
{
    return left.base_addr == right.base_addr && left.size == right.size;
}*/

#include "CTRPluginFramework/System.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Graphics/OSDImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"

#include <cstdio>
#include <cstring>
#include "csvc.h"

extern      Handle gspThreadEventHandle;

namespace CTRPluginFramework
{
    using namespace CTRPluginFrameworkImpl::Services;

    Handle      ProcessImpl::ProcessHandle = 0;
    u32         ProcessImpl::IsPaused = 0;
    u32         ProcessImpl::Status = Running;
    u32         ProcessImpl::ProcessId = 0;
    u64         ProcessImpl::TitleId = 0;

    KThread *   ProcessImpl::MainThread;
    KProcess *  ProcessImpl::KProcessPtr;
    KCodeSet    ProcessImpl::CodeSet;
    u32         ProcessImpl::MainThreadTls;
    MemInfo     ProcessImpl::InvalidRegion = MemInfo{0, 0, 0, 0};
    Mutex       ProcessImpl::MemoryMutex;
    std::vector<MemInfo> ProcessImpl::MemRegions;
    LightEvent  ProcessImpl::waitForExitEvent;
    u32         ProcessImpl::exceptionCount = 0;

    Process::ProcessEventCallback ProcessImpl::processEventCallback = nullptr;

    void    ProcessImpl::Initialize(void)
    {
        char    kproc[0x100] = {0};
        bool    isNew3DS = System::IsNew3DS();

        if (SystemImpl::IsCitra) {
            MainThread = new KThread();
            KProcessPtr = new KProcess();

            s64 out;
            u32 out32;
            svcGetThreadId(&out32, CUR_THREAD_HANDLE);
            MainThread->threadId = out32;
            svcGetProcessId(&ProcessId, CUR_PROCESS_HANDLE);

            MainThread->tls = MainThreadTls;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10000);
            memcpy(CodeSet.processName, &out, 8);

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10001);
            CodeSet.titleId = out;
            TitleId = out;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10002);
            CodeSet.text.totalPages = out >> 12;
            CodeSet.textPages = out >> 12;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10003);
            CodeSet.rodata.totalPages = out >> 12;
            CodeSet.rodataPages = out >> 12;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10004);
            CodeSet.data.totalPages = out >> 12;
            CodeSet.rwPages = out >> 12;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10005);
            CodeSet.text.startAddress = out;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10006);
            CodeSet.rodata.startAddress = out;

            svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10007);
            CodeSet.data.startAddress = out;

            // Create handle for this process
            ProcessHandle = CUR_PROCESS_HANDLE;
        } else {
            // Get current KProcess
            KProcessPtr = KProcess::GetCurrent();
            KProcessPtr->PatchMaxThreads();

            // Copy KProcess data
            Kernel::Memcpy(kproc, KProcessPtr, 0x100);

            if (isNew3DS)
            {
                // Copy KCodeSet
                Kernel::Memcpy(&CodeSet, (void *)*(u32 *)(kproc + 0xB8), sizeof(KCodeSet));

                // Copy process id
                ProcessId = *(u32 *)(kproc + 0xBC);

                // Get main thread
                MainThread = (KThread *)*(u32 *)(kproc + 0xC8);

                // Patch KProcess to allow creating threads on Core2
                KProcessPtr->PatchCore2Access();
            }
            else
            {
                // Copy KCodeSet
                Kernel::Memcpy(&CodeSet, (void *)*(u32 *)(kproc + 0xB0), sizeof(KCodeSet));

                // Copy process id
                ProcessId = *(u32 *)(kproc + 0xB4);

                // Get main thread
                MainThread = (KThread *)*(u32 *)(kproc + 0xC0);
            }

            // Copy title id
            TitleId = CodeSet.titleId;

            // Create handle for this process
            svcOpenProcess(&ProcessHandle, ProcessId);
        }
        UpdateMemRegions();

        // Init wait for exit event
        LightEvent_Init(&waitForExitEvent, RESET_ONESHOT);
    }

    extern "C" Handle gspEvent;
    extern "C" bool   IsPaused(void)
    {
        return (ProcessImpl::IsPaused > 0);
    }

    void    ProcessImpl::Pause(bool useFading)
    {
        // Increase pause counter
        ++IsPaused;

        // If game is already paused, nothing to do
        if (IsPaused > 1)
            return;

        Status |= Paused;
        if (Process::OnPauseResume) Process::OnPauseResume(true);

        // Wait for the frame to be paused
        OSDImpl::WaitFramePaused();

        // Acquire screens
        // TODO: error handling
        ScreenImpl::AcquireFromGsp();

        OSDImpl::UpdateScreens();

        // Update memregions
        UpdateMemRegions();
    }

    void    ProcessImpl::Play(bool forced)
    {
        // If game isn't paused, abort
        if (!IsPaused)
            return;

        // Decrease pause counter
        --IsPaused;

        // Force play (reset counter) if requested
        if (forced)
            IsPaused = 0;

        // Resume frame
        if (!IsPaused)
        {
            Status &= ~Paused;
            if (Process::OnPauseResume) Process::OnPauseResume(false);
            ScreenImpl::Top->Release();
            ScreenImpl::Bottom->Release();
            OSDImpl::ResumeFrame();
        }
    }

    bool     ProcessImpl::PatchProcess(u32 addr, u8 *patch, u32 length, u8 *original)
    {
        if (original != nullptr)
        {
            if (!Process::CopyMemory((void *)original, (void *)addr, length))
                goto error;
        }

        if (!Process::CopyMemory((void *)addr, (void *)patch, length))
            goto error;

        return (true);
    error:
        return (false);
    }

    void    ProcessImpl::GetHandleTable(KProcessHandleTable& table, std::vector<HandleDescriptor>& handleDescriptors)
    {
        bool    isNew3DS = System::IsNew3DS();

        // Copy KProcessHandleTable
        Kernel::Memcpy(&table, (void *)((u32)KProcessPtr + (isNew3DS ? 0xDC : 0xD4)), sizeof(KProcessHandleTable));

        u32 count = table.handlesCount;

        handleDescriptors.resize(count);
        Kernel::Memcpy(handleDescriptors.data(), table.handleTable, count * sizeof(HandleDescriptor));
    }

    void    ProcessImpl::GetGameThreads(std::vector<KThread *> &threads)
    {
        threads.clear();

        KProcessHandleTable             table;
        std::vector<HandleDescriptor>   handles;

        GetHandleTable(table, handles);

        threads.push_back(MainThread);

        for (HandleDescriptor &handle : handles)
        {
            if (!(handle.obj->GetType() == KType::KThread))
                continue;

            KThread *thread = reinterpret_cast<KThread *>(handle.obj);

            if (!thread->IsPluginThread())
                threads.push_back(thread);
        }
    }

    #define THREADVARS_MAGIC  0x21545624 // !TV$

    // Ensure that only game threads are locked
    static bool    ThreadPredicate(KThread *thread)
    {
        u32 *tls = (u32 *)thread->tls;
        KThread* currentThread = *(KThread**)0xFFFF9000;

        if (currentThread != thread && *tls != THREADVARS_MAGIC)
            return true;
        return false;
    }

    void    ProcessImpl::LockGameThreads(void)
    {
        if (SystemImpl::IsCitra)
            svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_SCHEDULE_THREADS_WITHOUT_TLS_MAGIC, 1, THREADVARS_MAGIC);
        else
            svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_SCHEDULE_THREADS, 1, (u32)ThreadPredicate);
    }

    void    ProcessImpl::UnlockGameThreads(void)
    {
        if (SystemImpl::IsCitra)
            svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_SCHEDULE_THREADS_WITHOUT_TLS_MAGIC, 0, THREADVARS_MAGIC);
        else
            svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_SCHEDULE_THREADS, 0, (u32)ThreadPredicate);
    }

    static bool     IsInRegion(MemInfo &memInfo, u32 addr)
    {
        addr -= memInfo.base_addr;
        return addr < memInfo.size;
    }

    extern "C" u32 __ctru_heap;

    void    ProcessImpl::UpdateMemRegions(bool ignoreLock)
    {
        if (!ignoreLock) MemoryMutex.Lock();

        MemRegions.clear();

        //bool    regionPatched  = false;

        for (u32 addr = 0x00100000; addr < 0x40000000; )
        {
            MemInfo     memInfo;
            PageInfo    pageInfo;

            if (R_SUCCEEDED(svcQueryProcessMemory(&memInfo, &pageInfo, ProcessHandle, addr)))
            {
                // If region is FREE, IO, SHARED or LOCKED, skip it
                if (memInfo.state == MEMSTATE_FREE || memInfo.state == MEMSTATE_IO)
                   // || memInfo.state == MEMSTATE_LOCKED || memInfo.state == MEMSTATE_SHARED)
                {
                    addr = memInfo.base_addr + memInfo.size;
                    continue;
                }

                // Same if the memregion is part of CTRPF or NTR
                /*if (memInfo.base_addr == 0x06000000 || memInfo.base_addr == 0x07000000
                    || memInfo.base_addr == 0x01E80000 || IsInRegion(memInfo, __ctru_heap))
                {
                    addr = memInfo.base_addr + memInfo.size;
                    continue;
                }*/

                // Add it to the vector if necessary
                if (memInfo.perm & MEMPERM_READ)
                    MemRegions.push_back(memInfo);

                addr = memInfo.base_addr + memInfo.size;
                continue;
            }

            addr += 0x1000;
        }
        if (!ignoreLock) MemoryMutex.Unlock();
    }

    static bool     IsRegionProhibited(const MemInfo& memInfo)
    {
        // Yes if the memregion is part of CTRPF
        if (memInfo.base_addr == 0x06000000 || memInfo.base_addr == 0x07000000
            || memInfo.base_addr == 0x01E80000)
            return true;

        // Same if the memory is shared
        if (memInfo.state == MEMSTATE_SHARED)
            return true;

        return false;
    }

    bool        ProcessImpl::IsValidAddress(const u32 address)
    {
        Lock lock(MemoryMutex);

        for (MemInfo &memInfo : MemRegions)
            if (IsInRegion(memInfo, address))
                return true;

        return false;
    }

    u32        ProcessImpl::GetPAFromVA(const u32 address)
    {
        u32 pa = 0;

        svcControlProcess(ProcessImpl::ProcessHandle, PROCESSOP_GET_PA_FROM_VA, (u32)&pa, address);

        return pa;
    }

    MemInfo     ProcessImpl::GetMemRegion(u32 address)
    {
        Lock lock(MemoryMutex);

        for (MemInfo &memInfo : MemRegions)
            if (IsInRegion(memInfo, address))
                return memInfo;

        // Not found return an empty region
        return ProcessImpl::InvalidRegion;
    }

    MemInfo     ProcessImpl::GetNextRegion(const MemInfo &region)
    {
        Lock lock(MemoryMutex);

        for (MemInfo &memInfo : MemRegions)
            if (memInfo > region && !IsRegionProhibited(memInfo))
                return memInfo;

        return region;
    }

    MemInfo     ProcessImpl::GetPreviousRegion(const MemInfo &region)
    {
        Lock lock(MemoryMutex);

        MemInfo *prev = nullptr;

        for (MemInfo &memInfo : MemRegions)
        {
            if (memInfo >= region && !IsRegionProhibited(memInfo))
                return prev != nullptr ? *prev : memInfo;

            prev = &memInfo;
        }

        return region;
    }

    void        ProcessImpl::GetFreeBlocks(std::vector<MemInfo> &blocks)
    {
        Lock lock(MemoryMutex);

        blocks.clear();

        for (u32 addr = 0x00100000; addr < 0x40000000; )
        {
            MemInfo     memInfo;
            PageInfo    pageInfo;

            if (R_SUCCEEDED(svcQueryProcessMemory(&memInfo, &pageInfo, ProcessHandle, addr)))
            {
                // If region is FREE, add it to the vector
                if (memInfo.state == MEMSTATE_FREE)
                {
                    blocks.push_back(memInfo);
                }

                addr = memInfo.base_addr + memInfo.size;
                continue;
            }

            addr += 0x1000;
        }
    }

    u32         ProcessImpl::GetFreeMemRegion(const u32 size, const u32 searchStart)
    {
        Lock lock(MemoryMutex);
        std::vector<MemInfo> freeBlocks;
        GetFreeBlocks(freeBlocks);

        // Round up to closest page size
        u32 realSize = (size & ~0xFFF) + ((size & 0xFFF) ? 0x1000 : 0);

        // Best fit algorithm
        MemInfo bestFit;
        bestFit.base_addr = 0;
        bestFit.size = 0xFFFFFFFF;

        for (auto block : freeBlocks)
        {
            if (block.size <= 0x1000 || searchStart >= block.base_addr + block.size) continue;
            u32 realBlockStart = std::max(searchStart, block.base_addr);
            u32 realBlockSize = (block.base_addr + block.size) - realBlockStart;
            if (realBlockSize >= realSize && realBlockSize < bestFit.size)
            {
                bestFit.base_addr = realBlockStart;
                bestFit.size = realBlockSize;
            }
        }
        return bestFit.base_addr;
    }

    void ProcessImpl::EnableExceptionHandlers()
    {
        if (MainThreadTls)
        {
            *(u32*)(MainThreadTls + 0x40) = (u32)ProcessImpl::ExceptionHandler;
            *(u32*)(MainThreadTls + 0x44) = 1;//(u32)&stack[0x1000];
            *(u32*)(MainThreadTls + 0x48) = 1;//(u32)&exceptionData;
        }
    }

    void ProcessImpl::DisableExceptionHandlers()
    {
        if (MainThreadTls)
            *(u32*)(MainThreadTls + 0x40) = 0;
    }

    void    ProcessImpl::ReturnFromException(CpuRegisters* regs) {
#ifndef _MSC_VER
        __asm__ __volatile__(
            "ldr sp,    [r0,#0x34]  @sp \n"
            "ldr r1,    [r0, #0x3c] @pc \n"
            "str r1,    [sp, #-4]!      \n"
            "ldr r1,    [r0, #0x38] @lr \n"
            "str r1,    [sp, #-4]!      \n"
            "mov r2,    #0x30           \n"

            "_store_reg_loop:           \n"
            "ldr r1,    [r0, r2]        \n"
            "str r1,    [sp, #-4]!      \n"
            "sub r2,    r2, #4          \n"
            "cmp r2,    #0              \n"
            "bge        _store_reg_loop \n"

            "ldr r1,    [r0, #0x40]     \n"
            "msr cpsr,  r1              \n"
            "ldmfd sp!, {r0-r12, lr, pc}\n"
        );
#endif
    }
    static CpuRegisters g_previousException;

    void    ProcessImpl::ExceptionHandler(ERRF_ExceptionInfo* excep, CpuRegisters* regs) {
        // Default exception handler, if the user didn't set an custom exception handler or an exception happened in the user callback
        if (AtomicPostIncrement(&exceptionCount) || !Process::exceptionCallback) {
            DisableExceptionHandlers();
            ReturnFromException(Process::ThrowOldExceptionOnCallbackException ? &g_previousException : regs);
        }
        // Lock game threads
        LockGameThreads();

        // Backup exception register state
        memcpy(&g_previousException, regs, sizeof(CpuRegisters));

        // Resume interrupt reciever and acquire screens
        // NOTE: NEEDS TO BE DISABLED IF THIS FUNCTION IS MADE TO RETURN EXECUTION
        GSP::ResumeInterruptReceiver();
        if (!ScreenImpl::AcquireFromGsp(false))
            // Update OSD screens
            OSDImpl::UpdateScreens();

        // Update memregions, this layout is used by internal checks
        UpdateMemRegions(true);

        Process::ExceptionCallbackState ret = Process::EXCB_LOOP;

        while (ret == Process::EXCB_LOOP)
            ret = Process::exceptionCallback(excep, regs);

        switch (ret)
        {
        case Process::EXCB_REBOOT:
            System::Reboot();
            break;
        case Process::EXCB_RETURN_HOME:
            Process::ReturnToHomeMenu();
            break;
        default:
            // Rethrow the exception to the default exceptions handlers
            DisableExceptionHandlers();
            ReturnFromException(regs);
            break;
        }
    }

    void     ProcessImpl::SignalExit(void)
    {
        LightEvent_Signal(&waitForExitEvent);
    }

    void     ProcessImpl::WaitForExit(void)
    {
        LightEvent_Wait(&waitForExitEvent);
    }
}
