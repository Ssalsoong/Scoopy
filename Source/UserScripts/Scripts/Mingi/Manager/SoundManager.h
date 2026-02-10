#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

#include "AudioClip.h"
#include "AudioSource.h"
#include "GameObject.h"
#include "Object.h"
#include "ResourceManager.h"
#include "Transform.h"

#include <array>
#include <string>
#include <unordered_map>

namespace MMMEngine
{
    class USERSCRIPTS SoundManager : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND

        struct AudioEntry
        {
            ResPtr<AudioClip> clip = nullptr;
            float baseVolume = 1.0f;
        };

        struct SFXEmitterSlot
        {
            ObjPtr<GameObject> emitter;
            ObjPtr<AudioSource> source;
            ObjPtr<Transform> followTarget;
            bool trackCaller = false;
            bool is3D = false;
            uint64_t playSerial = 0;
        };

        static constexpr int kSfxPoolSize = 16;
        static constexpr const char* kAudioCsvPath = "Assets/Sound/AllAudioClips.csv";

        std::unordered_map<std::string, AudioEntry> m_bgmTable;
        std::unordered_map<std::string, AudioEntry> m_sfxTable;

        ObjPtr<AudioSource> m_bgmSource;
        std::array<SFXEmitterSlot, kSfxPoolSize> m_sfxSlots{};
        uint64_t m_playSerialCounter = 0;

        void LoadAudioTable();
        void EnsureBGMSource();
        void EnsureSFXPool();

        int AcquireSFXSlot();
        ObjPtr<Transform> ResolveCallerTransform(const ObjPtr<Component>& caller) const;
        void PlaySFXInternal(const std::string& key, const ObjPtr<Component>& caller, bool spatialize, bool trackCaller, float volumeScale);

        static std::string Trim(const std::string& s);
        static bool TryParseCsvRow(const std::string& line, std::array<std::string, 4>& outRow);

    public:
        SoundManager()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        static ObjPtr<SoundManager> Instance;

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
        void OnDestroy();

        void PlayBGM(const std::string& key, float volumeScale = 1.0f, bool loop = true);
        void StopBGM();

        void PlaySFX2D(const std::string& key, const ObjPtr<Component>& caller, float volumeScale = 1.0f);
        void PlaySFX3D(const std::string& key, const ObjPtr<Component>& caller, bool trackCaller, float volumeScale = 1.0f);
        void StopAllSFX();
    };
}
