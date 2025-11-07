#pragma once

#include <3ds.h>
#include "PatternManager.hpp"

namespace CTRPluginFramework
{
    enum PatternStatus {
        NotFound  = -1,
        NotActive = 0,
        Active    = 1
    };

    static inline u32* findNearestSTMFDptr(u32* newaddr) {
        u32 i;
        for (i = 0; i < 1024; i++) {
            newaddr--;
            i++;
            if (*((u16*)newaddr + 1) == 0xE92D) {
                return newaddr;
            }
        }
        return 0;
    }

    PatternStatus getACNLPatchesStatus();

    void initPretendoPatches(PatternManager& pm);
    void enablePretendoPatches();
    void disablePretendoPatches();
    void finiPatches();
}
