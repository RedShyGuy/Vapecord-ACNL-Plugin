#include "ctrulibExtension.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

extern const u8 __tdata_lma[];
extern const u8 __tdata_lma_end[];
extern u8 __tls_start[];
extern u8 __tls_end[];

struct Thread_tag
{
	Handle handle;
	ThreadFunc ep;
	void* arg;
	int rc;
	bool detached, finished;
	struct _reent reent;
	void* stacktop;
};

static void __panic(void)
{
	svcBreak(USERBREAK_PANIC);
	for (;;);
}

static void _thread_begin(void* arg)
{
	Thread t = (Thread)arg;
	ThreadVars* tv = getThreadVars();
	tv->magic = THREADVARS_MAGIC;
	tv->reent = &t->reent;
	tv->thread_ptr = t;
	tv->tls_tp = (u8*)t->stacktop-8; // ARM ELF TLS ABI mandates an 8-byte header
	t->ep(t->arg);
	threadExit(0);
}

u32     KProcess__PatchCategory(u32 newCatagory);
u32     KProcess__PatchMaxPriority(u32 newPrio);

Thread __createThread(ThreadFunc entrypoint, u32 stackSize, bool detached)
{
    size_t stackoffset = (sizeof(struct Thread_tag)+7)&~7;
	size_t allocsize   = stackoffset + ((stackSize+7)&~7);
	size_t tlssize = __tls_end-__tls_start;
	size_t tlsloadsize = __tdata_lma_end-__tdata_lma;
	size_t tbsssize = tlssize-tlsloadsize;

	// Guard against overflow
	if (allocsize < stackoffset) return NULL;
	if ((allocsize-stackoffset) < stackSize) return NULL;
	if ((allocsize+tlssize) < allocsize) return NULL;

    Thread t = (Thread)memalign(8, allocsize+tlssize);
    if (!t) return NULL;

    t->ep       = entrypoint;
    t->ep       = entrypoint;
	t->arg      = NULL;
	t->detached = detached;
	t->finished = false;
	t->stacktop = (u8*)t + allocsize;

	if (tlsloadsize)
		memcpy(t->stacktop, __tdata_lma, tlsloadsize);
	if (tbsssize)
		memset((u8*)t->stacktop+tlsloadsize, 0, tbsssize);

	// Set up child thread's reent struct, inheriting standard file handles
	_REENT_INIT_PTR(&t->reent);
	struct _reent* cur = getThreadVars()->reent;
	t->reent._stdin  = cur->_stdin;
	t->reent._stdout = cur->_stdout;
	t->reent._stderr = cur->_stderr;

    return t;
}

Result __startThread(Thread t, int prio, int affinity)
{
    u32     oldAppType = -1;
    u32     oldPrio = -1;
	Result  rc;

    // If affinity is meant to be on syscore or N3DS Core3, patch the application
    if (affinity != -1 && affinity & 1)
        oldAppType = KProcess__PatchCategory(0x300);

    // Patch max priority allowed if necessary
    if (prio < 0x18)
        oldPrio = KProcess__PatchMaxPriority(0);

    rc = svcCreateThread(&t->handle, _thread_begin, (u32)t, (u32 *)t->stacktop, prio, affinity);

    if (oldAppType != -1)
        KProcess__PatchCategory(oldAppType);

    if (oldPrio != -1)
        KProcess__PatchMaxPriority(oldPrio);

    return rc;
}

Thread threadCreate(ThreadFunc entrypoint, void* arg, size_t stack_size, int prio, int affinity, bool detached)
{
	Thread t = __createThread(entrypoint, stack_size, detached);
	if (!t) return NULL;

    t->arg = arg;
	if (R_FAILED(__startThread(t, prio, affinity)))
	{
		free(t);
		return NULL;
	}

	return t;
}

Handle threadGetHandle(Thread thread)
{
	if (!thread || thread->finished) return ~0UL;
	return thread->handle;
}

int threadGetExitCode(Thread thread)
{
	if (!thread || !thread->finished) return 0;
	return thread->rc;
}

void threadFree(Thread thread)
{
	if (!thread || !thread->finished) return;
	svcCloseHandle(thread->handle);
	free(thread);
}

Result threadJoin(Thread thread, u64 timeout_ns)
{
	if (!thread || thread->finished) return 0;
	return svcWaitSynchronization(thread->handle, timeout_ns);
}


void threadDetach(Thread thread)
{
	if (!thread || thread->detached)
		return;

	if (thread->finished)
	{
		threadFree(thread);
		return;
	}

	thread->detached = true;
	return;
}


Thread threadGetCurrent(void)
{
	ThreadVars* tv = getThreadVars();

    // If magic isn't valid, then it's a game's thread
	if (tv->magic != THREADVARS_MAGIC)
    {
		//__panic();
        return NULL;
    }

	return tv->thread_ptr;
}

void threadExit(int rc)
{
	Thread t = threadGetCurrent();
	if (!t)
		__panic();

	t->finished = true;
	if (t->detached)
		threadFree(t);
	else
		t->rc = rc;

	svcExitThread();
}
