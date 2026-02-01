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
    struct AudioObject {
        std::unique_ptr<SoLoud::Wav> m_pWav;
        unsigned int m_iHandle = UINT_MAX;
        // float m_fTimeBuffer = 0.0f;
        bool m_bAwaitFinish = true;
    };
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
    class SoundSystem {
        friend class Game;
    public:
        [[nodiscard]] static SoundSystem* GetInstance();
        void PlaySound(Audio_GUID iAudio);
        void PlayMusic(Audio_GUID iAudio);
        void RegisterSound(Audio_GUID iAudio, const std::string& sFileName, bool bAwaitFinish = true);
        void RegisterMusic(Audio_GUID iAudio, const std::string& sFileName);

        SoundSystem(const SoundSystem& obj) = delete;
        SoundSystem& operator=(const SoundSystem& obj) = delete;
        SoundSystem(SoundSystem&& obj) = delete;
        SoundSystem& operator=(SoundSystem&& obj) = delete;
        ~SoundSystem() = default;
    private:
        SoundSystem();
        static SoundSystem* m_pSoundSystem;

        std::unique_ptr<SoLoud::Soloud> m_pAudioPlayer;
        unsigned int m_iCurrentMusic = UINT_MAX;
        // std::unordered_map<std::string, AudioObject> m_AudioObjects;
        std::vector<AudioObject> m_AudioObjects;
    };
}
