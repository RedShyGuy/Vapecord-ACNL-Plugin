#include "CTRPluginFrameworkImpl/Menu/WatchPointManager.hpp"
#include "CTRPluginFramework/System/Lock.hpp"
#include "csvc.h"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"

namespace CTRPluginFramework
{

    #define DBG_EXCEPTION_ENTRY_BREAKPOINT 1
    #define DBG_EXCEPTION_ENTRY_WATCHPOINT 2
    #define DBG_EXCEPTION_ENTRY_BKPT 3

    WatchPointManager   WatchPointManager::_singleton;

    void __ExceptionHandler(ERRF_ExceptionInfo* excep, CpuRegisters* regs);
    u8 stack[0x1000] ALIGN(8);
    ERRF_ExceptionData exceptionData;

    static u32      GetWFAR(void)
    {
        u32     (*K__GetWFAR)(void) = [](void)
        {
            u32 wfar;
#ifndef _MSC_VER
            __asm__ __volatile__("mrc p15, 0, %[val], c6, c0, 1" : [val] "=r" (wfar));
#endif
            return wfar;
        };

        return svcCustomBackdoor((void *)K__GetWFAR);
    }

    static u32      GetDSCR(void)
    {
        u32     (*K__GetDSCR)(void) = [](void)
        {
            u32 dscr;
#ifndef _MSC_VER
            __asm__ __volatile__("mrc p14, 0, %[val], c0, c1, 0" : [val] "=r" (dscr));
#endif
            return dscr;
        };

        return svcCustomBackdoor((void *)K__GetDSCR);
    }

    WatchPointHit&  WatchPoint::GetHitData(u32 wfar)
    {
        for (auto& hit : _hits)
            if (hit._wfar == wfar)
                return hit;

        _hits.emplace_back(wfar);
        return _hits.back();
    }

    void     WatchPointManager::Initialize(void)
    {
        Lock    lock(_mutex);

        if (_isInitialized)
            return;

        svcKernelSetState(0x10003); // enable monitor mode debugging
        svcKernelSetState(0x10004, 0); // disable watchpoint 0
        svcKernelSetState(0x10004, 1); // disable watchpoint 1

        // Get current ctx id
        u32 (*K_GetCtxId)(void) = [](void)
        {

            u32 contextId;
#ifndef _MSC_VER
        __asm__ __volatile__("mrc p15, 0, %[val], c13, c0, 1" : [val] "=r" (contextId));
#endif
            return contextId;
        };

        _contextId = svcCustomBackdoor((void *)K_GetCtxId, 0, 0, 0);

    	*(u32 *)(ProcessImpl::MainThreadTls + 0x40) = (u32)__ExceptionHandler;
	    *(u32 *)(ProcessImpl::MainThreadTls + 0x44) = 1;//(u32)&stack[0x1000];
	    *(u32 *)(ProcessImpl::MainThreadTls + 0x48) = 1;//(u32)&exceptionData;

        _isInitialized = true;
    }

    void    WatchPointManager::SetWatchPoint(u32 address, u32 size, u32 kind)
    {
        auto&   manager = _singleton;

        Lock    lock(manager._mutex);

        manager.Initialize();

        u32     offset = address - (address & ~3);

        if (manager._total >= 2)
            return;

        if(size == 0 || (offset + size) > 4 || kind == WatchPoint::Disabled)
            return;

        u32     id = manager._watchPoints[0]._kind == WatchPoint::Disabled ? 0 : 1;

        if (manager._watchPoints[!id]._address == address)
            return; ///< No duplicate

        u32 selectMask = ((1 << size) - 1) << offset;

        u32 WCR = (1          << 20) | /* linked */
                  ((4 + id)   << 16) | /* ID of the linked BRP */
                  (selectMask <<  5) | /* byte address select */
                  ((u32)kind  <<  3) | /* kind */
                  (2          <<  1) | /* user mode only */
                  (1          <<  0) ; /* enabled */



        svcKernelSetState(id == 0 ? 0x10005 : 0x10006, address, WCR, manager._contextId); // set watchpoint
        WatchPoint& watchpoint = manager._watchPoints[id];

        manager._total++;
        watchpoint._address = address;
        watchpoint._size = size;
        watchpoint._kind = kind;
        watchpoint._totalHit = 0;
    }

    void    WatchPointManager::HandleException(ERRF_ExceptionInfo *excep, CpuRegisters *regs)
    {
        auto&   manager = _singleton;

        u32     wpid;
        u32     tflag = (regs->cpsr & 0x20) >> 5;
        u32     wfar = GetWFAR() - 8 + tflag * 4;

        {
            Lock    scopedLock(manager._mutex);

            for (wpid = 0; wpid < 2; ++wpid)
            {
                if (manager._watchPoints[wpid]._address == excep->far)
                    break;
            }

            if (wpid >= 2)
                return; ///< Didn't found the watchpoint ???

            WatchPoint&     wp = manager._watchPoints[wpid];
            WatchPointHit&  hit = wp.GetHitData(wfar);

            ++hit._count;

            if ((hit._flags & WatchPointHit::Skip) == 0)
                OSD::Notify(Utils::Format("WP%d triggered: 0x%08X", wpid, wfar));

            if (tflag)
		    {
			    regs->cpsr &= ~0x20;
			    regs->pc += 1;
		    }
        }
        ProcessImpl::ReturnFromException(regs);
    }

    void    __ExceptionHandler(ERRF_ExceptionInfo* excep, CpuRegisters* regs)
    {
        u32     dscrEvent = (GetDSCR() >> 2) & 0xF;

        if (dscrEvent == DBG_EXCEPTION_ENTRY_BKPT)
        {
            // TODO: not sure about bkpt support but why not
        }
        else if (dscrEvent == DBG_EXCEPTION_ENTRY_BREAKPOINT)
        {
            // TODO: hardware bp are ok to implement
        }
        else if (dscrEvent == DBG_EXCEPTION_ENTRY_WATCHPOINT)
        {
            WatchPointManager::HandleException(excep, regs);
        }

        // Re-throw the error
        ERRF_ExceptionHandler(excep, regs);
    }
}
