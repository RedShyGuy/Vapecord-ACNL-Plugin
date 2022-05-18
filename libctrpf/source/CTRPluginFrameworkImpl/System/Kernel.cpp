#include "CTRPluginFrameworkImpl/System/Kernel.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFramework/System/Lock.hpp"
#include "3ds.h"
#include "csvc.h"

#define THREADVARS_MAGIC  0x21545624 // !TV$

using CTRPluginFramework::Lock;
using CTRPluginFramework::SystemImpl;
using CTRPluginFramework::ProcessImpl;
extern "C" void* memcpy(void *dst, const void *src, unsigned int size);

static inline u32 __get_cpsr(void)
{
    u32 cpsr;
    __asm__ __volatile__("mrs %0, cpsr" : "=r"(cpsr));
    return cpsr;
}

static inline bool __is_svmode(void)
{
    return (__get_cpsr() & 0b11111) == 0b10011;
}

static inline void __set_cpsr_cx(u32 cpsr)
{
    __asm__ __volatile__("msr cpsr_cx, %0" :: "r"(cpsr));
}

namespace Kernel
{
    //static KRecursiveLock * CriticalSectionLock; // = (KRecursiveLock *)0xFFF2F0AC;
    //static KThread  **  CurrentKThread = (KThread **)0xFFFF9000;
    static KProcess **  CurrentKProcess = (KProcess **)0xFFFF9004;
    //static KScheduler** CurrentScheduler  = (KScheduler **)0xFFFF900C;
    //static KObjectMutex * SynchronizationMutex;
    //static KCoreContext * CoreCtxs; // (KCoreContext *)0xFFFC9000;

    //static void    (*KRecursiveLock__Lock)(KRecursiveLock *lock); // = (void (*)(KRecursiveLock *))0xFFF1DC24;
    //static void    (*KRecursiveLock__Unlock)(KRecursiveLock *lock); // = (void (*)(KRecursiveLock *))0xFFF1DD64;
    //static void    (*KScheduler__AdjustThread)(KScheduler *scheduler, KThread *thread, u32 oldSchedulingMask);// = (void (*)(KScheduler *, KThread*, u32))0xFFF1DD38;

    static inline u32   decodeARMBranch(const u32 *src)
    {
        s32 off = (*src & 0xFFFFFF) << 2;
        off = (off << 6) >> 6; // sign extend

        return (u32)src + 8 + off;
    }

    void    Initialize(void)
    {
        return;
    }

    #define MPCORE_REGS_BASE        (0x17E00000 | (1u << 31))
    #define MPCORE_SCU_CFG          (*(vu32 *)(MPCORE_REGS_BASE + 4))
    #define MPCORE_INT_ACK          (*(vu32 *)(MPCORE_REGS_BASE + 0x10C))

    #define MPCORE_GID_REGS_BASE    (MPCORE_REGS_BASE + 0x1000)
    #define MPCORE_GID_SGI          (*(vu32 *)(MPCORE_GID_REGS_BASE + 0xF00))

    /*
    static inline u32 getNumberOfCores(void)
    {
        return (MPCORE_SCU_CFG & 3) + 1;
    }

    static inline u32 getCurrentCoreID(void)
    {
        u32 coreId;
        __asm__ volatile("mrc p15, 0, %0, c0, c0, 5" : "=r"(coreId));
        return coreId & 3;
    }

    void KScheduler__TriggerCrossCoreInterrupt(KScheduler *scheduler)
    {
        scheduler->triggerCrossCoreInterrupt = false;
        for(s16 i = 0; i < (s16)getNumberOfCores(); i++)
        {
            if(scheduler->coreNumber != i)
                MPCORE_GID_SGI = (1 << (16 + i)) | 8;
        }
    } */

    void    Memcpy(void *dst, const void *src, const u32 size)
    {
        void    (*K__Memcpy)(void *, void *, u32) = [](void *dst, void *src, u32 size) -> void
        {
            __asm__ __volatile__ ("cpsid aif");
            memcpy(dst, src, size);
            __asm__ __volatile__ ("cpsie aif");
        };

        svcCustomBackdoor((void *)K__Memcpy, dst, src, size);
    }

    u32     GetCurrentCoreId(void)
    {
        u32    (*K_GetCurrentCoreId)(void) = [](void) -> u32
        {
            u32 coreId;
            __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 5" : "=r"(coreId));
            return coreId & 3;
        };

        return svcCustomBackdoor((void *)K_GetCurrentCoreId);
    }
}

std::string     KAutoObject::GetName(void)
{
    char    buffer[9] = {0};

    void    (*K_GetName)(KAutoObject *, char *) = [](KAutoObject *obj, char *out)
    {
        KClassToken token;

        obj->vtable->GetClassToken(&token, obj);
        for (int i = 0; i < 8; ++i)
            *out++ = token.name[i];
    };

    if (__is_svmode())
        K_GetName(this, buffer);
    else
        svcCustomBackdoor((void *)K_GetName, this, buffer);

    return buffer;
}

KType   KAutoObject::GetType(void)
{
    KType   (*K_GetType)(KAutoObject *) = [](KAutoObject *obj) -> KType
    {
        KClassToken token;

        obj->vtable->GetClassToken(&token, obj);
        return (KType)token.flags;
    };

    if (__is_svmode())
        return K_GetType(this);
    else
        return (KType)svcCustomBackdoor((void *)K_GetType, this);
}

