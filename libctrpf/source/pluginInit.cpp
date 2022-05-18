#include <3ds.h>
#include "ctrulibExtension.h"
#include "CTRPluginFrameworkImpl.hpp"
#include "CTRPluginFramework.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Font.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"
#include "CTRPluginFrameworkImpl/System/HookManager.hpp"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"
#include "CTRPluginFrameworkImpl/Sound.hpp"
#include "csvc.h"
#include "plgldr.h"

#define PA_PTR(addr)            (void *)((u32)(addr) | 1 << 31)
#define REG32(addr)             (*(vu32 *)(PA_PTR(addr)))

extern "C"
{
    u32     g_gspEventThreadPriority;

    void    LoadCROHooked(void);
    void    OnLoadCro(void);
    void    abort(void);
    void    initSystem(void);
    void    initLib(void);
    Result  __sync_init(void);
    void    __system_initSyscalls(void);

    s32     PLGLDR__FetchEvent(void);
    void    PLGLDR__Reply(s32 event);

    void hidInitFake();
    void hidExitFake();
    void hidSetSharedMem(vu32* sharedMem);

    extern u32 __ctru_heap;
    extern u32 __ctru_heap_size;
}

using CTRPluginFramework::Hook;
using namespace CTRPluginFrameworkImpl::Services;

namespace CTRPluginFramework
{
    // Threads stacks
    static u8  keepThreadStack[0x1000] ALIGN(8);

    // Some globals
    FS_Archive  _sdmcArchive;
    Handle      g_continueGameEvent = 0;
    Handle      g_keepThreadHandle;
    Handle      g_keepEvent = 0;
    Handle      g_resumeEvent = 0;
    bool        g_keepRunning = true;

    void    ThreadInit(void *arg);
    void    ThreadExit(void);
    void    InstallOSD(void); ///< OSDImpl
    void    InitializeRandomEngine(void);
    static ThreadEx g_mainThread(ThreadInit, 0x4000, 0x19, -1);

    // From main.cpp
    int     main(void);
}

namespace Kernel
{
    void    Initialize(void);
}

static Hook         g_onLoadCroHook;
static Hook         g_onSharedMemMapHook;
static LightLock    g_onLoadCroLock;

void abort(void)
{
    if (CTRPluginFramework::System::OnAbort)
        CTRPluginFramework::System::OnAbort();

    CTRPluginFramework::Color c(255, 69, 0);
    CTRPluginFramework::ScreenImpl::Top->Flash(c);
    CTRPluginFramework::ScreenImpl::Bottom->Flash(c);

    CTRPluginFramework::ThreadExit();
    while (true);
}

static void    ExecuteLoopOnEvent(void)
{
    using CTRPluginFramework::PluginMenuExecuteLoop;

    // Execute AR codes
    PluginMenuExecuteLoop::ExecuteAR();

    // Execute builtin codes
    PluginMenuExecuteLoop::ExecuteBuiltin();
}

void     OnLoadCro(void)
{
    LightLock_Lock(&g_onLoadCroLock);
    ExecuteLoopOnEvent();
    LightLock_Unlock(&g_onLoadCroLock);
}

Result     OnSharedMemMap(Handle handle, void* sharedMem, MemPerm myPerm, MemPerm otherPerm)
{
    Result res = svcMapMemoryBlock(handle, (u32)sharedMem, myPerm, otherPerm);
    if (R_SUCCEEDED(res) && sharedMem && CTRPluginFramework::FwkSettings::Get().UseGameHidMemory)
    {
        u64 firstSysTick = reinterpret_cast<vu64*>(sharedMem)[0];
        u32 arrayIndex = reinterpret_cast<vu32*>(sharedMem)[4];

        u32 sharedMemTouchScreen = (u32)sharedMem + 0xA8;
        u64 firstSysTickTS = reinterpret_cast<vu64*>(sharedMemTouchScreen)[0];
        u32 arrayIndexTS = reinterpret_cast<vu32*>(sharedMemTouchScreen)[4];

        u64 currSysTick = svcGetSystemTick();

        // VERY HACKY VERIFICATION THIS IS HID SHARED MEM!!!
        // Probably breaks if hid thread is paused just here, but the likelyness of this
        // happening is very low, so I guess it's good enough...
        if (llabs(currSysTick - firstSysTick) < 1000000000ULL && llabs(currSysTick - firstSysTickTS) < 1000000000ULL &&
        arrayIndex < 8 && arrayIndexTS < 8)
            hidSetSharedMem((vu32*)sharedMem);
    }
    return res;
}

