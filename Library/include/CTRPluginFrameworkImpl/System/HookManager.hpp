#ifndef CTRPLUGINFRAMEWORKIMPL_SYSTEM_HOOKMANAGER_HPP
#define CTRPLUGINFRAMEWORKIMPL_SYSTEM_HOOKMANAGER_HPP

#include "types.h"
#include "CTRPluginFramework/System/Clock.hpp"
#include "CTRPluginFramework/System/Mutex.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include <vector>

namespace CTRPluginFramework
{
    #define MAX_HOOKS (0x1000 / sizeof(HookWrapper))

    struct HookWrapper
    {
        u32             jumpCode{0};   // ldr pc, [pc, #-4]
        u32             callback{0};   // Asm wrapper
        HookContext *   ctx{nullptr};  // to be set in tls[15]
    };

    struct  AsmWrapper
    {
        HookContext *   ctx{nullptr};
        u32             code[15]{0};
    };

    /*struct HookWrapper
    {
        u32     backupAndUpdateLR;  ///< Default: nop
        u32     overwrittenInstr;   ///< Default: nop
        u32     setLR[2];           ///< Default: nop
        u32     jumpToCallback;     ///< ldr pc, [pc, #-4]
        u32     callbackAddress;    ///< Address of the code to jump to
        u32     restoreLR;          ///< Default: nop
        u32     overwrittenInstr2;  ///< Default: nop
        u32     jumpBackToGame;     ///< ldr pc, [pc, #-4]
        u32     returnAddress;
        u32     lrBackup;
    }; */

    struct HookManager
    {
        HookManager(void);

        static Mutex&   GetLock(void)
        {
            return _singleton._mutex;
        }

        static HookResult   ApplyHook(HookContext& ctx);
        static HookResult   DisableHook(HookContext& ctx);

        AsmWrapper&     GetFreeAsmWrapper(void);
        HookWrapper&    GetFreeHookWrapper(s32& index);
        AsmWrapper&     GetAsmWrapper(HookContext *ctx);

        static void            Lock();
        static void            Unlock();

        static void            PrepareToUnmapMemory();
        static void            RecoverFromUnmapMemory();

        Mutex                       _mutex{};
        HookWrapper *               _hookWrappers{reinterpret_cast<HookWrapper *>(0x1E80000)};
        std::vector<AsmWrapper>     _asmWrappers{};

        static HookManager          _singleton;
    };
}

#endif
