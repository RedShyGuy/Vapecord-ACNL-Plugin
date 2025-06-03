#ifndef CTRPLUGINFRAMEWORKIMPL_SYSTEM_KERNEL_HPP
#define CTRPLUGINFRAMEWORKIMPL_SYSTEM_KERNEL_HPP

#include "types.h"
#include <string>

enum class KType
{
    KAutoObject = 0,
    KSynchronizationObject = 1,
    KEvent = 0x1F,
    KSemaphore = 0x2F,
    KTimer = 0x35,
    KDebug = 0x4D,
    KServerPort = 0x55,
    KDmaObject = 0x59,
    KClientPort = 0x65,
    KCodeSet = 0x68,
    KSession = 0x70,
    KThread = 0x8D,
    KServerSession = 0x95,
    KAddressArbiter = 0x98,
    KClientSession = 0xA5,
    KPort = 0xA8,
    KSharedMemory = 0xB0,
    KProcess = 0xC5,
    KResourceLimit = 0xC8
};

struct KClassToken
{
    const char  *name;
    u8          flags; ///< See @enum KType
} PACKED;

struct KAutoObject
{
    struct Vtable
    {
        void    *f;
        void    *f1;
        void    *f2;
        void    *f3;
        KAutoObject *(*DecrementRefCount)(KAutoObject *obj);
        void    *f4;
        KClassToken *(*GetClassToken)(KClassToken *out, KAutoObject *obj);
    }*      vtable;
    u32     refcount;

    std::string     GetName(void);
    KType           GetType(void);
} PACKED;

struct KCodeSetMemDescriptor
{
    u32     startAddress;
    u32     totalPages;
    u32     kBlockInfoCount;
    u32     *firstKLinkedNode;
    u32     *lastKLinkedNode;
} PACKED;

struct KCodeSet
{
    u32     *vtable;
    u32     refCount;
    KCodeSetMemDescriptor text;
    KCodeSetMemDescriptor rodata;
    KCodeSetMemDescriptor data;
    u32     textPages;
    u32     rodataPages;
    u32     rwPages;
    char    processName[8];
    u32     unknown;
    u64     titleId;
} PACKED;

struct HandleDescriptor
{
    u32             info;
    KAutoObject     *obj;
} PACKED;

struct KThread;
struct KObjectMutex
{
    KThread *owner;
    s16     counter1;
    s16     counter2;
} PACKED;

struct KProcessHandleTable
{
    HandleDescriptor    *handleTable;
    s16                 maxHandle;
    s16                 openedHandleCounter;
    HandleDescriptor    *nextOpenHandleDecriptor;
    s16                 totalHandles;
    s16                 handlesCount;
    KObjectMutex        mutex;
    HandleDescriptor    table[0x28];
} PACKED;

struct KProcess;
struct KThread
{
    u32     vtable;
    u32     refcount;
    u32     nbKThreadSync;
    u32     firstKThreadSync;
    u32     lastKThreadSync;
    u32     events[8];
    u8      schedulingMask;
    u8      sendSyncRequestWokenUp;
    s8      shallTerminate;
    u8      error;
    u32     kdebug;
    u32     basePriority;
    u32     objWaitingOn;
    s32     status;
    u32     kObjectMutex;
    u32     arbitrationAddress;
    u32     firstWaitingObj;
    u32     lastWaitingObj;
    u32     kMutexLinkedList[2];
    u32     kMutexCount;
    u32     firstKMutexList;
    u32     lastKMutexList;
    s32     dynamicPriority;
    s32     coreId;
    u32     kPreemptionTimer;
    u32     unknown;
    u8      isAlive;
    u8      hasBeenTerminated;
    u8      affinityMask;
    u8      padding;
    KProcess* owner;
    u32     threadId;
    u32     svcRegisterStorage;
    u32     endAddress;
    s32     idealProcessor;
    u32     tls;
    u32     fcramTls;
    u32     padding1;
    u32     scheduledThreads[2];
    u32     ptrToLinkedList;
    s32     priorityForInheritance;

    u32  *  GetTls(void);
    bool    IsPluginThread(void);

    KProcess * GetOwner(void);

} PACKED;

struct KProcess
{
    void    *vtable;
    u32     refcount;

    static KProcess *   GetCurrent(void);

    void    PatchCore2Access(void);
    u32     PatchCategory(u32 newCategory);
    u32     PatchMaxPriority(u32 newPrio);
    void    PatchMaxThreads(void);
    KAutoObject *   GetObjFromHandle(Handle handle);
    std::string     GetName(void);

} PACKED;

struct KScheduler
{
    void    *vtable;
    void    *KSyncObj;
    u32     countThreadSwitchAttempt;
    u8      contextSwitchNeeded;
    u8      contextSwitchStartedDuringInterrupt;
    u8      triggerCrossCoreInterrupt;
    u8      postInterruptReschedulingNeeded;
    s16     coreNumber;
    s16     countOfThreads;
    u64     priorityBitfield;
    KThread*schedulerThread;
    u32     threadList[2];
    u32     block[0x80];
} PACKED;

struct KCoreObjectContext
{
    KThread *       currentThread;
    KThread *       currentProcess;
    KScheduler *    currentScheduler;
    u32             currentKSchedulableInterruptEventLinkedList;
    KThread *       lastExceptionThread;
    u32             padding;
    KThread         schedulerThreadInstance;
    KScheduler      schedulerInstance;
    u32             block[0x344];
} PACKED;

struct KCoreContext
{
    u32     na[0x400];
    u32     l1[0x1000];
    u32     na2[0x400];
    u32     kthreadContext[0x400];
    u32     na3[0x400];
    KCoreObjectContext  objectContext;
} PACKED;

struct KPreemptionTimer
{
    u32     maxCpuTime0;
    u32     currentTimer;
    u32     lastWatchdogTimer;
    u32     maxCpuTume1;
} PACKED;

struct KResourceLimit
{
    void    *vtable;
    u32     refcount;
    s32     maxPriority;
    s32     maxCommit;
    s32     maxThread;
    s32     maxEvent;
    s32     maxMutex;
    s32     maxSemaphore;
    s32     maxTimer;
    s32     maxSharedArbiter;
    s32     maxCpuTime;
    s32     currentPriority;
    s32     currentCommit;
    s32     currentThread;
    s32     currentEvent;
    s32     currentMutex;
    s32     currentSemaphore;
    s32     currentTimer;
    s32     currentSharedMemory;
    s32     currentAddressArbiter;
    s32     currentCpuTime;
    KObjectMutex        mutex;
    KPreemptionTimer    timer;

    void    IncreaseMaxThreadLimit(void);
} PACKED;

struct KRecursiveLock
{
    KThread     *owner;
    s32         count;

    void    Lock(void);
    void    Unlock(void);
};


namespace Kernel
{
    void    Memcpy(void *dst, const void *src, const u32 size);
    u32     GetCurrentCoreId(void);
}

#endif
