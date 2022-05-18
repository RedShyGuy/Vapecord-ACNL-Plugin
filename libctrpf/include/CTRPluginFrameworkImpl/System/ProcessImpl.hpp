#ifndef CTRPLUGINFRAMEWORKIMPL_PROCESSIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_PROCESSIMPL_HPP

#include "3ds.h"
#include "CTRPluginFrameworkImpl/System/Kernel.hpp"
#include "CTRPluginFramework/System/Mutex.hpp"
#include <vector>

namespace CTRPluginFramework
{
    #define MEMPERM_RW (MEMPERM_READ | MEMPERM_WRITE)
    #define MEMPERM_RWX (MEMPERM_RW | MEMPERM_EXECUTE)

    static inline bool      operator<(const MemInfo& left, const MemInfo& right)
    {
        return left.base_addr < right.base_addr
            || (left.base_addr == right.base_addr && left.size < right.size);
    }

    static inline bool      operator>(const MemInfo& left, const MemInfo& right)
    {
        return left.base_addr > right.base_addr
            || (left.base_addr == right.base_addr && left.size > right.size);
    }

    static inline bool      operator<=(const MemInfo& left, const MemInfo& right)
    {
        return left.base_addr < right.base_addr
            || (left.base_addr == right.base_addr && left.size <= right.size);
    }

    static inline bool      operator>=(const MemInfo& left, const MemInfo& right)
    {
        return left.base_addr > right.base_addr
            || (left.base_addr == right.base_addr && left.size >= right.size);
    }

    static inline bool      operator==(const MemInfo& left, const MemInfo& right)
    {
        return left.base_addr == right.base_addr && left.size == right.size;
    }

    static inline bool      operator!=(const MemInfo& left, const MemInfo& right)
    {
        return left.base_addr != right.base_addr || left.size != right.size;
    }

    enum
    {
        Running = 0,
        Paused = 1,
        NoImage = 2,
    };

    class ProcessImpl
    {
    public:
        // Pause the current process
        static void     Pause(bool useFading);
        // Unpause the current process
        static void     Play(bool forced);

        static void     Initialize(void);

        static bool     PatchProcess(u32 addr, u8 *patch, u32 length, u8 *original);
        static void     GetHandleTable(KProcessHandleTable &table, std::vector<HandleDescriptor> &handleDescriptors);
        static void     GetGameThreads(std::vector<KThread *> &threads);
        static void     LockGameThreads(void);
        static void     UnlockGameThreads(void);

        static void     UpdateMemRegions(bool ignoreLock = false);
        static bool     IsValidAddress(const u32 address);
        static u32      GetPAFromVA(const u32 address);
        static MemInfo  GetMemRegion(const u32 address);
        static MemInfo  GetNextRegion(const MemInfo &region);
        static MemInfo  GetPreviousRegion(const MemInfo &region);
        static void     GetFreeBlocks(std::vector<MemInfo> &blocks);
        static u32      GetFreeMemRegion(const u32 size, const u32 searchStart = 0x100000);
        static void ExceptionHandler(ERRF_ExceptionInfo* excep, CpuRegisters* regs) NORETURN;
        static void     SignalExit(void);
        static void     WaitForExit(void);

        static Handle       ProcessHandle;
        static u32          IsPaused;
        static u32          Status;
        static u32          ProcessId;
        static u64          TitleId;

        static KThread *    MainThread;
        static KProcess *   KProcessPtr;
        static KCodeSet     CodeSet;
        static u32          MainThreadTls;

        static MemInfo      InvalidRegion;
        static Mutex        MemoryMutex;
        static std::vector<MemInfo>     MemRegions;

        static LightEvent   waitForExitEvent;

        static u32          exceptionCount;
        static void         EnableExceptionHandlers();
        static void         DisableExceptionHandlers();
        static void         ReturnFromException(CpuRegisters* regs) NAKED NORETURN;
    };
}

#endif
