#include "PointerSearcher.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "3ds/result.h"

namespace CTRPluginFramework
{
    namespace MemRegion
    {
        struct Region
        {
            u32     start;
            u32     end;
        };

        static u32                  g_index;
        static const Region *       g_lastRegion;
        static std::vector<Region>  g_regions;

        static void     Update(void)
        {
            g_regions.clear();
            g_index = 0;

            Handle      process = Process::GetHandle();
            PageInfo    pageI;
            MemInfo     memI;

            svcQueryProcessMemory(&memI, &pageI, process, 0x00100000);
            g_regions.push_back((Region){memI.base_addr, memI.base_addr + memI.size});

            for (u32 addr = memI.base_addr + memI.size + 1; addr < 0x40000000; addr += memI.size + 1)
            {
                if (R_FAILED(svcQueryProcessMemory(&memI, &pageI, process, addr)))
                    break;

                // Don't search in NTR & plugin's regions
                if (memI.base_addr == 0x06000000 || memI.base_addr == 0x07000000 || memI.base_addr == 0x07500000)
                    continue;

                if (memI.state != 0x0 && memI.state != 0x2 && memI.state != 0x3 && memI.state != 0x6)
                {
                    if (memI.perm & MEMPERM_READ)
                        g_regions.push_back((Region){ memI.base_addr, memI.base_addr + memI.size });
                }
            }
        }

        static inline bool      NextRegion(void)
        {
            ++g_index;
            return g_index >= g_regions.size();
        }

        static inline Region    GetRegion(void)
        {
            return *g_lastRegion; ///< I know it's dangerous, so make sure g_lastRegion is always set
        }

        static bool     IsValidAddress(const u32 address)
        {
            if (g_lastRegion->start <= address && g_lastRegion->end > address)
                return true;

            for (const Region &region : g_regions)
            {
                if (region.start >= address && region.end < address)
                {
                    g_lastRegion = &region;
                    return true;
                }
            }

            return false;
        }
    }

    PointerSearcher::PointerSearcher(const u32 address, const u32 maxOffset, const u32 maxLayer) :
        _address{ address }, _maxOffset{ maxOffset }, _maxLayer{ maxLayer }
    {
    }

    void    PointerSearcher::Start(void)
    {
        MemRegion::Update();
    }
}
