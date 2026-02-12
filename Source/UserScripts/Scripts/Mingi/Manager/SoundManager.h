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
#include <cstdint>
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
            std::string currentKey;
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
        uint64_t PlaySFXInternal(const std::string& key, const ObjPtr<Component>& caller, bool spatialize, bool trackCaller, float volumeScale, bool loop);

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

        /** @return 재생 ID (0 = 재생 실패). 이 ID로 StopSFXByID 호출 시 해당 인스턴스만 정지 */
        uint64_t PlaySFX2D(const std::string& key, const ObjPtr<Component>& caller, float volumeScale = 1.0f, bool loop = false);
        /** @return 재생 ID (0 = 재생 실패). 이 ID로 StopSFXByID 호출 시 해당 인스턴스만 정지 */
        uint64_t PlaySFX3D(const std::string& key, const ObjPtr<Component>& caller, bool trackCaller, float volumeScale = 1.0f, bool loop = false);
        /** key에 해당하는 모든 SFX 인스턴스 정지 */
        void StopSFX(const std::string& key);
        /** 특정 재생 인스턴스만 정지 (PlaySFX2D/PlaySFX3D 반환 ID 사용) */
        void StopSFXByID(uint64_t playId);
        void StopAllSFX();
    };
}
