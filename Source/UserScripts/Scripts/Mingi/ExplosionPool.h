#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class ExplosionParticles;
    class Prefab;
    class USERSCRIPTS ExplosionPool : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ExplosionPool()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);

        }
        
        static ObjPtr<ExplosionPool> Instance;

        USCRIPT_MESSAGE()
        void Awake();

        // 풀에서 하나 꺼내서 재생
        void Spawn(const DirectX::SimpleMath::Vector3& position);

        // ExplosionParticles 가 끝났을 때 호출
        void Release(ObjPtr<ExplosionParticles> effect);

        USCRIPT_MESSAGE()
        void OnDestroy();

        USCRIPT_PROPERTY()
        ResPtr<Prefab> ExplosionPrefab;

        std::vector<ObjPtr<ExplosionParticles>> m_pool;
    };
}