#ifdef _MSC_VER
#define WEAK_SYMBOL
#else
#define WEAK_SYMBOL __attribute__((weak))
#endif

namespace CTRPluginFramework
{
    void WEAK_SYMBOL    PatchProcess(FwkSettings& settings) {}
    void WEAK_SYMBOL    EarlyCallback(u32* savedInstructions);
    void WEAK_SYMBOL    OnProcessExit(void) {}
    void WEAK_SYMBOL    OnPluginToSwap(void) {}
    void WEAK_SYMBOL    OnPluginFromSwap(void) {}

    namespace Heap
    {
        extern u8* __ctrpf_heap;
        extern u32 __ctrpf_heap_size;
    }

    static void InitFS(void)
    {
        // Init sdmcArchive
        {
            FS_Path sdmcPath = { PATH_EMPTY, 1, (u8*)"" };
            FSUSER_OpenArchive(&_sdmcArchive, ARCHIVE_SDMC, sdmcPath);
        }

        // Set current working directory
        if (FwkSettings::Header->isDefaultPlugin)
        {
            std::string path = "/luma/plugins/ActionReplay";

            if (!Directory::IsExists(path))
                Directory::Create(path);

            path += Utils::Format("/%016llX", Process::GetTitleID());

            if (!Directory::IsExists(path))
                Directory::Create(path);

            Directory::ChangeWorkingDirectory(path + "/");
        }
        else
        {
            char path[255] = {0};

            PLGLDR__GetPluginPath(path);
            for (u32 i = 254; i > 0; --i)
            {
                if (path[i] != '/')
                    continue;
                path[i] = 0;
                break;
            }
            Directory::ChangeWorkingDirectory(path);
        }
    }

    static void     InitHeap(void)
    {
        u32         size = 0x100000; ///< Mostly used for Search

        // Init System::Heap
        Heap::__ctrpf_heap_size = size;
        Heap::__ctrpf_heap = static_cast<u8 *>(::operator new(size));
    }

