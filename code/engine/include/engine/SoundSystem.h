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
    class Game;
    class SoundSystem {
        friend class Game;
    public:
        [[nodiscard]] static SoundSystem* GetInstance();
        void PlaySound(const std::string& sFileName);
        void PlayMusic(const std::string& sFileName);
        void RegisterSound(const std::string& sFileName, bool bAwaitFinish = true);
        void RegisterMusic(const std::string& sFileName);

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
        std::unordered_map<std::string, AudioObject> m_AudioObjects;
    };
}
