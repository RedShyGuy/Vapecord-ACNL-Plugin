/*
 * This file is part of Nimbus.
 * https://github.com/PretendoNetwork/nimbus
 *
 * Copyright (C) 2025 Nimbus contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <3ds.h>
#include "PatternManager.hpp"

namespace CTRPluginFramework {
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