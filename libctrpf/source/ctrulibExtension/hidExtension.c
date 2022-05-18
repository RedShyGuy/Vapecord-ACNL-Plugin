#include "types.h"
#include "malloc.h"

extern vu32* hidSharedMem;

static bool fakeInit = false;
static bool properInit = false;
void hidInitFake()
{
    if (!fakeInit && !properInit)
    {
        fakeInit = true;
        hidSharedMem = malloc(0x2B0);
    }
}

void hidExitFake()
{
    if (fakeInit && !properInit)
    {
        fakeInit = false;
        free((void*)hidSharedMem);
    }
}

void hidSetSharedMem(vu32* sharedMem) {
    hidExitFake();
    properInit = true;
    hidSharedMem = sharedMem;
}