    void    KeepThreadMain(void *arg UNUSED)
    {
        // Initialize the synchronization subsystem
        __sync_init();

        // Initialize newlib's syscalls
        __system_initSyscalls();

        // Init heap and newlib's syscalls
        initLib();

        // Initialize services
        srvInit();
        acInit();
        amInit();
        fsInit();
        hidInitFake();
        cfguInit();
        ncsndInit(false);
        plgLdrInit();

        // Set cwav VA to PA function
        SoundEngineImpl::Initializelibcwav();
        SoundEngineImpl::SetVaToPaConvFunction([](const void* addr) {return svcConvertVAToPA(addr, false);});

        // Initialize Kernel stuff
        Kernel::Initialize();

        // Init Framework's system constants
        SystemImpl::Initialize();

        // Initialize Globals settings
        InitializeRandomEngine();

        // Init Process info
        ProcessImpl::Initialize();

        // Init Screen
        ScreenImpl::Initialize();

        // Init default settings
        FwkSettings &settings = FwkSettings::Get();

        settings.ThreadPriority = 0x30;
        settings.AllowActionReplay = true;
        settings.AllowSearchEngine = true;
        settings.WaitTimeToBoot = Seconds(5.f);
        settings.TryLoadSDSounds = true;

        // Set default theme
        FwkSettings::SetThemeDefault();

        // Init scheduler
        Scheduler::Initialize();

        // Init OSD hook
        OSDImpl::_Initialize();

        // Install CRO hook
        {
            const std::vector<u32> LoadCroPattern =
            {
                0xE92D5FFF, 0xE28D4038, 0xE89407E0, 0xE28D4054,
                0xE8944800, 0xEE1D4F70, 0xE59FC058, 0xE3A00000,
                0xE5A4C080, 0xE284C028, 0xE584500C, 0xE584A020
            };

            /*const std::vector<u32> UnloadCroPattern =
            {
                0xE92D4070, 0xEE1D4F70, 0xE59F502C, 0xE3A0C000,
                0xE5A45080, 0xE2846004, 0xE5840014, 0xE59F001C,
                0xE886100E, 0xE5900000, 0xEF000032, 0xE2001102
            };*/

            // TODO: the new plugin system make this useless
            u32     loadCroAddress = Utils::Search<u32>(0x00100000, Process::GetTextSize(), LoadCroPattern);

            if (loadCroAddress)
            {
                LightLock_Init(&g_onLoadCroLock);
                g_onLoadCroHook.Initialize(loadCroAddress, (u32)LoadCROHooked);
                g_onLoadCroHook.Enable();
            }
        }

        // Install svcMapMemoryBlock hook
        {
            u32 svcMapMemoryBlockAddr = 0x00100000 - 4;
            do
            {
                svcMapMemoryBlockAddr = Utils::Search<u32>(svcMapMemoryBlockAddr + 4, Process::GetTextSize(), {0xEF00001F});
                if (svcMapMemoryBlockAddr)
                {
                    for (int i = 0; i > -10; i--)
                    {
                        if (reinterpret_cast<u32*>(svcMapMemoryBlockAddr)[i] == 0xE3A03201) // MOV R3, #0x10000000
                        {
                            g_onSharedMemMapHook.Initialize(svcMapMemoryBlockAddr, (u32)OnSharedMemMap);
                            g_onSharedMemMapHook.SetFlags(USE_LR_TO_RETURN);
                            g_onSharedMemMapHook.Enable();
                            svcMapMemoryBlockAddr = 0; // Exit condition for while
                            break;
                        }
                    }
                }
            } while (svcMapMemoryBlockAddr);
        }

        // Init sdmc & paths
        InitFS();

        // Init System::Heap
        InitHeap();

        // Patch process before it starts & let the dev init some settings
        PatchProcess(settings);

        // Init hid properly depending on the settings
        if (settings.UseGameHidMemory)
        {
            // Check the svcMapMemoryBlock hook was installed properly
            if (!g_onSharedMemMapHook.IsEnabled())
                abort();
        } else {
            hidExitFake();
            hidInit();
        }

        // Init menu sounds.
        SoundEngineImpl::InitializeMenuSounds();

        // Load settings
        Preferences::LoadSettings();

        // Continue game
        svcSignalEvent(g_continueGameEvent);

        // Check threads priorities
        settings.ThreadPriority = std::min(settings.ThreadPriority, (u32)0x3E);

        if (GSP::Initialize())
        {
            ScreenImpl::Top->Flash((Color&)Color::Yellow);
            abort();
        }

        // Wait for the required time
        Sleep(settings.WaitTimeToBoot);

        svcCreateEvent(&g_keepEvent, RESET_ONESHOT);

        // Create plugin's main thread
        g_mainThread.Start(nullptr);

        // Reduce priority
        while (R_FAILED(svcSetThreadPriority(g_keepThreadHandle, settings.ThreadPriority - 1)));

        // Wait until Main Thread finished all it's initializing
        svcWaitSynchronization(g_keepEvent, U64_MAX);
        svcCloseHandle(g_keepEvent);

        Handle memLayoutChanged;

        svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_GET_ON_MEMORY_CHANGE_EVENT, (u32)&memLayoutChanged, 0);
        while (true)
        {
            if (svcWaitSynchronization(memLayoutChanged, 100000000ULL) == 0x09401BFE) // 0.1s
            {
                s32 event = PLGLDR__FetchEvent();

                if (event == PLG_SLEEP_ENTRY)
                {
                    SoundEngineImpl::NotifyAptEvent(APT_HookType::APTHOOK_ONSLEEP);
                    SystemImpl::AptStatus |= BIT(6);
                    PLGLDR__Reply(event);
                }
                else if (event == PLG_SLEEP_EXIT)
                {
                    SystemImpl::WakeUpFromSleep();
                    SoundEngineImpl::NotifyAptEvent(APT_HookType::APTHOOK_ONWAKEUP);
                    PLGLDR__Reply(event);
                }
                else if (event == PLG_ABOUT_TO_SWAP)
                {
                    OnPluginToSwap();

                    SoundEngineImpl::NotifyAptEvent(APT_HookType::APTHOOK_ONSUSPEND);

                    // Close csnd as it may be needed by other processes (4 sessions max.)
                    ncsndExit();

                    // Un-map hook memory
                    HookManager::Lock();
                    HookManager::PrepareToUnmapMemory();
                    svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x01E80000, 0x2000);

                    // Reply and wait
                    PLGLDR__Reply(event);

                    // Re-map hook memory
                    svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x1E80000, CUR_PROCESS_HANDLE,
                        __ctru_heap + __ctru_heap_size, 0x2000);

                    HookManager::RecoverFromUnmapMemory();
                    HookManager::Unlock();

                    // Init csnd again.
                    ncsndInit(false);

                    SoundEngineImpl::NotifyAptEvent(APT_HookType::APTHOOK_ONRESTORE);

                    OnPluginFromSwap();
                }
                else if (event == PLG_ABOUT_TO_EXIT)
                {
                    OnProcessExit();
                    ProcessImpl::SignalExit();

                    SoundEngineImpl::NotifyAptEvent(APT_HookType::APTHOOK_ONEXIT);
                    SoundEngineImpl::ClearMenuSounds();

                    SystemImpl::AptStatus |= BIT(3);
                    Scheduler::Exit();

                    // Close PluginMenu to quit main thread
                    PluginMenuImpl::ForceExit();

                    // Close some handles
                    ncsndExit();
                    if (settings.UseGameHidMemory)
                        hidExitFake();
                    else
                        hidExit();
                    cfguExit();
                    fsExit();
                    amExit();
                    acExit();
                    srvExit();

                    // Un-map hook wrapper memory
                    HookManager::Lock();
                    HookManager::PrepareToUnmapMemory();
                    svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x01E80000, 0x2000);

