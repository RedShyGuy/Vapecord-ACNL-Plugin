#include "Helpers/Pretendo.hpp"
#include "PatternManager.hpp"
#include "rt.h"

namespace CTRPluginFramework {
    // Pretendo GameHelper functions and variables
    RT_HOOK acnlPacket0x81HandlerHook = { 0 };
    u32 acnlPacket0x81HandlerAddr = 0;
    Mutex acnlPacket0x81HandlerMutex;
    std::vector<u32> acnlGameServerIDAddrs;
    Mutex acnlGameServerIDAddrMutex;

    void acnlPacket0x81Handler(u8 arg0, void* buffer, u32 size, u32 arg3, u32 arg4) {
        if (size > 0x1AC) {
            OSD::Notify("Detected attempt of ACNL exploit");
            size = 0x1AC;
        }

        return ((void(*)(u8, void*, u32, u32, u32))acnlPacket0x81HandlerHook.callCode)(arg0, buffer, size, arg3, arg4);
    }

    bool installACNLPacket0x81Handler(u32 addr) {
        Lock lock(acnlPacket0x81HandlerMutex);
        if (acnlPacket0x81HandlerAddr) return true;
        u32 funcStart = addr; // (u32)findNearestSTMFDptr((u32*)addr); -- The pattern already points to the beginning of the function
        if (!funcStart) return false;
        rtInitHook(&acnlPacket0x81HandlerHook, funcStart, (u32)acnlPacket0x81Handler);
        acnlPacket0x81HandlerAddr = funcStart;
        return true;
    }

    bool installACNLGameServerIDAddr(u32 addr) {
        Lock lock(acnlGameServerIDAddrMutex);
        acnlGameServerIDAddrs.push_back(addr);
        return false;
    }

    PatternStatus getACNLPatchesStatus() {
        if (!acnlPacket0x81HandlerAddr) {
            return PatternStatus::NotFound;
        }

        if (!acnlPacket0x81HandlerHook.isEnabled) {
            return PatternStatus::NotActive;
        }

        return PatternStatus::Active;
    }

    void initPretendoPatches(PatternManager& pm) {
        const u8 acnlPacket0x81HandlerPat[] = { 0xF0, 0x40, 0x2D, 0xE9, 0x6B, 0xDF, 0x4D, 0xE2 };
        pm.Add(acnlPacket0x81HandlerPat, sizeof(acnlPacket0x81HandlerPat), installACNLPacket0x81Handler);

        const u8 acnlGameServerIDPat[] = { 0x01, 0x62, 0x08, 0x00 };
        pm.Add(acnlGameServerIDPat, sizeof(acnlGameServerIDPat), installACNLGameServerIDAddr);
    }

    void enablePretendoPatches() {
        if (acnlPacket0x81HandlerAddr) {
            rtEnableHook(&acnlPacket0x81HandlerHook);
            for (u32 addr : acnlGameServerIDAddrs) {
                *reinterpret_cast<u32*>(addr) = 0x00086202;
            }
        }
    }

    void disablePretendoPatches() {
        rtDisableHook(&acnlPacket0x81HandlerHook);
        for (u32 addr : acnlGameServerIDAddrs) {
            *reinterpret_cast<u32*>(addr) = 0x00086201;
        }
    }

    void finiPatches() {
        disablePretendoPatches();
        acnlGameServerIDAddrs.clear();
    }
}