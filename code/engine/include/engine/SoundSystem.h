//
// Created by Julian Kalb on 01/02/2026.
//

#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace SoLoud {
    class Wav;
    class Soloud;
}

namespace gl3::engine {
    /// @brief Object to bundle data used for handling a single audio file.
    struct AudioObject {
        std::unique_ptr<SoLoud::Wav> m_pWav;
        unsigned int m_iHandle = UINT_MAX;
        bool m_bAwaitFinish = true;
    };
    /// @brief Used to identify audio files in std::vector and improve readability. Needs to be expanded to add new tracks.
    enum Audio_GUID {
        UI_BUTTON_PRESS,
        UI_BUTTON_HOVER,
        UI_DRAG_SLIDER,
        WALKING_1,
        IMPACT_1,
        DAMAGE_1,
        USE_1,
        ADVANCE_1,
        VICTORY_1,
        L0SS_1,
        MUSIC_SELECTION,
        MUSIC_COMBAT_1,
        MUSIC_MAIN_MENU,
        AUDIO_GUID_LAST
    };
    class Game;
    /// @brief Singleton class for Audio handling. Uses soloud as a core framework.
    class SoundSystem {
        friend class Game;
    public:
        /// @brief Returns a Pointer to the single existing instance of this class.
        ///
        /// @return Pointer to singleton
        [[nodiscard]] static SoundSystem* GetInstance();
        /// @brief Play the audio object specified by its ID.
        ///
        /// @param iAudio Enum value of type Audio_GUID.
        void PlaySound(Audio_GUID iAudio);
        /// @brief Play the audio object specified by its ID.
        ///
        /// @param iAudio Enum value of type Audio_GUID.
        void PlayMusic(Audio_GUID iAudio);
        /// @return Current volume as a float between 0 and 1
        float GetVolume() const;
        /// @param newValue Value between 0 and 1.
        void SetVolume(float newValue) const;
        /// @brief If an audio object, that was registered as Music is currently playing, the playback is stopped.
        void StopMusic();
        /// @brief Register an audio file as an audio object to this classes' container.
        /// The file is set to SingleInstance().
        ///
        /// @param iAudio Enum value which will identify the audio object for usage in other classes
        /// @param sFileName String that contains the file name. File needs to be placed in ../assets/audio/
        /// @param bAwaitFinish Optional value. If set to false, each time PlaySound() is called the playback will start from the beginning.
        void RegisterSound(Audio_GUID iAudio, const std::string& sFileName, bool bAwaitFinish = true);
        /// @brief Register an audio file as an audio object to this classes' container.
        /// The file is set to SingleInstance() and to loop.
        ///
        /// @param iAudio Enum value which will identify the audio object for usage in other classes
        /// @param sFileName String that contains the file name. File needs to be placed in ../assets/audio/
        void RegisterMusic(Audio_GUID iAudio, const std::string& sFileName);

        SoundSystem(const SoundSystem& obj) = delete;
        SoundSystem& operator=(const SoundSystem& obj) = delete;
        SoundSystem(SoundSystem&& obj) = delete;
        SoundSystem& operator=(SoundSystem&& obj) = delete;
        ~SoundSystem();
    private:
        SoundSystem();
        static SoundSystem* m_pSoundSystem;

        std::unique_ptr<SoLoud::Soloud> m_pAudioPlayer;
        unsigned int m_iCurrentMusic = UINT_MAX;
        // std::unordered_map<std::string, AudioObject> m_AudioObjects;
        std::vector<AudioObject> m_AudioObjects;
    };
}
