#include <3ds.h>

void hidExitFake();

void    __appExit(void)
{
	// Exit services
    cfguExit();
    fsExit();
    amExit();
    acExit();
    srvExit();
}
