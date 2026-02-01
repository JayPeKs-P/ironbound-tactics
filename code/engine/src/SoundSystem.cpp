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
        auto& sound = m_AudioObjects[sFileName];
        if (m_pAudioPlayer->isValidVoiceHandle(sound.m_iHandle) && sound.m_bAwaitFinish) return;

        sound.m_iHandle = m_pAudioPlayer->play(*sound.m_pWav);
    }

    void SoundSystem::PlayMusic(const std::string& sFileName) {
        auto& music = m_AudioObjects[sFileName];
        if (m_pAudioPlayer->isValidVoiceHandle(music.m_iHandle)) return;

        if (m_iCurrentMusic != UINT_MAX) m_pAudioPlayer->stop(m_iCurrentMusic);
        music.m_iHandle = m_pAudioPlayer->play(*music.m_pWav);
        m_iCurrentMusic = music.m_iHandle;
    }

    SoundSystem::SoundSystem() {
        m_pAudioPlayer = std::make_unique<SoLoud::Soloud>();
        m_pAudioPlayer->init();
        m_pAudioPlayer->setGlobalVolume(0.4f);
    }

    void SoundSystem::RegisterSound(const std::string& sFileName, bool bAwaitFinish) {
        AudioObject soundObject;
        soundObject.m_pWav = std::make_unique<SoLoud::Wav>();
        soundObject.m_pWav->load(resolveAssetPath("audio/" + sFileName).string().c_str());
        soundObject.m_pWav->setSingleInstance(true);
        soundObject.m_bAwaitFinish = bAwaitFinish;
        m_AudioObjects[sFileName] = std::move(soundObject);
    }

    void SoundSystem::RegisterMusic(const std::string& sFileName) {
        AudioObject musicObject;
        musicObject.m_pWav = std::make_unique<SoLoud::Wav>();
        musicObject.m_pWav->load(resolveAssetPath("audio/" + sFileName).string().c_str());
        musicObject.m_pWav->setSingleInstance(true);
        musicObject.m_pWav->setLooping(true);
        m_AudioObjects[sFileName] = std::move(musicObject);
    }
}
