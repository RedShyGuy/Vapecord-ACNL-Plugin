#ifndef CTRPLUGINFRAMEWORK_CONTROLLER_HPP
#define CTRPLUGINFRAMEWORK_CONTROLLER_HPP

#include "types.h"
#include "Vector.hpp"

namespace CTRPluginFramework
{
    enum Key
    {
        A           = 1,
        B           = 1 << 1,
        Select      = 1 << 2,
        Start       = 1 << 3,
        DPadRight   = 1 << 4,
        DPadLeft    = 1 << 5,
        DPadUp      = 1 << 6,
        DPadDown    = 1 << 7,
        R           = 1 << 8,
        L           = 1 << 9,
        X           = 1 << 10,
        Y           = 1 << 11,
        ZL          = 1 << 14,               ///< The ZL button (New 3DS only)
        ZR          = 1 << 15,               ///< The ZR button (New 3DS only)
        Touchpad    = 1 << 20,
        CStickRight = 1 << 24,
        CStickLeft  = 1 << 25,
        CStickUp    = 1 << 26,
        CStickDown  = 1 << 27,
        CPadRight   = 1 << 28,
        CPadLeft    = 1 << 29,
        CPadUp      = 1 << 30,
        CPadDown    = 1 << 31,
        Up          = DPadUp    | CPadUp,
        Down        = DPadDown  | CPadDown,
        Left        = DPadLeft  | CPadLeft,
        Right       = DPadRight | CPadRight,
        CPad        = CPadLeft  | CPadRight | CPadUp | CPadDown,
        CStick      = CStickLeft | CStickRight | CStickUp | CStickDown
    };
    class Controller
    {
    public:
        /**
         * @brief Get the keys being held down
         *
         * @param includePressed Whether to include the keys that just got pressed
         * @return Keys being held down
         */
        static inline u32      GetKeysDown(bool includePressed = true)
        {
            return ((includePressed ? _keysDown : 0) | _keysHeld);
        }

        /**
         * @brief Get all the keys that just got pressed
         *
         * @return Keys that just got pressed
         */
        static inline u32      GetKeysPressed(void)
        {
            return _keysDown;
        }

        /**
         * @brief Get all the keys that just were released
         *
         * @return Keys that just were released
         */
        static inline u32      GetKeysReleased(void)
        {
            return _keysReleased;
        }

        /**
         * @brief Check if the key is being held
         *
         * @param key Key to check
         * @return Whether the key is being held
         */
        static inline bool     IsKeyDown(Key key)
        {
            return (GetKeysDown(false) & static_cast<u32>(key)) != 0;
        }

        /**
         * @brief Check if the key just got pressed
         *
         * @param key Key to check
         * @return Whether the key just got pressed
         */
        static inline bool     IsKeyPressed(Key key)
        {
            return (GetKeysPressed() & static_cast<u32>(key)) != 0;
        }

        /**
         * @brief Check if the key just got released
         *
         * @param key Key to check
         * @return Whether the key just got released
         */
        static inline bool     IsKeyReleased(Key key)
        {
            return (GetKeysReleased() & static_cast<u32>(key)) != 0;
        }

        /**
         * @brief Check if the key combination is being held
         *
         * @param keys Key combination to check
         * @return Whether the key combination is being held
         */
        static inline bool     IsKeysDown(u32 keys)
        {
            return ((GetKeysDown(false) & keys) == keys);
        }

        /**
         * @brief Check if the key combination was just pressed.
         *
         * @param keys Key combination to check
         * @return Whether the key combination was just pressed
         */
        static inline bool     IsKeysPressed(u32 keys)
        {
            return ((GetKeysPressed() & keys) && (GetKeysDown(true) & keys) == keys);
        }

        /**
         * @brief Check if the keys combo just was released at the same time.
         *
         * @param keys Key combination to check
         * @return true Whether the key combination was just released
         */
        static inline bool     IsKeysReleased(u32 keys)
        {
            return ((GetKeysReleased() & keys) == keys);
        }

        /**
         * @brief Gets the circle pad position.
         *
         * @return Vector containing the x and y positions of the circle pad.
         */
        static shortVector  GetCirclePadPosition();

        /**
         * @brief Update the controller status.
         *
         */
        static void     Update(void);

        /**
         * @brief Inject a touch in the specified coordinates. May not work depending on the game timing.
         *
         * @param posX Touch position X
         * @param posY Touch position Y
         */
        static void     InjectTouch(u16 posX, u16 posY);

        /**
         * @brief Inject the specified key. May not work depending on the game timing.
         *
         * @param key Key to inject
         */
        static void     InjectKey(u32 key);
    private:
        static u32      _keysDown;
        static u32      _keysHeld;
        static u32      _keysReleased;
    };
}

#endif