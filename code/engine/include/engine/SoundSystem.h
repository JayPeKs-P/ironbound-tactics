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
    struct SoundObject {
        std::unique_ptr<SoLoud::Wav> m_pWav;
        unsigned int m_iHandle = -1;
        float m_fTimeBuffer = 0.0f;
    };
    class Game;
    class SoundSystem {
        friend class Game;
    public:
        [[nodiscard]] static SoundSystem* GetInstance();
        void PlaySound(const std::string& sFileName);
        void PlayMusic(const std::string& sFileName);
        void RegisterSound(const std::string& sFileName);
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
        std::shared_ptr<SoLoud::Wav> m_pCurrentMusic;
        int m_iCurrentMusic = -1;
        std::unordered_map<std::string, std::shared_ptr<SoLoud::Wav>> m_ListMusic;
        std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> m_ListSound;
        std::unordered_map<std::string, SoundObject> m_Sounds;
        std::unordered_map<std::string, unsigned int> m_ListHandles;
    };
}