u32 *   KThread::GetTls(void)
{
    u32     (*KFunc)(KThread *) = [](KThread *thread) -> u32
    {
        return thread->tls;
    };

    return (u32 *)svcCustomBackdoor((void *)KFunc, this);
}

bool    KThread::IsPluginThread(void)
{
    return *GetTls() == THREADVARS_MAGIC;
}

KProcess * KThread::GetOwner(void)
{
    KProcess *(*K_GetOwner)(KThread *) = (KProcess *(*)(KThread *))[](KThread *thread)
    {
        return thread->owner;
    };

    return (KProcess *)svcCustomBackdoor((void *)K_GetOwner, this);
}

KProcess * KProcess::GetCurrent(void)
{
    KProcess *(*K_GetCurrent)(void) = [](void) -> KProcess *
    {
        return *Kernel::CurrentKProcess;
    };

    return (KProcess *)svcCustomBackdoor((void *)K_GetCurrent);
}

void    KProcess::PatchCore2Access(void)
{
    if (!SystemImpl::IsNew3DS)
        return;

    void    (*K_PatchCore2Access)(KProcess *) = [](KProcess *process)
    {
        u32 *   kernelFlags = (u32 *)((u32)process + 0xB0);

        *kernelFlags |= 0x2000;
    };

    svcCustomBackdoor((void *)K_PatchCore2Access, this);
}

// C function for thread.c
extern "C" u32  KProcess__PatchCategory(u32 newCatagory)
{
    return ProcessImpl::KProcessPtr->PatchCategory(newCatagory);
}

u32     KProcess::PatchCategory(u32 newCategory)
{
    u32     (*K_PatchCategory)(KProcess *, u32, u32) = [](KProcess *process, u32 newCategory, u32 offset) -> u32
    {
        u32 *   category = (u32 *)((u32)process + offset);
        u32     old = *category & 0xF00;

        *category &= ~0xF00;
        *category |= newCategory;
        return old;
    };

    return svcCustomBackdoor((void *)K_PatchCategory, this, newCategory, SystemImpl::IsNew3DS ? 0xB0 : 0xA8);
}

// C function for thread.c
extern "C" u32     KProcess__PatchMaxPriority(u32 newPrio)
{
    return ProcessImpl::KProcessPtr->PatchMaxPriority(newPrio);
}

u32     KProcess::PatchMaxPriority(u32 newPrio)
{
    u32     (*K_PatchMaxPriority)(KProcess *, u32, u32) = [](KProcess *process, u32 newPrio, u32 offset) -> u32
    {
        KResourceLimit  *resLimit = (KResourceLimit *)*(u32 *)((u32)process + offset);
        u32  oldPrio = resLimit->maxPriority;

        resLimit->maxPriority = newPrio;
        return oldPrio;
    };

    return svcCustomBackdoor((void *)K_PatchMaxPriority, this, newPrio, SystemImpl::IsNew3DS ? 0x84 : 0x7C);
}

void    KProcess::PatchMaxThreads(void)
{
    void (*K_PatchMaxThreads)(KProcess *, u32) = [](KProcess *process, u32 offset)
    {
        KResourceLimit *resLimit = (KResourceLimit *)*(u32 *)((u32)process + offset);

        resLimit->maxThread += 4;
        //resLimit->IncreaseMaxThreadLimit();
    };

    svcCustomBackdoor((void *)K_PatchMaxThreads, this, SystemImpl::IsNew3DS ? 0x84 : 0x7C);
}

KAutoObject * KProcess::GetObjFromHandle(Handle handle)
{
    KAutoObject *(*K_GetObjFromHandle)(KProcess *, Handle) = [](KProcess *process, Handle handle)
    {
        KProcessHandleTable *table = (KProcessHandleTable *)((u32)process + (SystemImpl::IsNew3DS ? 0xDC : 0xD4));

        return table->handleTable[handle & 0xFFFF].obj;
    };

    if (__is_svmode())
        return K_GetObjFromHandle(this, handle);

    return (KAutoObject *)svcCustomBackdoor((void *)K_GetObjFromHandle, this, handle);
}

std::string     KProcess::GetName(void)
{
    void    (*K_GetName)(KProcess *, char *) = [](KProcess *process, char *out)
    {
        KCodeSet *codeset = (KCodeSet *)((u32)process + (SystemImpl::IsNew3DS ? 0xB8 : 0xD0));

        for (int i = 0; i < 8; ++i)
            out[i] = codeset->processName[i];
    };

    char buffer[9] = {0};

    if (__is_svmode())
        K_GetName(this, buffer);
    else
        svcCustomBackdoor((void *)K_GetName, this, buffer);

    return buffer;
}

void    KResourceLimit::IncreaseMaxThreadLimit(void)
{
    void    (*K_IncreaseMaxThreadLimit)(KResourceLimit *) = [](KResourceLimit *resLimit)
    {
        resLimit->maxThread += 4;
    };

    if (__is_svmode())
        K_IncreaseMaxThreadLimit(this);
    else
        svcCustomBackdoor((void *)K_IncreaseMaxThreadLimit, this);
}
