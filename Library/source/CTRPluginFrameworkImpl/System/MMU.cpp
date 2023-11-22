#include "CTRPluginFrameworkImpl/System/MMU.hpp"
#include "3ds.h"
#include "csvc.h"

static u32 ALIGN(0x1000) HookL2Table[256] = {0};

DescType     L1Descriptor__GetType(u32 descriptor)
{
    L1Descriptor   pdesc = {descriptor};

    if (pdesc.reserved.bits1_0 == 0b00)
        return Descriptor_TranslationFault;
    if (pdesc.reserved.bits1_0 == 0b01)
        return Descriptor_CoarsePageTable;
    if (pdesc.reserved.bits1_0 == 0b10)
        return pdesc.section.bit18 == 0 ? Descriptor_Section : Descriptor_Supersection;
    return Descriptor_Reserved;
}

DescType     L2Descriptor__GetType(u32 descriptor)
{
    L2Descriptor    pdesc = {descriptor};

    if (pdesc.translationFault.bits1_0 == 0b01)
        return Descriptor_LargePage;
    if (pdesc.smallPage.bit1 == 1)
        return Descriptor_SmallPage;

    return Descriptor_TranslationFault;
}

void    L2MMUTable__RWXForAll(u32 *table)
{
    u32     *tableEnd = table + 256;

    for (; table != tableEnd; ++table)
    {
        L2Descriptor    descriptor = {*table};

        switch (L2Descriptor__GetType(descriptor.raw))
        {
            case Descriptor_LargePage:
            {
                descriptor.largePage.xn = 0;
                descriptor.largePage.apx = 0;
                descriptor.largePage.ap = 3;
                *table = descriptor.raw;
                break;
            }
            case Descriptor_SmallPage:
            {
                descriptor.smallPage.xn = 0;
                descriptor.smallPage.apx = 0;
                descriptor.smallPage.ap = 3;
                *table = descriptor.raw;
                break;
            }
            default:
                break;
        }
    }
}

void    L1MMUTable__RWXForAll(u32 *table)
{
    u32     *tableEnd = table + 1024;

    for (; table != tableEnd; ++table)
    {
        L1Descriptor    descriptor = {*table};

        switch (L1Descriptor__GetType(descriptor.raw))
        {
            case Descriptor_CoarsePageTable:
            {
                u32     *l2table = (u32 *)((descriptor.coarsePageTable.addr << 10) - 0x40000000);

                L2MMUTable__RWXForAll(l2table);
                break;
            }
            case Descriptor_Section:
            {
                descriptor.section.xn = 0;
                descriptor.section.apx = 0;
                descriptor.section.ap = 3;
                *table = descriptor.raw;
                break;
            }
            case Descriptor_Supersection:
            {
                descriptor.supersection.xn = 0;
                descriptor.supersection.ap = 3;
                *table = descriptor.raw;
                break;
            }
            default:
                break;
        }
    }
}

static u32 *GetL1MmuTable(void)
{
    s64 out;

    svcGetProcessInfo(&out, CUR_PROCESS_HANDLE, 0x10009);

    return (u32 *)(out >> 32);
}

static u32 L2MMUTable__CreateL2Table(u32 *table, u32 va, u32 pa)
{
    u32 *           tableDesc = &table[(va << 12) >> 24];
    L1Descriptor    cpt = {0};
    L2Descriptor    page = {0};

    cpt.coarsePageTable.bits1_0 = 0b01;
    cpt.coarsePageTable.p = 0;
    cpt.coarsePageTable.addr = svcConvertVAToPA(table, false) >> 10;

    // Initialize the small page desc
    page.smallPage.bit1 = 1;
    page.smallPage.b = 1;
    page.smallPage.ap = 3;
    page.smallPage.tex = 0b110;
    page.smallPage.s = 1;
    page.smallPage.ng = 1;
    page.smallPage.addr = pa >> 12;

    *tableDesc = page.raw;

    return cpt.raw;
}

void    L1MMUTable__MapHookWrappers(u32 va)
{
    u32     *table = GetL1MmuTable();

    u32 *           tableDst = &table[0x1E80000 >> 20];
    L1Descriptor    src = {table[va >> 20]};
    //L1Descriptor    dst;

    switch (L1Descriptor__GetType(src.raw))
    {
    case Descriptor_CoarsePageTable:
    {
        u32 pa = src.coarsePageTable.addr << 10;
        *tableDst = L2MMUTable__CreateL2Table(HookL2Table, 0x1E80000, pa);
        break;
    }
    case Descriptor_Section:
    {
        u32 pa = src.section.addr << 20;
        *tableDst = L2MMUTable__CreateL2Table(HookL2Table, 0x1E80000, pa);
        break;
    }
    case Descriptor_Supersection:
    {
        u32 pa = src.supersection.addr << 24;
        *tableDst = L2MMUTable__CreateL2Table(HookL2Table, 0x1E80000, pa);
        break;
    }
    default:
        svcBreak(USERBREAK_PANIC);
        break;
    }
}