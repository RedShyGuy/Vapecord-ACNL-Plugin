#include <stdlib.h>
#include <stdio.h>
#include "3ds.h"

extern CFNT_s* g_sharedFont;
int g_charPerSheet = 1;

Result fontEnsureMappedExtension(void)
{
    Result res = fontEnsureMapped();
    if (R_SUCCEEDED(res)) g_charPerSheet = g_sharedFont->finf.tglp->nRows * g_sharedFont->finf.tglp->nLines;
    return res;
}
