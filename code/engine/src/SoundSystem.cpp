//
// Created by Julian Kalb on 01/02/2026.
//

#include "../include/engine/SoundSystem.h"
#include <soloud.h>
#include <soloud_wav.h>

#include "engine/util/Assets.h"

namespace gl3::engine {
    SoundSystem* SoundSystem::m_pSoundSystem = nullptr;

    SoundSystem* SoundSystem::GetInstance() {
        if (!m_pSoundSystem) m_pSoundSystem = new SoundSystem();
        return m_pSoundSystem;
    }

    void SoundSystem::PlaySound(const std::string& sFileName) {
        auto& sound = m_Sounds[sFileName];
        if (m_pAudioPlayer->isValidVoiceHandle(sound.m_iHandle)) {
            return;
        }
        sound.m_iHandle = m_pAudioPlayer->play(*m_Sounds[sFileName].m_pWav);
    }

    void SoundSystem::PlayMusic(const std::string& sFileName) {
        auto& music = m_ListMusic[sFileName];
        if (music == m_pCurrentMusic) return;
        if (m_iCurrentMusic > -1) m_pAudioPlayer->stop(m_iCurrentMusic);
        m_iCurrentMusic = m_pAudioPlayer->play(*m_ListMusic[sFileName]);
        m_pCurrentMusic = m_ListMusic[sFileName];
    }

    SoundSystem::SoundSystem() {
        m_pAudioPlayer = std::make_unique<SoLoud::Soloud>();
        m_pAudioPlayer->init();
        m_pAudioPlayer->setGlobalVolume(0.4f);
    }

    void SoundSystem::RegisterSound(const std::string& sFileName) {
        SoundObject soundObject;
        soundObject.m_pWav = std::make_unique<SoLoud::Wav>();
        soundObject.m_pWav->load(resolveAssetPath("audio/" + sFileName).string().c_str());
        soundObject.m_pWav->setSingleInstance(true);
        m_Sounds[sFileName] = std::move(soundObject);
    }

    void SoundSystem::RegisterMusic(const std::string& sFileName) {
        auto music = std::make_unique<SoLoud::Wav>();
        music->load(resolveAssetPath("audio/" + sFileName).string().c_str());
        music->setSingleInstance(true);
        music->setLooping(true);
        m_ListMusic[sFileName] = std::move(music);
    }
}
