#include "CTRPluginFramework/System/Thread.hpp"
#include "CTRPluginFramework/System/Mutex.hpp"
#include "CTRPluginFramework/System/Lock.hpp"
#include "ctrulibExtension.h"

#include <cstdlib>
#include <cstring>
#include <map>

using CTRPluginFramework::Mutex;
using CTRPluginFramework::Lock;

typedef struct
{
    // Magic value used to check if the struct is initialized
    u32 magic;

    // Pointer to the current thread (if exists)
    Thread thread_ptr;

    // Pointer to this thread's newlib state
    struct _reent* reent;

    // Pointer to this thread's thread-local segment
    void* tls_tp; // !! Keep offset in sync inside __aeabi_read_tp !!

    // FS session override
    u32    fs_magic;
    Handle fs_session;

    struct _reent _reent;
} ThreadVarsHook;

static Mutex                        __mutex;
static std::map<u32, ThreadVars *>  __threads;

extern "C"
{
    extern const u8 __tdata_lma[];
    extern const u8 __tdata_lma_end[];
    extern u8 __tls_start[];
    extern u8 __tls_end[];

    Thread __createThread(ThreadFunc entrypoint, u32 stackSize, bool detached);
    Result __startThread(Thread t, int prio, int affinity);

    static ThreadVars* CreateThreadVars(ThreadVars * mainThreadVars)
    {
        size_t tvsize = (sizeof(ThreadVarsHook)+7)&~7;
        size_t tlssize = __tls_end-__tls_start;
        size_t tlsloadsize = __tdata_lma_end-__tdata_lma;
        size_t tbsssize = tlssize-tlsloadsize;

        ThreadVarsHook *tv = (ThreadVarsHook *)_malloc_r(mainThreadVars->reent, tvsize+tlssize);
        if (!tv) return NULL;

        u8 *tls = ((u8 *)tv) + tvsize;

        memset(tv, 0, tvsize+tlssize);
        tv->magic = THREADVARS_MAGIC;
        tv->tls_tp = tls - 8;
        tv->reent = &tv->_reent;

        if (tlsloadsize)
            memcpy(tls, __tdata_lma, tlsloadsize);
        if (tbsssize)
            memset((u8 *)tls+tlsloadsize, 0, tbsssize);

        // Set up child thread's reent struct, inheriting standard file handles
        _REENT_INIT_PTR(&tv->_reent);
        struct _reent* cur = mainThreadVars->reent;
        tv->_reent._stdin  = cur->_stdin;
        tv->_reent._stdout = cur->_stdout;
        tv->_reent._stderr = cur->_stderr;

        return (ThreadVars*)tv;
    }
    //TODO: hook on svcExitThread to clean this
    ThreadVars* __ctrpf_getThreadVars(ThreadVars * mainThreadVars)
    {
        Lock    lock(__mutex);

        // Hackish way to ensure that we don't recursive ourselves
        if (((RecursiveLock *)&__mutex)->counter > 1)
            return mainThreadVars;

        // Get thread's tls (used as key)
        u32     tls = (u32)getThreadLocalStorage();

        // Check if we already created a context for this thread
        auto    pair = __threads.find(tls);

        if (pair != __threads.end())
            return pair->second;

        // Otherwise create one
        ThreadVars  *tv = CreateThreadVars(mainThreadVars);

        // and add it to our list of thread ctxs
        __threads.insert(std::make_pair(tls, tv));

        // Then return our new buffer
        return tv;
    }
}

namespace CTRPluginFramework
{
    ThreadEx::ThreadEx(ThreadFunc entrypoint, u32 stackSize, u32 priority, int affinity) :
        priority{static_cast<u8>(priority)},
        affinity{static_cast<s8>(affinity)},
        _thread{__createThread(entrypoint, stackSize, false)}
    {
    }

    ThreadEx::~ThreadEx(void)
    {
        if (_thread)
        {
            std::free(_thread);
            _thread = nullptr;
        }
    }

    Result ThreadEx::Start(void *arg)
    {
        if (_thread == nullptr)
            return -1;

        if (_state == RUNNING)
            return 0;

        ((Thread_tag*)_thread)->arg = arg;
        Result res = __startThread(_thread, priority, affinity);

        if (R_SUCCEEDED(res))
            _state = RUNNING;

        return res;
    }

    Result ThreadEx::Join(bool releaseResources)
    {
        Result res = threadJoin(_thread, U64_MAX);
        if (releaseResources)
        {
            threadFree(_thread);
            _thread = nullptr;
        }
        _state = FINISHED;
        return res;
    }

    Handle ThreadEx::GetHandle(void)
    {
        if (_thread)
            return ((Thread_tag*)_thread)->handle;
        return 0;
    }

    u32 ThreadEx::GetStatus(void)
    {
        return _state;
    }

    void    ThreadEx::Yield(void)
    {
        svcSleepThread(0);
    }
}
