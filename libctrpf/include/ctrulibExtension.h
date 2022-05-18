#pragma once
#include <sys/reent.h>
#include "types.h"
#include "3ds.h"

#define THREADVARS_MAGIC  0x21545624 // !TV$
#define FS_OVERRIDE_MAGIC 0x21465324 // !FS$

#define PA_FROM_VA_PTR(addr)    PA_FROM_VA(addr)

// Needed to override ctrulib Thread_tag
struct CThread_tag
{
    Handle handle;
    ThreadFunc ep;
    void* arg;
    int rc;
    bool detached, finished;
    struct _reent reent;
    void* stacktop;
};

// Keep this structure under 0x80 bytes
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
} ThreadVars;

static inline ThreadVars* getThreadVars(void)
{
	return (ThreadVars*)getThreadLocalStorage();
}

#ifdef __cplusplus
extern "C" {
#endif
// font extension
Result fontEnsureMappedExtension(void);
extern u32 __apt_appid;
extern u32 __system_runflags;

#ifdef __cplusplus
}
#endif
