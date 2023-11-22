#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUNDENGINE_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUNDENGINE_HPP

#include "CTRPluginFramework/Sound/Sound.hpp"

namespace CTRPluginFramework
{
    class SoundEngine
    {
    public:

        /// Menu sound event types.
        enum class Event {
            CURSOR = 0, ///< Movement of the cursor.
            ACCEPT,     ///< Acces menu or entry, pressing OK, etc.
            CANCEL,     ///< Exit menu or entry, pressing Cancel, etc.
            SELECT,     ///< Selecing a button or enabling entry.
            DESELECT,   ///< Deselecting a button or disabling entry.

            NUM_EVENTS  ///< Total amount of events.
        };

        /**
         * \brief Replaces the sound associated to the event with the new sound.
         * \param eventType Event to assign the sound to.
         * \param sound Sound to assign to the event.
         * \return Whether the operation was successful or not.
        */
        static bool RegisterMenuSoundEvent(Event eventType, Sound& sound);

        /**
         * \brief Gets a reference to the sound associated with an event.
         * \param eventType Event to get the sound from.
         * \return Reference to the sound associated with the event.
        */
        static Sound& GetMenuSoundEvent(SoundEngine::Event eventType);

        /**
         * \brief Plays the sound associated with an event.
         * \param eventTypes Event to play its sound.
         * \return Result from the CWAVStatus enum.
        */
        static Sound::CWAVStatus PlayMenuSound(Event eventType);

        /**
         * \brief Stops the sound associated with an event.
         * \param eventTypes Event to stop its sound.
        */
        static void StopMenuSound(Event eventType);

        /**
         * \brief Removes the sound associated to an event.
         * \param eventType Event to remove the sound.
        */
        static void DeRegisterMenuSoundEvent(Event eventType);

    };
}

#endif