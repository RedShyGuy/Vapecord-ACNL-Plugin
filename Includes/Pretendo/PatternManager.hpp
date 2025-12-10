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

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    class PatternManager
    {
    using   PatternCallback = bool(*)(u32);
    public:

        class PatternEntry
        {
            friend class PatternManager;
        public:
            PatternEntry(const u8* pattern, u32 size, PatternCallback cb);
            ~PatternEntry(void) = default;
        private:
            bool                isHandled;
            size_t              patsize;
            PatternCallback     callback;
            std::vector<u8>     pat;
        };

        PatternManager(void) = default;
        ~PatternManager(void) = default;

        void    Add(const u8* pattern, u32 size, PatternCallback cb);
        void    Perform(void);

    private:
        std::vector<PatternEntry> entries{};


        static s32  PerformTaskFunc(void *arg);
    };
}