                    // This function do not return and exit the thread
                    PLGLDR__Reply(event);
                }
                continue;
            }
            // Memory layout changed, update memory
            ProcessImpl::UpdateMemRegions();
        }

    // exit
        svcCloseHandle(g_keepEvent);
        svcExitThread();
    }

    // Initialize most subsystem / Global variables
    void    Initialize(void)
    {
        // Init sysfont
        Font::Initialize();
        {
            // If /cheats/ doesn't exists, create it
            const char *dirpath = "/cheats";
            if (!Directory::IsExists(dirpath))
                Directory::Create(dirpath);
        }

        // Set AR file path
        Preferences::CheatsFile = "cheats.txt";

        // Default: cheats.txt in cwd
        if (!File::Exists(Preferences::CheatsFile))
            Preferences::CheatsFile = Utils::Format("/cheats/%016llX.txt", Process::GetTitleID());

        {
            // If /Screenshots/ doesn't exists, create it
            const char *dirpath = "/Screenshots";
            if (!Directory::IsExists(dirpath))
                Directory::Create(dirpath);

            // Set default screenshot path
            Screenshot::Path = dirpath;
            Screenshot::Path.append("/");

            // Set default screenshot prefix
            Screenshot::Prefix = "[";
            Process::GetName(Screenshot::Prefix);
            Screenshot::Prefix += Utils::Format(" - %08X] - Screenshot", (u32)Process::GetTitleID());
            Screenshot::Initialize();
        }
    }

    // Main thread's start
    void  ThreadInit(void *arg)
    {
        Initialize();

        // Wake up init thread
        svcSignalEvent(g_keepEvent);

        // Reduce thread priority
        svcSetThreadPriority(threadGetHandle(threadGetCurrent()), FwkSettings::Get().ThreadPriority);

        // Update memory layout
        ProcessImpl::UpdateMemRegions();

        // Start plugin
        main();

        ThreadExit();
    }

    void  ThreadExit(void)
    {
        // In which thread are we ?
        if (reinterpret_cast<u32>(((CThread_tag*)threadGetCurrent())->stacktop) < 0x07000000)
        {
            // ## Main Thread ##

            // Remove the OSD Hook
            OSDImpl::OSDHook.Disable();

            // Release process in case it's currently paused
            ProcessImpl::IsPaused = std::min((u32)ProcessImpl::IsPaused, (u32)1);
            ProcessImpl::Play(true);

            // Exit services
            gspExit();

            // Exit loop in keep thread
            g_keepRunning = false;
            svcSignalEvent(g_keepEvent);

            threadExit(1);
            return;
        }

        // ## Keep Thread ##
        if (g_mainThread.GetStatus() == ThreadEx::RUNNING)
        {
            ProcessImpl::Play(true);
            PluginMenuImpl::ForceExit();
            g_mainThread.Join(false);
        }
        else // We aborted in a very early stage, so just release the game and exit
            svcSignalEvent(g_continueGameEvent);
        svcExitThread();
    }

    extern "C"
    int   __entrypoint(int arg)
    {
        // Call early callback, with pointer to the 2 saved instructions
        if (EarlyCallback)
            EarlyCallback((u32*)arg);

        // Set ProcessImpl::MainThreadTls
        ProcessImpl::MainThreadTls = (u32)getThreadLocalStorage();
        // Set exception handlers
        ProcessImpl::EnableExceptionHandlers();

        // Create event
        svcCreateEvent(&g_continueGameEvent, RESET_ONESHOT);
        // Start ctrpf's primary thread
        svcCreateThread(&g_keepThreadHandle, KeepThreadMain, arg, (u32 *)&keepThreadStack[0x1000], 0x1A, 0);
        // Wait until basic initialization has been made before returning to game
        svcWaitSynchronization(g_continueGameEvent, U64_MAX);
        // Close the event
        svcCloseHandle(g_continueGameEvent);

        return 0;
    }
}
