#ifndef CTRPLUGINFRAMEWORK_SYSTEM_HOOK_H
#define CTRPLUGINFRAMEWORK_SYSTEM_HOOK_H

#include "types.h"
#include <type_traits>

namespace CTRPluginFramework
{
    enum
    {
        USE_LR_TO_RETURN = 1 << 0,      ///< Enable the use of bx lr to return from callback. LR will be properly restored after the callback is executed
        EXECUTE_OI_BEFORE_CB = 1 << 1,  ///< If the instruction overwritten by the hook (target) must be executed before the callback
        EXECUTE_OI_AFTER_CB = 1 << 2,   ///< If the instruction overwritten by the hook (target) must be executed after the callback
        MITM_MODE = 1 << 3,             ///< This mode is perfect to intercepts functions calls
        WRAP_SUB = 1 << 4,              ///< Wraps a subroutine call

        HOOK_DEFAULT_PARAMS = USE_LR_TO_RETURN | EXECUTE_OI_BEFORE_CB,
    };

    enum class HookResult
    {
        Success,
        InvalidContext,     ///< The HookContext obj linked to the Hook obj is invalid (Should not happen)
        InvalidAddress,     ///< The target address was not reachable
        AddressAlreadyHooked,   ///< A hook is already enabled to the same address
        TooManyHooks,       ///< You attained the maximum of enabled hooks (current limit: 91)
        HookParamsError,     ///< The parameters of your hook seems off
        TargetInstructionCannotBeHandledAutomatically, ///< The target's instruction is position dependent (PC) hence using ExecuteOverwrittenInstructionBeforeCallback or ExecuteOverwrittenInstructionAfterCallback is impossible.
    };

    struct  HookContext
    {
        s32     refcount;
        u32     flags;
        u32     targetAddress;
        u32     returnAddress;
        u32     callbackAddress;
        u32     callbackAddress2;
        u32     overwrittenInstr;
        u32     index;

        // Bunch of functions to be used inside a hook's function body
        static HookContext&    GetCurrent(void);

        // The functions below are to be used only with MITM_MODE hooks
        template <typename TResult, typename... Args>
        typename std::enable_if<!std::is_same<TResult, void>::value, TResult>::type
        OriginalFunction(Args... args)
        {
            using FPtr = TResult(*)(Args...);

            FPtr    fptr = reinterpret_cast<FPtr>(GetCallCode());

            if (fptr)
                return fptr(args...);

            return (TResult)fptr;
        }

        template <typename TResult, typename... Args>
        typename std::enable_if<std::is_same<TResult, void>::value, void>::type
        OriginalFunction(Args... args)
        {
            using FPtr = void(*)(Args...);

            FPtr    fptr = reinterpret_cast<FPtr>(GetCallCode());

            if (fptr)
                fptr(args...);
        }

    private:
        void    *GetCallCode(void);
    };

    struct  Hook
    {
        Hook(void);
        // Warning: copies share the same HookContext
        Hook(const Hook& hook);
        Hook(Hook&& hook) noexcept;
        Hook& operator=(const Hook& hook);
        Hook& operator=(Hook&& hook) noexcept;
        ~Hook(void);

        /**
         * \brief Initialize hook target and callback. Hook's return address is set to targetAddr + 4
         * \param targetAddr The address to hook to
         * \param callbackAddr The callback to be called by the hook
         * \return A reference to Hook obj
         */
        Hook&       Initialize(u32 targetAddr, u32 callbackAddr);

        /**
         * \brief Initialize a hook for mitm mode (apply the required flags)
         * \param targetAddr The address to hook to
         * \param callbackAddr The callback to be called by the hook
         * \return A reference to Hook obj
         */
        Hook&       InitializeForMitm(u32 targetAddr, u32 callbackAddr);

         /**
         * \brief Initialize a hook for subroutine wrapping (apply the required flags)
         * \param targetAddr The address with the subroutine call to wrap (BL only)
         * \param beforeCallback The callback to be called before the subroutine (can be null)
         * \param afterCallback The callback to be called after the subroutine (can be null)
         * \return A reference to Hook obj
         */
        Hook&       InitializeForSubWrap(u32 targetAddr, u32 beforeCallback, u32 afterCallback);

        /**
         * \brief Apply the specified flags to the hook. Must be done before enabling the hook otherwise nothing happens
         * \param flags The new hook settings
         * \return A reference to the Hook obj
         */
        Hook&       SetFlags(u32 flags);

        /**
         * \brief Set the return address of the hook (default: target + 4)
         * \param returnAddr The address for the hook to return to
         * \return A reference to the Hook obj
        */
        Hook&       SetReturnAddress(u32 returnAddr);


        /**
         * \brief Check if the hook is applied or not
         * \return true if the hook is applied, false otherwise
         */
        bool        IsEnabled(void);

        /**
         * \brief Apply the hook
         * \return Return the result of the operation (see HookResult for more infos)
         */
        HookResult  Enable(void);

        /**
         * \brief Disable the hook
         * \return Return the result of the operation (see HookResult for more infos)
         */
        HookResult  Disable(void);

        const HookContext&    GetContext(void) const;

    private:
        HookContext     *_ctx;
    };

    // Bunch of functions to be used inside a hook's function body (asm)
    extern "C"
    {
         /**
         * \brief Get a pointer to the current hook's context (see HookContext)
         * \return A pointer to current HookContext
         */
        void *  ctrpfHook__GetCurrent(void);

        /**
         * \brief Execute the original function. Only available for mitm hooks.
         * If the function takes some args, just set the registers accordingly
         * \return return value of original function
         */
        void *  ctrpfHook__ExecuteOriginalFunction(void);
    }
}

#endif
