#include <3ds.h>
#include "csvc.h"
#include "CTRPluginFramework/System/FwkSettings.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"

namespace CTRPluginFramework
{
    extern "C" char *fake_heap_start;
    extern "C" char *fake_heap_end;

    extern "C" u32 __ctru_heap;
    extern "C" u32 __ctru_heap_size;

    static void Fail(u32 res)
    {
        *(u32 *)0xDEADC0DE = res;
        for(;;);
    }

    struct ScreensFramebuffers
    {
        u8  topFramebuffer0[400 * 240 * 2];
        u8  topFramebuffer1[400 * 240 * 2];
        u8  bottomFramebuffer0[320 * 240 * 2];
        u8  bottomFramebuffer1[320 * 240 * 2];
    } PACKED;

    // Heap layout
    /*
    ** ScreensFramebuffer
    ** Newlib heap
    ** hook page
    ** ar shared page
    */
    extern "C" void   __system_allocateHeaps(void);
    void __system_allocateHeaps(void)
    {
        u32 fbSize = (sizeof(ScreensFramebuffers) + 0x1000) & ~0xFFF;

        // Heap params
        __ctru_heap = FwkSettings::Header->heapVA + fbSize;
        __ctru_heap_size = FwkSettings::Header->heapSize - 0x2000 - fbSize;

        // Set up mappable heap
        mappableInit(0x11000000, 0x14000000);

        // Map Hook memory + shared page
        Result res = svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x1E80000, CUR_PROCESS_HANDLE, __ctru_heap + __ctru_heap_size, 0x2000);

        if (R_FAILED(res))
            Fail(res);

        // Set up newlib heap
        fake_heap_start = reinterpret_cast<char*>(__ctru_heap);
        fake_heap_end = fake_heap_start + __ctru_heap_size;
    }
}
