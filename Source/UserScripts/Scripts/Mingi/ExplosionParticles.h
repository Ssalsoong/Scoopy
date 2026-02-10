#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class ExplosionPool;
    class ParticleRenderer;
    class USERSCRIPTS ExplosionParticles : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            bool m_inUse = false;
    public:
        ExplosionParticles()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
            void OnEnable();


        USCRIPT_MESSAGE()
            void Update();

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle1;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle2;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle3;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle4;

        ObjPtr<ExplosionPool> owner;
    };
}
